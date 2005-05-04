# -*- test-case-name: buildbot.test.test_status -*-

from __future__ import generators

from twisted.python import log, components
from twisted.internet import reactor, defer

import time, os.path, shutil, sys
try:
    import cPickle as pickle
except ImportError:
    import pickle

# sibling imports
from buildbot import interfaces, util

SUCCESS, WARNINGS, FAILURE, SKIPPED, EXCEPTION = range(5)
Results = ["success", "warnings", "failure", "skipped", "exception"]


# build processes call the following methods:
#
#  setDefaults
#
#  currentlyBuilding
#  currentlyIdle
#  currentlyInterlocked
#  currentlyOffline
#  currentlyWaiting
#
#  setCurrentActivity
#  updateCurrentActivity
#  addFileToCurrentActivity
#  finishCurrentActivity
#
#  startBuild
#  finishBuild

STDOUT = 0
STDERR = 1
HEADER = 2
ChunkTypes = ["stdout", "stderr", "header"]

class LogFileStub:
    __implements__ = interfaces.IStatusLogStub,

    def __init__(self, logfile):
        # stubs are created from a real LogFile
        self.step = logfile.getStep()
        self.name = logfile.getName()

    def getName(self):
        return self.name # set in BuildStepStatus.addLog

    def getStep(self):
        return self.step

    def isFinished(self):
        return True

def getFullLog(stubLog):
    logNumber = stubLog.step.getLogs().index(stubLog)
    stubBuild = stubLog.step.build
    stepNumber = stubBuild.getSteps().index(stubLog.step)
    buildNumber = stubBuild.number

    fullBuild = stubBuild.builder.getFullBuildByNumber(buildNumber)
    fullStep = fullBuild.getSteps()[stepNumber]
    fullLog = fullStep.getLogs()[logNumber]

    return fullLog

components.registerAdapter(getFullLog, LogFileStub, interfaces.IStatusLog)

class LogFile:
    __implements__ = interfaces.IStatusLog, interfaces.IStatusLogStub
    finished = False
    length = 0
    progress = None
    chunkSize = 10*1000
    runLength = 0
    runEntries = [] # provided so old pickled builds will getChunks() ok

    def __init__(self, parent):
        self.step = parent
        self.watchers = []
        self.finishedWatchers = []
        self.entries = []
        self.runEntries = []

    def getName(self):
        return self.name # set in BuildStepStatus.addLog

    def getStep(self):
        return self.step

    def isFinished(self):
        return self.finished
    def waitUntilFinished(self):
        if self.finished:
            d = defer.succeed(self)
        else:
            d = defer.Deferred()
            self.finishedWatchers.append(d)
        return d

    def getText(self):
        return "".join([e[1] for e in self.entries + self.runEntries
                        if e[0] in (STDOUT,STDERR)])
    def getTextWithHeaders(self):
        return "".join([e[1] for e in self.entries + self.runEntries])
    def getChunks(self):
        return self.entries + self.runEntries

    def subscribe(self, receiver, catchup):
        if self.finished:
            return
        self.watchers.append(receiver)
        if catchup:
            for channel, text in self.entries + self.runEntries:
                # TODO: add logChunks(), to send over everything at once?
                receiver.logChunk(self.step.build, self.step, self,
                                  channel, text)

    def unsubscribe(self, receiver):
        self.watchers.remove(receiver)

    # interface used by the build steps to add things to the log
    def logProgressTo(self, progress, name):
        self.progress = progress
        self.progressName = name

    def merge(self):
        # merge all .runEntries (which are all of the same type) into a
        # single chunk for .entries
        if not self.runEntries:
            return
        channel = self.runEntries[0][0]
        text = "".join([c[1] for c in self.runEntries])
        self.entries.append((channel, text))
        self.runEntries = []
        self.runLength = 0

    def addEntry(self, channel, text):
        assert not self.finished
        # we only add to .runEntries here. merge() is responsible for adding
        # merged chunks to .entries
        if self.runEntries and channel != self.runEntries[0][0]:
            self.merge()
        self.runEntries.append((channel, text))
        self.runLength += len(text)
        if self.runLength > self.chunkSize:
            self.merge()

        for w in self.watchers:
            w.logChunk(self.step.build, self.step, self, channel, text)
        self.length += len(text)
        if self.progress:
            self.progress.setProgress(self.progressName, self.length)

    def addStdout(self, text):
        self.addEntry(STDOUT, text)
    def addStderr(self, text):
        self.addEntry(STDERR, text)
    def addHeader(self, text):
        self.addEntry(HEADER, text)

    def finish(self):
        self.merge()
        self.finished = True
        watchers = self.finishedWatchers
        self.finishedWatchers = []
        for w in watchers:
            w.callback(self)
        if self.progress:
            self.progress.setProgress(self.progressName, self.length)
            del self.progress
            del self.progressName

    # persistence stuff
    def stubify(self):
        return LogFileStub(self)

    def __getstate__(self):
        d = self.__dict__.copy()
        del d['step'] # filled in upon unpickling
        del d['watchers']
        del d['finishedWatchers']
        if d.has_key('finished'):
            del d['finished']
        if d.has_key('progress'):
            del d['progress']
            del d['progressName']
        return d

    def __setstate__(self, d):
        self.__dict__ = d
        self.watchers = [] # probably not necessary
        self.finishedWatchers = [] # same
        # self.step must be filled in by our parent
        self.finished = True

class HTMLLogFile:
    __implements__ = interfaces.IStatusLog,

    def __init__(self, parent, html):
        self.step = parent
        self.html = html

    def getName(self):
        return self.name # set in BuildStepStatus.addLog
    def getStep(self):
        return self.step

    def isFinished(self):
        return True
    def waitUntilFinished(self):
        return defer.succeed(self)

    def getText(self):
        return self.html # looks kinda like text
    def getTextWithHeaders(self):
        return self.html
    def getChunks(self):
        return [(STDERR, self.html)]

    def subscribe(self, receiver, catchup):
        pass
    def unsubscribe(self, receiver):
        pass

    def finish(self):
        pass

    # persistence
    def stubify(self):
        return self

    def __getstate__(self):
        d = self.__dict__.copy()
        del d['step']
        return d


class Event:
    __implements__ = interfaces.IStatusEvent,

    started = None
    finished = None
    text = []
    color = None

    # IStatusEvent methods
    def getTimes(self):
        return (self.started, self.finished)
    def getText(self):
        return self.text
    def getColor(self):
        return self.color
    def getLogs(self):
        return []

    def finish(self):
        self.finished = util.now()

class TestResult:
    __implements__ = interfaces.ITestResult,

    def __init__(self, name, results, text, logs):
        assert type(name) is tuple
        self.name = name
        self.results = results
        self.text = text
        self.logs = logs

    def getName(self):
        return self.name

    def getResults(self):
        return self.results

    def getText(self):
        return self.text

    def getLogs(self):
        return self.logs


class BuildStepStatus:
    """
    I represent a collection of output status for a
    L{buildbot.process.step.BuildStep}.

    @type color: string
    @cvar color: color that this step feels best represents its
                 current mood. yellow,green,red,orange are the
                 most likely choices, although purple indicates
                 an exception
    @type progress: L{buildbot.status.progress.StepProgress}
    @cvar progress: tracks ETA for the step
    @type text: list of strings
    @cvar text: list of short texts that describe the command and its status
    @type text2: list of strings
    @cvar text2: list of short texts added to the overall build description
    @type logs: dict of string -> L{buildbot.status.builder.LogFile}
    @ivar logs: logs of steps
    """
    # note that these are created when the Build is set up, before each
    # corresponding BuildStep has started.
    __implements__ = interfaces.IBuildStepStatus, interfaces.IStatusEvent

    started = None
    finished = None
    progress = None
    text = []
    color = None
    results = (None, [])
    text2 = []
    watchers = []
    updates = {}
    finishedWatchers = []

    def __init__(self, parent):
        self.build = parent
        self.logs = []
        self.watchers = []
        self.updates = {}
        self.finishedWatchers = []

    def getName(self):
        """Returns a short string with the name of this step. This string
        may have spaces in it."""
        return self.name

    def getBuild(self):
        return self.build

    def getTimes(self):
        return (self.started, self.finished)

    def getExpectations(self):
        """Returns a list of tuples (name, current, target)."""
        if not self.progress:
            return []
        ret = []
        metrics = self.progress.progress.keys()
        metrics.sort()
        for m in metrics:
            t = (m, self.progress.progress[m], self.progress.expectations[m])
            ret.append(t)
        return ret

    def getLogs(self):
        return self.logs


    def isFinished(self):
        return (self.finished is not None)

    def waitUntilFinished(self):
        if self.finished:
            d = defer.succeed(self)
        else:
            d = defer.Deferred()
            self.finishedWatchers.append(d)
        return d

    # while the step is running, the following methods make sense.
    # Afterwards they return None

    def getETA(self):
        if self.started is None:
            return None # not started yet
        if self.finished is not None:
            return None # already finished
        if not self.progress:
            return None # no way to predict
        return self.progress.remaining()

    # Once you know the step has finished, the following methods are legal.
    # Before this step has finished, they all return None.

    def getText(self):
        """Returns a list of strings which describe the step. These are
        intended to be displayed in a narrow column. If more space is
        available, the caller should join them together with spaces before
        presenting them to the user."""
        return self.text

    def getColor(self):
        """Returns a single string with the color that should be used to
        display this step. 'green', 'orange', 'red', 'yellow' and 'purple'
        are the most likely ones."""
        return self.color

    def getResults(self):
        """Return a tuple describing the results of the step.
        'result' is one of the constants in L{buildbot.status.builder}:
        SUCCESS, WARNINGS, FAILURE, or SKIPPED.
        'strings' is an optional list of strings that the step wants to
        append to the overall build's results. These strings are usually
        more terse than the ones returned by getText(): in particular,
        successful Steps do not usually contribute any text to the
        overall build.

        @rtype:   tuple of int, list of strings
        @returns: (result, strings)
        """
        return (self.results, self.text2)

    # subscription interface

    def subscribe(self, receiver, updateInterval=10):
        # will get logStarted, logFinished, stepETAUpdate
        assert receiver not in self.watchers
        self.watchers.append(receiver)
        self.sendETAUpdate(receiver, updateInterval)

    def sendETAUpdate(self, receiver, updateInterval):
        self.updates[receiver] = None
        # they might unsubscribe during stepETAUpdate
        receiver.stepETAUpdate(self.build, self,
                           self.getETA(), self.getExpectations())
        if receiver in self.watchers:
            self.updates[receiver] = reactor.callLater(updateInterval,
                                                       self.sendETAUpdate,
                                                       receiver,
                                                       updateInterval)

    def unsubscribe(self, receiver):
        self.watchers.remove(receiver)
        if self.updates[receiver] is not None:
            self.updates[receiver].cancel()
            del self.updates[receiver]


    # methods to be invoked by the BuildStep

    def setName(self, stepname):
        self.name = stepname

    def setProgress(self, stepprogress):
        self.progress = stepprogress

    def stepStarted(self):
        self.started = util.now()
        if self.build:
            self.build.stepStarted(self)

    def addLog(self, name):
        assert self.started # addLog before stepStarted won't notify watchers
        log = LogFile(self)
        log.name = name
        self.logs.append(log)
        for w in self.watchers:
            receiver = w.logStarted(self.build, self, log)
            if receiver:
                log.subscribe(receiver, True)
        d = log.waitUntilFinished()
        d.addCallback(self.logFinished)
        return log

    def addHTMLLog(self, name, html):
        assert self.started # addLog before stepStarted won't notify watchers
        log = HTMLLogFile(self, html)
        log.name = name
        self.logs.append(log)
        for w in self.watchers:
            receiver = w.logStarted(self.build, self, log)
            # TODO: think about this: there isn't much point in letting
            # them subscribe
            #if receiver:
            #    log.subscribe(receiver, True)
            w.logFinished(self.build, self, log)

    def logFinished(self, log):
        for w in self.watchers:
            w.logFinished(self.build, self, log)

    def setColor(self, color):
        self.color = color
    def setText(self, text):
        self.text = text
    def setText2(self, text):
        self.text2 = text

    def stepFinished(self, results):
        self.finished = util.now()
        self.results = results
        for loog in self.logs:
            if not loog.isFinished():
                log.msg("log '%s' not closed" % loog.getName())
                loog.finish()

        for r in self.updates.keys():
            if self.updates[r] is not None:
                self.updates[r].cancel()
                del self.updates[r]

        watchers = self.finishedWatchers
        self.finishedWatchers = []
        for w in watchers:
            w.callback(self)

    # methods to be invoked by our parent IBuildStatus

    def pruneLogs(self):
        self.logs = []

    # persistence

    def stubify(self):
        self.logs = [loog.stubify() for loog in self.logs]
        return self

    def __getstate__(self):
        d = self.__dict__.copy()
        del d['build'] # filled in when loading
        if d.has_key('progress'):
            del d['progress']
        del d['watchers']
        del d['finishedWatchers']
        del d['updates']
        return d

    def __setstate__(self, d):
        self.__dict__ = d
        # self.build must be filled in by our parent
        for loog in self.logs:
            loog.step = self


class BuildStatus:
    __implements__ = interfaces.IBuildStatus, interfaces.IStatusEvent

    sourceStamp = None
    reason = None
    changes = []
    blamelist = []
    progress = None
    started = None
    finished = None
    currentStep = None
    text = []
    color = None
    results = None

    # these lists/dicts are defined here so that unserialized instances have
    # (empty) values. They are set in __init__ to new objects to make sure
    # each instance gets its own copy.
    watchers = []
    updates = {}
    finishedWatchers = []
    testResults = {}

    def __init__(self, parent, number):
        """
        @type  parent: L{BuilderStatus}
        @type  number: int
        """
        self.builder = parent
        self.number = number
        self.watchers = []
        self.updates = {}
        self.finishedWatchers = []
        self.steps = []
        self.testResults = {}

    # IBuildStatus

    def getBuilder(self):
        """
        @rtype: L{BuilderStatus}
        """
        return self.builder

    def getNumber(self):
        return self.number

    def getPreviousBuild(self):
        if self.number == 0:
            return None
        return self.builder.getBuild(self.number-1)

    def getSourceStamp(self):
        return self.sourceStamp

    def getReason(self):
        return self.reason

    def getChanges(self):
        return self.changes

    def getResponsibleUsers(self):
        return self.blamelist

    def getInterestedUsers(self):
        # TODO: the Builder should add others: sheriffs, domain-owners
        return self.blamelist

    def getSteps(self):
        """Return a list of IBuildStepStatus objects. For invariant builds
        (those which always use the same set of Steps), this should be the
        complete list, however some of the steps may not have started yet
        (step.getTimes()[0] will be None). For variant builds, this may not
        be complete (asking again later may give you more of them)."""
        return self.steps

    def getTimes(self):
        return (self.started, self.finished)

    def isFinished(self):
        return (self.finished is not None)

    def waitUntilFinished(self):
        if self.finished:
            d = defer.succeed(self)
        else:
            d = defer.Deferred()
            self.finishedWatchers.append(d)
        return d

    # while the build is running, the following methods make sense.
    # Afterwards they return None

    def getETA(self):
        if self.finished is not None:
            return None
        if not self.progress:
            return None
        eta = self.progress.eta()
        if eta is None:
            return None
        return eta - util.now()

    def getCurrentStep(self):
        return self.currentStep

    # Once you know the build has finished, the following methods are legal.
    # Before ths build has finished, they all return None.

    def getText(self):
        text = []
        text.extend(self.text)
        for s in self.steps:
            text.extend(s.text2)
        return text

    def getColor(self):
        return self.color

    def getResults(self):
        return self.results

    def getTestResults(self):
        return self.testResults

    def getLogs(self):
        # TODO: steps should contribute significant logs instead of this
        # hack, which returns every log from every step. The logs should get
        # names like "compile" and "test" instead of "compile.output"
        logs = []
        for s in self.steps:
            for log in s.getLogs():
                logs.append(log)
        return logs

    # subscription interface

    def subscribe(self, receiver, updateInterval=None):
        # will receive stepStarted and stepFinished messages
        # and maybe buildETAUpdate
        self.watchers.append(receiver)
        if updateInterval is not None:
            self.sendETAUpdate(receiver, updateInterval)

    def sendETAUpdate(self, receiver, updateInterval):
        self.updates[receiver] = None
        ETA = self.getETA()
        if ETA is not None:
            receiver.buildETAUpdate(self, self.getETA())
        # they might have unsubscribed during buildETAUpdate
        if receiver in self.watchers:
            self.updates[receiver] = reactor.callLater(updateInterval,
                                                       self.sendETAUpdate,
                                                       receiver,
                                                       updateInterval)

    def unsubscribe(self, receiver):
        self.watchers.remove(receiver)
        if self.updates[receiver] is not None:
            self.updates[receiver].cancel()
            del self.updates[receiver]

    # methods for the base.Build to invoke

    def addStep(self, step):
        """The Build is setting up, and has added a new BuildStep to its
        list. The BuildStep object is ready for static queries (everything
        except ETA). Give it a BuildStepStatus object to which it can send
        status updates."""

        s = BuildStepStatus(self)
        s.setName(step.name)
        step.step_status = s
        self.steps.append(s)

    def addTestResult(self, result):
        self.testResults[result.getName()] = result

    def setSourceStamp(self, revision, patch=None):
        self.sourceStamp = (revision, patch)
    def setReason(self, reason):
        self.reason = reason
    def setChanges(self, changes):
        self.changes = changes
    def setBlamelist(self, blamelist):
        self.blamelist = blamelist
    def setProgress(self, progress):
        self.progress = progress

    def buildStarted(self, build):
        """The Build has been set up and is about to be started. It can now
        be safely queried, so it is time to announce the new build."""

        self.started = util.now()
        # now that we're ready to report status, let the BuilderStatus tell
        # the world about us
        self.builder.buildStarted(self)

    def setText(self, text):
        assert type(text) in (list, tuple)
        self.text = text
    def setColor(self, color):
        self.color = color
    def setResults(self, results):
        self.results = results

    def buildFinished(self):
        self.currentStep = None
        self.finished = util.now()

        for r in self.updates.keys():
            if self.updates[r] is not None:
                self.updates[r].cancel()
                del self.updates[r]

        watchers = self.finishedWatchers
        self.finishedWatchers = []
        for w in watchers:
            w.callback(self)

    # methods called by our BuildStepStatus children

    def stepStarted(self, step):
        self.currentStep = step
        name = self.getBuilder().getName()
        for w in self.watchers:
            receiver = w.stepStarted(self, step)
            if receiver:
                if type(receiver) == type(()):
                    step.subscribe(receiver[0], receiver[1])
                else:
                    step.subscribe(receiver)
        step.waitUntilFinished().addCallback(self._stepFinished)

    def _stepFinished(self, step):
        results = step.getResults()
        for w in self.watchers:
            w.stepFinished(self, step, results)

    # methods called by our BuilderStatus parent

    def pruneLogs(self):
        # this build is somewhat old: remove the build logs to save space
        # TODO: delete logs visible through IBuildStatus.getLogs
        for s in self.steps:
            s.pruneLogs()

    def pruneSteps(self):
        # this build is very old: remove the build steps too
        self.steps = []

    # persistence stuff

    def stubify(self):
        self.steps = [step.stubify() for step in self.steps]
        return self

    def __getstate__(self):
        d = self.__dict__.copy()
        # for now, a serialized Build is always "finished". We will never
        # save unfinished builds.
        if not self.finished:
            d['finished'] = True
            # TODO: push an "interrupted" step so it is clear that the build
            # was interrupted. The builder will have a 'shutdown' event, but
            # someone looking at just this build will be confused as to why
            # the last log is truncated.
        del d['builder'] # filled in by our parent when loading
        del d['watchers']
        del d['updates']
        del d['finishedWatchers']
        return d

    def __setstate__(self, d):
        self.__dict__ = d
        # self.builder must be filled in by our parent when loading
        for step in self.steps:
            step.build = self
        self.watchers = []
        self.updates = {}
        self.finishedWatchers = []

    def saveYourself(self):
        filename = os.path.join(self.builder.basedir, "%d" % self.number)
        if os.path.isdir(filename):
            # leftover from 0.5.0, which stored builds in directories
            shutils.rmtree(filename, ignore_errors=True)
        tmpfilename = filename + ".tmp"
        try:
            pickle.dump(self, open(tmpfilename, "w"), -1)
            if sys.platform == 'win32':
                # windows cannot rename a file on top of an existing one, so
                # fall back to delete-first. There are ways this can fail and
                # lose the builder's history, so we avoid using it in the
                # general (non-windows) case
                if os.path.exists(filename):
                    os.unlink(filename)
            os.rename(tmpfilename, filename)
        except:
            log.msg("unable to save build %s-#%d" % (self.builder.name,
                                                     self.number))
            log.err()



class BuilderStatus:
    """I handle status information for a single process.base.Builder object.
    That object sends status changes to me (frequently as Events), and I
    provide them on demand to the various status recipients, like the HTML
    waterfall display and the live status clients. It also sends build
    summaries to me, which I log and provide to status clients who aren't
    interested in seeing details of the individual build steps.

    I am responsible for maintaining the list of historic Events and Builds,
    pruning old ones, and loading them from / saving them to disk.

    I live in the buildbot.process.base.Builder object, in the .statusbag
    attribute.

    @type  category: string
    @ivar  category: user-defined category this builder belongs to; can be
                     used to filter on in status clients
    """

    __implements__ = interfaces.IBuilderStatus,

    buildHorizon = 100 # prune builds beyond this
    stubBuildCacheSize = 30
    fullBuildCacheSize = 2
    stepHorizon = 50 # prune steps in builds beyond this
    logHorizon = 20 # prune logs in builds beyond this
    slavename = None
    category = None
    currentBuild = None
    currentBigState = "offline" # or idle/waiting/interlocked/building
    ETA = None
    nextBuildNumber = 0
    basedir = None # filled in by our parent

    def __init__(self, buildername, category=None):
        self.name = buildername
        self.category = category

        self.events = []
        # these three hold Events, and are used to retrieve the current
        # state of the boxes.
        self.lastBuildStatus = None
        #self.currentBig = None
        #self.currentSmall = None
        self.nextBuild = None
        self.eta = None
        self.watchers = []
        self.fullBuildCache = [] # TODO: age builds out of the cache
        self.stubBuildCache = []

    # persistence

    def __getstate__(self):
        d = self.__dict__.copy()
        d['watchers'] = []
        del d['fullBuildCache']
        del d['stubBuildCache']
        if self.currentBuild:
            self.currentBuild.saveYourself()
            # TODO: push a 'hey, build was interrupted' event
            del d['currentBuild']
        del d['currentBigState']
        del d['basedir']
        del d['status']
        return d

    def __setstate__(self, d):
        self.__dict__ = d
        self.fullBuildCache = []
        self.stubBuildCache = []
        self.watchers = []
        # self.basedir must be filled in by our parent
        # self.status must be filled in by our parent

    def saveYourself(self):
        for b in self.fullBuildCache:
            if not b.isFinished:
                # interrupted build, need to save it anyway.
                # BuildStatus.saveYourself will mark it as interrupted.
                b.saveYourself()
        filename = os.path.join(self.basedir, "builder")
        tmpfilename = filename + ".tmp"
        try:
            pickle.dump(self, open(tmpfilename, "w"), -1)
            if sys.platform == 'win32':
                # windows cannot rename a file on top of an existing one
                if os.path.exists(filename):
                    os.unlink(filename)
            os.rename(tmpfilename, filename)
        except:
            log.msg("unable to save builder %s" % self.name)
            log.err()
        

    # build cache management

    def addFullBuildToCache(self, build):
        if build in self.fullBuildCache:
            return
        self.fullBuildCache.append(build)
        # there might be a stripped version of this one in the stub cache
        for b in self.stubBuildCache:
            if b.number == build.number:
                self.stubBuildCache.remove(b)
                break
        while len(self.fullBuildCache) > self.fullBuildCacheSize:
            old = self.fullBuildCache.pop(0)
            if not old.finished:
                log.msg("BuilderStatus.addFullBuildToCache: "
                        "weird, old build %d isn't finished" % old.number)
                # this can happen if a lot of people hit logfiles of
                # different builds at the same time: all the corresponding
                # builds must be pulled fully into memory, and a
                # currently-active one could be pushed out. We need to keep
                # it from being stripped until it finishes changing its logs
                self.fullBuildCache.append(old)
                break
            else:
                old = old.stubify()
                self.addStubBuildToCache(old)

    def addStubBuildToCache(self, build):
        if build in self.stubBuildCache:
            return
        self.stubBuildCache.append(build)
        if len(self.stubBuildCache) > self.stubBuildCacheSize:
            self.stubBuildCache.pop(0)

    def getStubBuildByNumber(self, number):
        for build in self.fullBuildCache:
            if build.number == number:
                # full builds are ok too
                return build
        for build in self.stubBuildCache:
            if build.number == number:
                return build
        # need to load it from disk
        filename = os.path.join(self.basedir, "%d" % number)
        # TODO: consider using "%d.stub" to avoid loading in all the build's
        # logs when we're going to throw them out in a moment. requires two
        # separate files on disk, though.
        try:
            build = pickle.load(open(filename, "r"))
            build.builder = self
            build = build.stubify()
            self.addStubBuildToCache(build)
            return build
        except IOError:
            raise IndexError("no such build %d" % number)
        except EOFError:
            raise IndexError("corrupted build pickle %d" % number)

    def getFullBuildByNumber(self, number):
        for build in self.fullBuildCache:
            if build.number == number:
                return build
        filename = os.path.join(self.basedir, "%d" % number)
        try:
            build = pickle.load(open(filename, "r"))
            build.builder = self
            self.addFullBuildToCache(build)
            return build
        except IOError:
            raise IndexError("no such build %d" % number)
        except EOFError:
            raise IndexError("corrupted build pickle %d" % number)

    def prune(self):
        return # TODO: change this to walk through the filesystem
        # first, blow away all builds beyond our build horizon
        self.builds = self.builds[-self.buildHorizon:]
        # then prune steps in builds past the step horizon
        for b in self.builds[0:-self.stepHorizon]:
            b.pruneSteps()
        # then prune logs in steps in builds past the log horizon
        for b in self.builds[0:-self.logHorizon]:
            b.pruneLogs()

    def getETA(self):
        eta = self.ETA # absolute time, set by currentlyWaiting
        state = self.currentBigState
        if state == "waiting":
            eta = self.ETA - util.now()
        elif state == "building":
            eta = self.currentBuild.getETA()
        return eta

    # IBuilderStatus methods
    def getName(self):
        return self.name

    def getState(self):
        return (self.currentBigState, self.getETA(), self.currentBuild)

    def getSlave(self):
        return self.status.getSlave(self.slavename)

    def getCurrentBuild(self):
        return self.currentBuild

    def getLastFinishedBuild(self):
        b = self.getBuild(-1)
        if not (b and b.isFinished()):
            b = self.getBuild(-2)
        return b

    def getBuild(self, number):
        if number < 0:
            number = self.nextBuildNumber + number
        if number < 0 or number >= self.nextBuildNumber:
            return None

        try:
            return self.getStubBuildByNumber(number)
        except IndexError:
            return None

    def getEvent(self, number):
        try:
            return self.events[number]
        except IndexError:
            return None

    def eventGenerator(self):
        """This function creates a generator which will provide all of this
        Builder's status events, starting with the most recent and
        progressing backwards in time. """

        # remember the oldest-to-earliest flow here. "next" means earlier.

        # TODO: interleave build steps and self.events by timestamp

        eventIndex = -1
        e = self.getEvent(eventIndex)
        for Nb in range(1, self.nextBuildNumber+1):
            b = self.getBuild(-Nb)
            if not b:
                break
            steps = b.getSteps()
            for Ns in range(1, len(steps)+1):
                if steps[-Ns].started:
                    step_start = steps[-Ns].getTimes()[0]
                    while e is not None and e.getTimes()[0] > step_start:
                        yield e
                        eventIndex -= 1
                        e = self.getEvent(eventIndex)
                    yield steps[-Ns]
            yield b
        while e is not None:
            yield e
            eventIndex -= 1
            e = self.getEvent(eventIndex)

    def subscribe(self, receiver):
        # will get builderChangedState, buildStarted, and buildFinished
        self.watchers.append(receiver)
        self.publishState(receiver)

    def unsubscribe(self, receiver):
        self.watchers.remove(receiver)

    ## Builder interface (methods called by the Builder which feeds us)

    def setSlavename(self, name):
        self.slavename = name

    def addEvent(self, text=[], color=None):
        # this adds a duration event. When it is done, the user should call
        # e.finish(). They can also mangle it by modifying .text and .color
        e = Event()
        e.started = util.now()
        e.text = text
        e.color = color
        self.events.append(e)
        return e # they are free to mangle it further

    def addPointEvent(self, text=[], color=None):
        # this adds a point event, one which occurs as a single atomic
        # instant of time.
        e = Event()
        e.started = util.now()
        e.finished = 0
        e.text = text
        e.color = color
        self.events.append(e)
        return e # for consistency, but they really shouldn't touch it

    def currentlyOffline(self):
        log.msg("currentlyOffline")
        self.currentBigState = "offline"
        self.publishState()

    def currentlyIdle(self):
        self.currentBigState = "idle"
        self.ETA = None
        self.currentBuild = None
        self.publishState()

    def currentlyWaiting(self, when):
        self.currentBigState = "waiting"
        self.ETA = when
        self.currentBuild = None
        self.publishState()

    def currentlyInterlocked(self, interlocks):
        self.currentBigState = "interlocked"
        self.ETA = None
        self.currentBuild = None
        #names = [interlock.name for interlock in interlocks]
        #self.currentBig = Event(color="yellow",
        #                        text=["interlocked"] + names)
        self.publishState()

    def buildETAText(self, text):
        # UNUSED, should live in the clients
        if self.eta:
            done = self.eta.eta()
            if done != None:
                text += [time.strftime("ETA: %H:%M:%S", time.localtime(done)),
                         "[%d seconds]" % (done - util.now())]
            else:
                text += ["ETA: ?"]
        
    def NOTcurrentlyBuilding(self, build, eta):
        # eta is a progress.BuildProgress object
        self.currentBigState = "building"
        self.currentBuild = build
        if eta:
            self.ETA = eta.eta()
        else:
            self.ETA = None
        self.publishState()

    def publishState(self, target=None):
        state = self.currentBigState
        eta = self.getETA()

        if target is not None:
            # unicast
            target.builderChangedState(self.name, state, eta)
            return
        for w in self.watchers:
            w.builderChangedState(self.name, state, eta)

    def newBuild(self):
        """The Builder has decided to start a build, but the Build object is
        not yet ready to report status (it has not finished creating the
        Steps). Create a BuildStatus object that it can use."""
        number = self.nextBuildNumber
        self.nextBuildNumber += 1
        s = BuildStatus(self, number)
        s.waitUntilFinished().addCallback(self._buildFinished)
        return s

    # buildStarted is called by our child BuildStatus instances
    def buildStarted(self, s):
        """Now the BuildStatus object is ready to go (it knows all of its
        Steps, its ETA, etc), so it is safe to notify our watchers."""

        assert s.builder is self # paranoia
        assert s.number == self.nextBuildNumber - 1
        self.currentBuild = s
        self.currentBigState = "building"
        self.addFullBuildToCache(self.currentBuild)
        self.publishState()

        # now that the BuildStatus is prepared to answer queries, we can
        # announce the new build to all our watchers

        for w in self.watchers: # TODO: maybe do this later? callLater(0)?
            receiver = w.buildStarted(self.getName(), s)
            if receiver:
                if type(receiver) == type(()):
                    s.subscribe(receiver[0], receiver[1])
                else:
                    s.subscribe(receiver)

    def _buildFinished(self, s):
        assert s is self.currentBuild
        self.currentBigState = "idle"
        self.ETA = None
        self.currentBuild.saveYourself()
        self.currentBuild = None

        name = self.getName()
        results = s.getResults()
        for w in self.watchers:
            w.buildFinished(name, s, results)

        self.prune() # conserve disk


    # waterfall display (history)
    
    # top-row: last-build status 
    def setLastBuildStatus(self, event):
        log.msg("setLastBuildStatus", event)
        self.lastBuildStatus = event
        for w in self.watchers:
            self.sendLastBuildStatus(w)

    # I want some kind of build event that holds everything about the build:
    # why, what changes went into it, the results of the build, itemized
    # test results, etc. But, I do kind of need something to be inserted in
    # the event log first, because intermixing step events and the larger
    # build event is fraught with peril. Maybe an Event-like-thing that
    # doesn't have a file in it but does have links. Hmm, that's exactly
    # what it does now. The only difference would be that this event isn't
    # pushed to the clients.

    # publish to clients
    def sendLastBuildStatus(self, client):
        #client.newLastBuildStatus(self.lastBuildStatus)
        pass
    def sendCurrentActivityBigToEveryone(self):
        for s in self.subscribers:
            self.sendCurrentActivityBig(s)
    def sendCurrentActivityBig(self, client):
        state = self.currentBigState
        if state == "offline":
            client.currentlyOffline()
        elif state == "idle":
            client.currentlyIdle()
        elif state == "waiting":
            client.currentlyWaiting(self.nextBuild - util.now())
        elif state == "interlocked":
            client.currentlyInterlocked()
        elif state == "building":
            client.currentlyBuilding(self.eta)
            # let them format the time as they wish
        else:
            log.msg("Hey, self.currentBigState is weird:", state)
            

    # current-activity-small
    def OFFsetCurrentActivity(self, event):
        self.pushEvent(event)
        self.currentSmall = event
        for s in self.subscribers:
            s.newEvent(event)

    def OFFpushEvent(self, event):
        if self.events:
            next = self.events[-1].number + 1
        else:
            next = 0
        event.setName(self, next)
        self.events.append(event)
        self.pruneEvents()


    def OFFupdateCurrentActivity(self, **kwargs):
        self.currentSmall.update(**kwargs)
    def OFFaddFileToCurrentActivity(self, name, logfile):
        self.currentSmall.addFile(name, logfile)
    def OFFfinishCurrentActivity(self):
        self.currentSmall.finish()

    def setCurrentBuild(self):
        pass
    def finishCurrentBuild(self):
        pass
    
    ## HTML display interface
    def getLastBuildStatus(self):
        return self.lastBuildStatus
    def getCurrentBig(self):
        state = self.currentBigState
        if state == "waiting":
            when = self.nextBuild
            return Event(color="yellow",
                         text=["waiting", "next build",
                               time.strftime("%H:%M:%S",
                                             time.localtime(when)),
                               "[%d seconds]" % (when - util.now())
                               ])
        elif state == "building":
            text = ["building"]
            self.buildETAText(text)
            return Event(color="yellow", text=text)
        else:
            return self.currentBig
    def getCurrentSmall(self):
        return self.currentSmall

    def getEventNumbered(self, num):
        # deal with dropped events, pruned events
        first = self.events[0].number
        if first + len(self.events)-1 != self.events[-1].number:
            log.msg(self,
                    "lost an event somewhere: [0] is %d, [%d] is %d" % \
                    (self.events[0].number,
                     len(self.events) - 1,
                     self.events[-1].number))
            for e in self.events:
                log.msg("e[%d]: " % e.number, e)
            return None
        offset = num - first
        log.msg(self, "offset", offset)
        try:
            return self.events[offset]
        except IndexError:
            return None

    ## Persistence of Status
    def loadYourOldEvents(self):
        if hasattr(self, "allEvents"):
            # first time, nothing to get from file. Note that this is only if
            # the Application gets .run() . If it gets .save()'ed, then the
            # .allEvents attribute goes away in the initial __getstate__ and
            # we try to load a non-existent file.
            return
        self.allEvents = self.loadFile("events", [])
        if self.allEvents:
            self.nextEventNumber = self.allEvents[-1].number + 1
        else:
            self.nextEventNumber = 0
    def saveYourOldEvents(self):
        self.saveFile("events", self.allEvents)

    ## clients

    def addClient(self, client):
        if client not in self.subscribers:
            self.subscribers.append(client)
            self.sendLastBuildStatus(client)
            self.sendCurrentActivityBig(client)
            client.newEvent(self.currentSmall)
    def removeClient(self, client):
        if client in self.subscribers:
            self.subscribers.remove(client)

class SlaveStatus:
    __implements__ = interfaces.ISlaveStatus,

    admin = None
    host = None
    connected = False

    def __init__(self, name):
        self.name = name

    def getName(self):
        return self.name
    def getAdmin(self):
        return self.admin
    def getHost(self):
        return self.host
    def isConnected(self):
        return self.connected

class Status:
    """
    I represent the status of the buildmaster.
    """
    __implements__ = interfaces.IStatus,

    def __init__(self, botmaster, basedir):
        """
        @type  botmaster: L{buildbot.master.BotMaster}
        @param botmaster: the Status object uses C{.botmaster} to get at
                          both the L{buildbot.master.BuildMaster} (for
                          various buildbot-wide parameters) and the
                          actual Builders (to get at their L{BuilderStatus}
                          objects). It is not allowed to change or influence
                          anything through this reference.
        @type  basedir: string
        @param basedir: this provides a base directory in which saved status
                        information (changes.pck, saved Build status
                        pickles) can be stored
        """
        self.botmaster = botmaster
        self.basedir = basedir
        self.watchers = []
        assert os.path.isdir(basedir)

    def getProjectName(self):
        return self.botmaster.parent.projectName
    def getProjectURL(self):
        return self.botmaster.parent.projectURL
    def getBuildbotURL(self):
        return self.botmaster.parent.buildbotURL

    def getBuilderNames(self, categories=None):
        if categories == None:
            return self.botmaster.builderNames[:] # don't let them break it
        
        l = []
        # respect addition order
        for name in self.botmaster.builderNames:
            builder = self.botmaster.builders[name]
            if builder.builder_status.category in categories:
                l.append(name)
        return l

    def getBuilder(self, name):
        """
        @rtype: L{BuilderStatus}
        """
        return self.botmaster.builders[name].builder_status

    def getSlave(self, slavename):
        return self.botmaster.slaves[slavename].slave_status

    def subscribe(self, target):
        self.watchers.append(target)
        for name in self.botmaster.builderNames:
            self.announceNewBuilder(target, name, self.getBuilder(name))
    def unsubscribe(self, target):
        self.watchers.remove(target)

    def announceNewBuilder(self, target, name, builder_status):
        t = target.builderAdded(name, builder_status)
        if t:
            builder_status.subscribe(t)

    def builderAdded(self, name, basedir, category=None):
        """
        @rtype: L{BuilderStatus}
        """
        filename = os.path.join(self.basedir, basedir, "builder")
        log.msg("trying to load status pickle from %s" % filename)
        builder_status = None
        try:
            builder_status = pickle.load(open(filename, "r"))
        except IOError:
            log.msg("no saved status pickle, creating a new one")
        except:
            log.msg("error while loading status pickle, creating a new one")
        if not builder_status:
            builder_status = BuilderStatus(name, category)
            builder_status.addPointEvent(["builder", "created"])
        log.msg("added builder %s in category %s" % (name, category))
        # an unpickled object might not have category set from before,
        # so set it here to make sure
        builder_status.category = category
        builder_status.basedir = os.path.join(self.basedir, basedir)
        builder_status.name = name # it might have been updated
        builder_status.status = self

        if not os.path.isdir(builder_status.basedir):
            os.mkdir(builder_status.basedir)
        builder_status.currentlyOffline()

        for t in self.watchers:
            self.announceNewBuilder(t, name, builder_status)

        return builder_status

    def builderRemoved(self, name):
        for t in self.watchers:
            t.builderRemoved(name)

    def prune(self):
        for b in self.botmaster.builders.values():
            b.builder_status.prune()
