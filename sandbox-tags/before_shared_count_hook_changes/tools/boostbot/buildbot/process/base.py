# -*- test-case-name: buildbot.test.test_step -*-

import types, time
from StringIO import StringIO

from twisted.python import log, components
from twisted.python.failure import Failure
from twisted.internet import reactor, defer, error
from twisted.spread import pb
import twisted.web.util

from buildbot import interfaces
from buildbot.util import now
from buildbot.status.builder import SUCCESS, WARNINGS, FAILURE, EXCEPTION
from buildbot.status.builder import Results
from buildbot.status.progress import BuildProgress

class Build:
    """I represent a single build by a single bot. Specialized Builders can
    use subclasses of Build to hold status information unique to those build
    processes.

    I am responsible for two things:
      1. deciding B{when} a build should occur.  This involves knowing
         which file changes to ignore (documentation or comments files,
         for example), and deciding how long to wait for the tree to
         become stable before starting.  The base class pays attention
         to all files, and waits 10 seconds for a stable tree.
      
      2. controlling B{how} the build proceeds.  The actual build is
         broken up into a series of steps, saved in the .buildSteps[]
         array as a list of L{buildbot.process.step.BuildStep}
         objects. Each step is a single remote command, possibly a shell
         command.

    Before the build is started, I accumulate Changes and track the
    tree-stable timers and interlocks necessary to decide when I ought to
    start building.

    During the build, I don't do anything interesting.

    After the build, I hold historical data about the build, like how long
    it took, tree size, lines of code, etc. It is expected to be used to
    generate graphs and quantify long-term trends. It does not hold any
    status events or build logs.

    I can be used by a factory by setting buildClass on
    L{buildbot.process.factory.BuildFactory}
    """

    treeStableTimer = 10 #*60
    workdir = "build"
    build_status = None
    reason = "changes"
    sourceStamp = (None, None)
    finished = False
    results = None

    def __init__(self):
        self.unimportantChanges = []
        self.changes = []
        self.failedChanges = []
        self.maxChangeNumber = None
        # .timer and .nextBuildTime are only set while we are in the
        # Builder's 'waiting' slot
        self.timer = None
        self.nextBuildTime = None
        self.interlocks = []
        self.abandoned = False

        self.progress = None
        self.currentStep = None
        self.slaveEnvironment = {}

    def setBuilder(self, builder):
        """
        Set the given builder as our builder.

        @type  builder: L{buildbot.process.builder.Builder}
        """
        self.builder = builder

    def setSourceStamp(self, baserev, patch, reason="try"):
        # sourceStamp is (baserevision, (patchlevel, diff))
        self.sourceStamp = (baserev, patch)
        self.reason = reason

    def isFileImportant(self, filename):
        """
        I check if the given file is important enough to trigger a rebuild.

        Override me to ignore unimporant files: documentation, .cvsignore
        files, etc. 

        The timer is not restarted, so a checkout may occur in the middle of
        a set of changes marked 'unimportant'. Also, the checkout may or may
        not pick up the 'unimportant' changes. The implicit assumption is
        that any file marked 'unimportant' is incapable of affecting the
        results of the build.

        @param filename: name of a file to check, relative to the VC base
        @type  filename: string
      
        @rtype: 0 or 1
        @returns: whether the change to this file should trigger a rebuild
        """
        return 1
    
    def bumpMaxChangeNumber(self, change):
        if not self.maxChangeNumber:
            self.maxChangeNumber = change.number
        if change.number > self.maxChangeNumber:
            self.maxChangeNumber = change.number

    def addChange(self, change):
        """
        Add the change, deciding if the change is important or not. 
        Called by L{buildbot.process.builder.filesChanged}

        @type  change: L{buildbot.changes.changes.Change}
        """
        important = 0
        for filename in change.files:
            if self.isFileImportant(filename):
                important = 1
                break
        if important:
            self.addImportantChange(change)
        else:
            self.addUnimportantChange(change)

    def addImportantChange(self, change):
        self.bumpMaxChangeNumber(change)
        self.changes.append(change)
        self.nextBuildTime = change.when + self.treeStableTimer
        self.setTimer(self.nextBuildTime)
        self.builder.updateBigStatus()
            
    def addUnimportantChange(self, change):
        self.unimportantChanges.append(change)

    def allChanges(self):
        return self.changes + self.failedChanges + self.unimportantChanges

    def allFiles(self):
        # return a list of all source files that were changed
        files = []
        havedirs = 0
        for c in self.changes + self.unimportantChanges:
            for f in c.files:
                files.append(f)
            if c.isdir:
                havedirs = 1
        return files

    def failMerge(self, b):
        for c in b.unimportantChanges + b.changes + b.failedChanges:
            self.bumpMaxChangeNumber(c)
            self.failedChanges.append(c)
    def merge(self, b):
        self.unimportantChanges.extend(b.unimportantChanges)
        self.failedChanges.extend(b.failedChanges)
        self.changes.extend(b.changes)
        for c in b.unimportantChanges + b.changes + b.failedChanges:
            self.bumpMaxChangeNumber(c)

    def getSourceStamp(self):
        return self.sourceStamp

    def setTimer(self, when):
        log.msg("setting timer to %s" %
                time.strftime("%H:%M:%S", time.localtime(when)))
        if when < now():
            when = now() + 1
        if self.timer:
            self.timer.cancel()
        self.timer = reactor.callLater(when - now(), self.fireTimer)
    def stopTimer(self):
        if self.timer:
            self.timer.cancel()
            self.timer = None

    def fireTimer(self):
        """
        Fire the build timer on the builder.
        """
        self.timer = None
        self.nextBuildTime = None
        # tell the Builder to deal with us
        self.builder.buildTimerFired(self)

    def checkInterlocks(self, interlocks):
        assert interlocks
        # Build.interlocks is a list of the ones we are waiting for. As each
        # deferred fires, we remove one from the list. When the last one
        # fires, the build is started. When the first one fails, the build
        # is abandoned.

        # This could be done with a DeferredList, but we track the actual
        # Interlocks so we can provide better status information (i.e.
        # *which* interlocks the build is waiting for).

        self.interlocks = interlocks[:]
        for interlock in interlocks:
            d = interlock.hasPassed(self.maxChangeNumber)
            d.addCallback(self.interlockDone, interlock)
        # wait for all of them to pass

    def interlockDone(self, passed, interlock):
        # one interlock has finished
        self.interlocks.remove(interlock)
        if self.abandoned:
            return
        if passed and not self.interlocks:
            # that was the last holdup, we are now .buildable
            self.builder.interlockPassed(self)
        else:
            # failed, do failmerge
            self.abandoned = True
            self.builder.interlockFailed(self)

    def __repr__(self):
        return "<Build %s>" % (self.builder.name)

    def __getstate__(self):
        d = self.__dict__.copy()
        if d.has_key('remote'):
            del d['remote']
        d['timer'] = None
        return d
    def __setstate__(self, state):
        self.__dict__ = state
        if self.nextBuildTime:
            self.setTimer(self.nextBuildTime)

    def blamelist(self):
        who = {}
        for c in self.unimportantChanges + self.changes + self.failedChanges:
            who[c.who] = 1
        blamelist = who.keys()
        blamelist.sort()
        return blamelist
    def changesText(self):
        changetext = ""
        for c in self.failedChanges + self.unimportantChanges + self.changes:
            changetext += "-" * 60 + "\n\n" + c.asText() + "\n"
        # consider sorting these by number
        return changetext

    def setSteps(self, steps):
        """Set a list of StepFactories, which are generally just class
        objects which derive from step.BuildStep . These are used to create
        the Steps themselves when the Build starts (as opposed to when it is
        first created). By creating the steps later, their __init__ method
        will have access to things like build.allFiles() ."""
        self.stepFactories = steps # tuples of (factory, kwargs)
        for s in steps:
            pass




    useProgress = True

    def startBuild(self, build_status, expectations, remote):
        """This method sets up the build, then starts it by invoking the
        first Step. It returns a Deferred which will fire when the build
        finishes."""

        log.msg("%s.startBuild" % self)
        self.build_status = build_status
        self.remote = remote
        self.remote.notifyOnDisconnect(self.lostRemote)
        d = self.deferred = defer.Deferred()

        try:
            self.setupBuild(expectations) # create .steps
        except:
            # the build hasn't started yet, so log the exception as a point
            # event instead of flunking the build. TODO: associate this
            # failure with the build instead. this involves doing
            # self.build_status.buildStarted() from within the exception
            # handler
            log.msg("Build.setupBuild failed")
            log.err(Failure())
            self.builder.builder_status.addPointEvent(["setupBuild",
                                                       "exception"],
                                                      color="purple")
            self.finished = True
            self.results = FAILURE
            self.deferred = None
            d.callback(self)
            return d

        self.build_status.buildStarted(self)
        self.startNextStep()
        return d

    def setupBuild(self, expectations):
        # create the actual BuildSteps. If there are any name collisions, we
        # add a count to the loser until it is unique.
        self.steps = []
        self.stepStatuses = {}
        stepnames = []
        sps = []

        for factory, args in self.stepFactories:
            args = args.copy()
            if not args.has_key("workdir"):
                args['workdir'] = self.workdir
            step = factory(build=self, **args)
            name = step.name
            count = 1
            while name in stepnames and count < 100:
                count += 1
                name = step.name + "_%d" % count
            if name in stepnames:
                raise RuntimeError("duplicate step '%s'" % step.name)
            step.name = name
            stepnames.append(name)
            self.steps.append(step)

            # tell the BuildStatus about the step. This will create a
            # BuildStepStatus and bind it to the Step.
            self.build_status.addStep(step)

            sp = None
            if self.useProgress:
                # XXX: maybe bail if step.progressMetrics is empty? or skip
                # progress for that one step (i.e. "it is fast"), or have a
                # separate "variable" flag that makes us bail on progress
                # tracking
                sp = step.setupProgress()
            if sp:
                sps.append(sp)

        # Create a buildbot.status.progress.BuildProgress object. This is
        # called once at startup to figure out how to build the long-term
        # Expectations object, and again at the start of each build to get a
        # fresh BuildProgress object to track progress for that individual
        # build. TODO: revisit at-startup call

        if self.useProgress:
            self.progress = BuildProgress(sps)
            if self.progress and expectations:
                self.progress.setExpectationsFrom(expectations)

        # we are now ready to set up our BuildStatus.
        self.build_status.setSourceStamp(self.maxChangeNumber)
        self.build_status.setReason(self.reason)
        self.build_status.setChanges(self.changes +
                                     self.failedChanges +
                                     self.unimportantChanges)
        self.build_status.setBlamelist(self.blamelist())
        self.build_status.setProgress(self.progress)

        self.results = [] # list of FAILURE, SUCCESS, WARNINGS, SKIPPED
        self.result = SUCCESS # overall result, may downgrade after each step
        self.text = [] # list of text string lists (text2)

    def getNextStep(self):
        """This method is called to obtain the next BuildStep for this build.
        When it returns None (or raises a StopIteration exception), the build
        is complete."""
        if not self.steps:
            return None
        return self.steps.pop(0)

    def startNextStep(self):
        try:
            s = self.getNextStep()
        except StopIteration:
            s = None
        if not s:
            return self.allStepsDone()
        self.currentStep = s
        d = defer.maybeDeferred(s.startStep, self.remote)
        d.addCallback(self._stepDone, s)
        d.addErrback(self.buildException)

    def _stepDone(self, results, step):
        self.currentStep = None
        if self.finished:
            return # build was interrupted, don't keep building
        terminate = self.stepDone(results, step) # interpret/merge results
        if terminate:
            return self.allStepsDone()
        self.startNextStep()

    def stepDone(self, result, step):
        """This method is called when the BuildStep completes. It is passed a
        status object from the BuildStep and is responsible for merging the
        Step's results into those of the overall Build."""

        terminate = False
        text = None
        if type(result) == types.TupleType:
            result, text = result
        assert type(result) == type(SUCCESS)
        log.msg(" step '%s' complete: %s" % (step.name, Results[result]))
        self.results.append(result)
        if text:
            self.text.extend(text)
        if not self.remote:
            terminate = True
        if result == FAILURE:
            if step.warnOnFailure:
                if self.result != FAILURE:
                    self.result = WARNINGS
            if step.flunkOnFailure:
                self.result = FAILURE
            if step.haltOnFailure:
                self.result = FAILURE
                terminate = True
        elif result == WARNINGS:
            if step.warnOnWarnings:
                if self.result != FAILURE:
                    self.result = WARNINGS
            if step.flunkOnWarnings:
                self.result = FAILURE
        elif result == EXCEPTION:
            self.result = EXCEPTION
            terminate = True
        return terminate

    def lostRemote(self, remote):
        # the slave went away. There are several possible reasons for this,
        # and they aren't necessarily fatal. For now, kill the build, but
        # TODO: see if we can resume the build when it reconnects.
        log.msg("%s.lostRemote" % self)
        self.remote = None
        if self.currentStep:
            # this should cause the step to finish.
            log.msg(" stopping currentStep", self.currentStep)
            self.currentStep.interrupt(Failure(error.ConnectionLost()))

    def stopBuild(self, reason):
        # the idea here is to let the user cancel a build because, e.g.,
        # they realized they committed a bug and they don't want to waste
        # the time building something that they know will fail. Another
        # reason might be to abandon a stuck build. We want to mark the
        # build as failed quickly rather than waiting for the slave's
        # timeout to kill it on its own.

        log.msg(" %s: stopping build: %s" % (self, reason))
        if self.finished:
            return
        # TODO: include 'reason' in this point event
        self.builder.builder_status.addPointEvent(['interrupt'])
        self.currentStep.interrupt(reason)
        if 0:
            # TODO: maybe let its deferred do buildFinished
            if self.currentStep and self.currentStep.progress:
                # XXX: really .fail or something
                self.currentStep.progress.finish()
            text = ["stopped", reason]
            self.buildFinished(text, "red", FAILURE)

    def allStepsDone(self):
        if self.result == FAILURE:
            color = "red"
            text = ["failed"]
        elif self.result == WARNINGS:
            color = "orange"
            text = ["warnings"]
        else:
            color = "green"
            text = ["build", "successful"]
        text.extend(self.text)
        return self.buildFinished(text, color, self.result)

    def buildException(self, why):
        log.msg("%s.buildException" % self)
        log.err(why)
        self.buildFinished(["build", "exception"], "purple", FAILURE)

    def buildFinished(self, text, color, results):
        """This method must be called when the last Step has completed. It
        marks the Build as complete and returns the Builder to the 'idle'
        state.

        It takes three arguments which describe the overall build status:
        text, color, results. 'results' is one of SUCCESS, WARNINGS, or
        FAILURE.

        If 'results' is SUCCESS or WARNINGS, we will permit any dependant
        builds to start. If it is 'FAILURE', those builds will be
        abandoned."""

        self.finished = True
        if self.remote:
            self.remote.dontNotifyOnDisconnect(self.lostRemote)
        self.results = results

        log.msg(" %s: build finished" % self)
        self.build_status.setText(text)
        self.build_status.setColor(color)
        self.build_status.setResults(results)
        self.build_status.buildFinished()
        if self.progress:
            # XXX: also test a 'timing consistent' flag?
            log.msg(" setting expectations for next time")
            self.builder.setExpectations(self.progress)
        self.deferred.callback(self)
        self.deferred = None

    def testsFinished(self, results):
        """Accept a TestResults object."""
        self.builder.testsFinished(results)

class BuildControl(components.Adapter):
    __implements__ = interfaces.IBuildControl,

    def getStatus(self):
        return self.original.build_status

    def stopBuild(self, reason="<no reason given>"):
        self.original.stopBuild(reason)
