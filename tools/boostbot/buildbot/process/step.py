#! /usr/bin/python

import time, random, types, re, warnings
from email.Utils import formatdate

from twisted.internet import reactor, defer, error
from twisted.spread import pb
from twisted.python import log
from twisted.python.failure import Failure
from twisted.web.util import formatFailure

from buildbot.util import now
from buildbot.status import progress, builder
from buildbot.status.builder import SUCCESS, WARNINGS, FAILURE, SKIPPED

class RemoteCommand(pb.Referenceable):

    """This base class represents a single command to be run on the slave. It
    handles the details of reliably gathering status updates from the slave
    (acknowledging each), and (eventually) recovering from interrupted
    builds.

    The command should be started by calling .run(), which returns a Deferred
    that will fire when the command has finished, or will errback if an
    exception is raised. Typically __init__ or run() will set up
    self.remote_command to be a string which corresponds to one of the
    SlaveCommands registered in the buildslave, and self.args to a dictionary
    of arguments that will be passed to the SlaveCommand instance.

    start, remoteUpdate, and remoteComplete are available to be overridden

    """

    # this counter provides a unique value for each RemoteCommand executed
    # across all slaves. It is a class attribute.
    commandCounter = [0]
    active = False

    def __init__(self, remote_command, args):
        self.remote_command = remote_command
        self.args = args

    def __getstate__(self):
        dict = self.__dict__.copy()
        # Remove the remote ref: if necessary (only for resumed builds), it
        # will be reattached at resume time
        if dict.has_key("remote"):
            del dict["remote"]
        return dict

    def run(self, step, remote):
        self.active = True
        self.step = step
        self.remote = remote
        c = self.commandCounter[0]
        self.commandCounter[0] += 1
        #self.commandID = "%d %d" % (c, random.randint(0, 1000000))
        self.commandID = "%d" % c
        log.msg("%s: RemoteCommand.run [%s]" % (self, self.commandID))
        self.deferred = defer.Deferred()

        d = defer.maybeDeferred(self.start)

        # _finished is called with an error for unknown commands, errors
        # that occur while the command is starting (including OSErrors in
        # exec()), StaleBroker (when the connection was lost before we
        # started), and pb.PBConnectionLost (when the slave isn't responding
        # over this connection, perhaps it had a power failure, or NAT
        # weirdness). If this happens, self.deferred is fired right away.
        d.addErrback(self._finished)

        # Connections which are lost while the command is running are caught
        # when our parent Step calls our .lostRemote() method.
        return self.deferred

    def start(self):
        # We will receive remote_update messages as the command runs.
        # We will get a single remote_complete when it finishes.
        # We should fire self.deferred when the command is done.
        d = self.remote.callRemote("startCommand", self, self.commandID,
                                   self.remote_command, self.args)
        return d

    def interrupt(self, why):
        # TODO: consider separating this into interrupt() and stop(), where
        # stop() unconditionally calls _finished, but interrupt() merely
        # asks politely for the command to stop soon.

        log.msg("RemoteCommand.interrupt", self, why)
        if not self.active:
            log.msg(" but this RemoteCommand is already inactive")
            return
        if not self.remote:
            log.msg(" but our .remote went away")
            return
        if isinstance(why, Failure) and why.check(error.ConnectionLost):
            log.msg("RemoteCommand.disconnect: lost slave")
            self.remote = None
            self._finished(why)
            return

        # tell the remote command to halt. Returns a Deferred that will fire
        # when the interrupt command has been delivered.
        
        d = defer.maybeDeferred(self.remote.callRemote, "interruptCommand",
                                self.commandID, str(why))
        # the slave may not have remote_interruptCommand
        d.addErrback(self._interruptFailed)
        return d

    def _interruptFailed(self, why):
        log.msg("RemoteCommand._interruptFailed", self)
        # TODO: forcibly stop the Command now, since we can't stop it
        # cleanly
        return None

    def remote_update(self, updates):
        max_updatenum = 0
        for (update, num) in updates:
            #log.msg("update[%d]:" % num)
            try:
                if self.active: # ignore late updates
                    self.remoteUpdate(update)
            except:
                # log failure, terminate build, let slave retire the update
                self._finished(Failure())
                # TODO: what if multiple updates arrive? should
                # skip the rest but ack them all
            if num > max_updatenum:
                max_updatenum = num
        return max_updatenum

    def remoteUpdate(self, update):
        raise NotImplementedError("You must implement this in a subclass")

    def remote_complete(self, failure=None):
        # call the real remoteComplete a moment later, but first return an
        # acknowledgement so the slave can retire the completion message.
        if self.active:
            reactor.callLater(0, self._finished, failure)
        return None

    def _finished(self, failure=None):
        self.active = False
        # call .remoteComplete. If it raises an exception, or returns the
        # Failure that we gave it, our self.deferred will be errbacked. If
        # it does not (either it ate the Failure or there the step finished
        # normally and it didn't raise a new exception), self.deferred will
        # be callbacked.
        d = defer.maybeDeferred(self.remoteComplete, failure)
        # arrange for the callback to get this RemoteCommand instance
        # instead of just None
        d.addCallback(lambda r: self)
        d.addBoth(self.deferred.callback)

    def remoteComplete(self, maybeFailure):
        """Subclasses can override this.

        This is called when the RemoteCommand has finished. 'maybeFailure'
        will be None if the command completed normally, or a Failure
        instance in one of the following situations:

        #  the slave was lost before the command was started
        #  the slave didn't respond to the startCommand message
        #  the slave raised an exception while starting the command
        #   (bad command name, bad args, OSError from missing executable)
        #  the slave raised an exception while finishing the command
        #   (they send back a remote_complete message with a Failure payload)
        # and also (for now):
        #  slave disconnected while the command was running
        
        This method should do cleanup, like closing log files. It should
        normally return the 'failure' argument, so that any exceptions will
        be propagated to the Step. If it wants to consume them, return None
        instead."""

        return failure

class LoggedRemoteCommand(RemoteCommand):
    """This is a RemoteCommand which expects the slave to send back
    stdout/stderr/rc updates. It gathers these updates into a
    builder.LogFile named self.log . You can give it a LogFile to use by
    calling useLog(), or it will create its own when the command is started.
    Unless you tell it otherwise, it will close the log when the command is
    complete.

    The constructor takes an 'args' parameter which is a dictionary of
    arguments to send to the remote command."""

    log = None
    closeWhenFinished = False
    rc = None

    def __repr__(self):
        return "<RemoteCommand '%s' at %d>" % (self.remote_command, id(self))

    def useLog(self, loog, closeWhenFinished=False):
        self.log = loog
        self.closeWhenFinished = closeWhenFinished

    def start(self):
        if self.log is None:
            # orphan LogFile, cannot be subscribed to
            self.log = builder.LogFile(None)
            self.closeWhenFinished = True
        log.msg("LoggedRemoteCommand.start", self.log)
        return RemoteCommand.start(self)

    def addStdout(self, data):
        self.log.addStdout(data)
    def addStderr(self, data):
        self.log.addStderr(data)
    def addHeader(self, data):
        self.log.addHeader(data)
    def remoteUpdate(self, update):
        if update.has_key('stdout'):
            self.addStdout(update['stdout'])
        if update.has_key('stderr'):
            self.addStderr(update['stderr'])
        if update.has_key('header'):
            self.addHeader(update['header'])
        if update.has_key('rc'):
            rc = self.rc = update['rc']
            log.msg("%s rc=%s" % (self, rc))
            self.addHeader("program finished with exit code %d\n" % rc)

    def remoteComplete(self, maybeFailure):
        if self.closeWhenFinished:
            if maybeFailure:
                self.addHeader("\nremoteFailed: %s" % maybeFailure)
            else:
                log.msg("closing log")
            self.log.finish()
        return maybeFailure

class RemoteShellCommand(LoggedRemoteCommand):
    """This class helps you run a shell command on the build slave. It will
    accumulate all the command's output into a Log. When the command is
    finished, it will fire a Deferred. You can then check the results of the
    command and parse the output however you like.

    @param workdir: the directory where the command ought to run, relative to
    the Builder's home directory. If missing or 'None', it will default to
    '.': the same as the Builder's homedir. This should probably be '.' for
    the initial 'cvs checkout' command (which creates a workdir), and the
    workdir for all subsequent commands (including compiles and 'cvs
    update').

    @param command: the shell command to run, like 'make all' or 'cvs
    update'. This should be a list or tuple which can be used directly as the
    argv array. For backwards compatibility, if this is a string, the text
    will be given to '/bin/sh -c %s'.

    @param env: a dict of environment variables to add or change, or None
    to leave the slave's environment alone. Each command gets a separate
    environment; all inherit the slave's initial one.
    TODO: make it possible to delete some or all of the slave's environment.

    @param want_stdout: defaults to 1. Set to 0 if stdout should be thrown
    away. Do this to avoid storing or sending large amounts of useless data.

    @param want_stderr: Set to 0 if stderr should be thrown away.

    @param timeout: tell the remote that if the command fails to produce any
    output for this number of seconds, the command is hung and should be
    killed. Use None to disable the timeout.

    """

    def __init__(self, workdir, command, env=None, 
                 want_stdout=1, want_stderr=1,
                 timeout=20*60, **kwargs):
        self.command = command # stash .command, set it later
        args = {'workdir': workdir,
                'env': env,
                'want_stdout': want_stdout,
                'want_stderr': want_stderr,
                'timeout': timeout,
                }
        LoggedRemoteCommand.__init__(self, "shell", args)

    def start(self):
        self.args['command'] = self.command
        if self.remote_command == "shell":
            # non-ShellCommand slavecommands are responsible for doing this
            # fixup themselves
            if self.step.slaveVersion("shell", "old") == "old":
                self.args['dir'] = self.args['workdir']
        what = "command '%s' in dir '%s'" % (self.args['command'],
                                             self.args['workdir'])
        log.msg(what)
        return LoggedRemoteCommand.start(self)

    def __repr__(self):
        return "<RemoteShellCommand '%s'>" % self.command



class BuildStep:
    """This class represents a single step of the build process. This step
    may involve multiple commands to be run in the build slave, as well as
    arbitrary processing on the master side.

    The step is started by calling startStep(), which returns a Deferred that
    fires when the step finishes.

    __init__ and start are good methods to override. Don't forget to upcall
    BuildStep.__init__ or bad things will happen.

    To launch a RemoteCommand, pass it to .runCommand and wait on the
    Deferred it returns.

    Each BuildStep has a collection of output status. These will all be put
    into the 'step_status' (a BuildStepStatus instance). 'results' is also
    passed to the first callback of the Deferred that is returned by
    .startStep():

     results: this is one of SUCCESS/WARNINGS/FAILURE/SKIPPED
     progress: a StepProgress instance, which tracks ETA
     logs={}: a set of named status.builder.LogFile objects, holding text
     color: a string indicating the color that this step feels best
            represents its current mood. yellow,green,red,orange are the
            most likely choices, although purple indicates an exception
     text=[]: short text strings that describe the command and its status
     text2=[]: short text that is added to the overall build description

    """

    # these parameters are used by the parent Build object to decide how to
    # interpret our results. haltOnFailure will affect the build process
    # immediately, the others will be taken into consideration when
    # determining the overall build status.
    #
    haltOnFailure = False
    flunkOnWarnings = False
    flunkOnFailure = False
    warnOnWarnings = False
    warnOnFailure = False
    parms = ['build', 'name',
             'haltOnFailure',
             'flunkOnWarnings',
             'flunkOnFailure',
             'warnOnWarnings',
             'warnOnFailure',]

    name = "generic"
    progressMetrics = [] # 'time' is implicit
    useProgress = True # set to False if step is really unpredictable
    build = None
    step_status = None
    progress = None

    def __init__(self, **kwargs):
        self.build = kwargs['build'] # required
        for p in BuildStep.parms:
            if kwargs.has_key(p):
                setattr(self, p, kwargs[p])
                del kwargs[p]
        # we want to encourage all steps to get a workdir, so tolerate its
        # presence here. It really only matters for non-ShellCommand steps
        # like Dummy
        if kwargs.has_key('workdir'):
            del kwargs['workdir']
        if kwargs:
            why = "%s.__init__ got unexpected keyword argument(s) %s" \
                  % (self, kwargs.keys())
            raise TypeError(why)

    def setupProgress(self):
        if self.useProgress:
            sp = progress.StepProgress(self.name, self.progressMetrics)
            self.progress = sp
            self.step_status.setProgress(sp)
            return sp
        return None

    def startStep(self, remote):
        """Begin the step. This returns a Deferred that will fire with a
        constant of: SUCCESS, WARNINGS, FAILURE, SKIPPED. Any other status
        can be read out of our attributes.

        'status' is a BuildStepStatus object to which I will send status
        updates. 'progress' is a StepProgress object: I will call
        progress.setProgress(metric, value) as I work. 'remote' is a
        RemoteReference to a buildslave that will execute any RemoteCommands
        I want to run."""

        self.remote = remote
        self.deferred = defer.Deferred()
        if self.progress:
            self.progress.start()
        self.step_status.stepStarted()
        try:
            skip = self.start()
            if skip == SKIPPED:
                reactor.callLater(0, self.deferred.callback, SKIPPED)
        except:
            log.msg("BuildStep.startStep exception in .start")
            self.failed(Failure())
        return self.deferred

    def start(self):
        """Begin the step. Add code here to do local processing, fire off
        remote commands, etc.

        To spawn a command in the buildslave, create a RemoteCommand instance
        and run it with self.runCommand:

         c = RemoteCommandFoo(args)
         d = self.runCommand(c)
         d.addCallback(self.fooDone).addErrback(self.failed)

        As the step runs, it should send status information to the
        BuildStepStatus.

         self.step_status.setColor('red')
         self.step_status.setText(['compile', 'failed'])
         self.step_status.setText2(['4', 'warnings'])

        To add a LogFile, use self.addLog. Make sure it gets closed when it
        finishes. When giving a Logfile to a RemoteShellCommand, just ask it
        to close the log when the command completes:

         log = self.addLog('output')
         cmd = RemoteShellCommand
         cmd.useLog(log, closeWhenFinished=True)

        You can also create complete Logfiles with generated text in a single
        step:

         self.addCompleteLog('warnings', text)

        When the step is done, it should call self.finished(result). 'result'
        will be provided to the BuildProcess, and should one of the constants
        defined above: SUCCESS, WARNINGS, FAILURE, or SKIPPED.

        If the step encounters an exception, it should call self.failed(why).
        'why' should be a Failure object. This automatically fails the whole
        build with an exception. It is a good idea to add self.failed as an
        errback to any Deferreds you might obtain.

        If the step decides it does not need to be run, start() can return
        the constant SKIPPED. This fires the callback immediately: it is not
        necessary to call .finished yourself. This can also indicate to the
        status-reporting mechanism that this step should not be displayed."""
        
        raise NotImplementedError("your subclass must implement this method")

    def interrupt(self, reason):
        """Halt the command, either because the user has decided to cancel
        the build ('reason' is a string), or because the slave has
        disconnected ('reason' is a ConnectionLost Failure). Any further
        local processing should be skipped, and the Step completed with an
        error status. The results text should say something useful like
        ['step', 'interrupted'] or ['remote', 'lost']"""
        pass

    def finished(self, results):
        if self.progress:
            self.progress.finish()
        self.step_status.stepFinished(results)
        self.deferred.callback(results)

    def failed(self, why):
        # if isinstance(why, pb.CopiedFailure): # a remote exception might
        # only have short traceback, so formatFailure is not as useful as
        # you'd like (no .frames, so no traceback is displayed)
        log.msg("BuildStep.failed, traceback follows")
        log.err(why)
        try:
            if self.progress:
                self.progress.finish()
            self.addHTMLLog("err.html", formatFailure(why))
            self.addCompleteLog("err.text", why.getTraceback())
            # could use why.getDetailedTraceback() for more information
            self.step_status.setColor("#c000c0")
            self.step_status.setText([self.name, "exception"])
            self.step_status.setText2([self.name])
            self.step_status.stepFinished(FAILURE)
        except:
            log.msg("exception during failure processing")
            log.err()
            # the progress stuff may still be whacked (the StepStatus may
            # think that it is still running), but the build overall will now
            # finish
        log.msg("BuildStep.failed now firing callback")
        self.deferred.callback(FAILURE)

    # utility methods that BuildSteps may find useful

    def slaveVersion(self, command, oldversion=None):
        return self.build.builder.getSlaveCommandVersion(command, oldversion)

    def addLog(self, name):
        loog = self.step_status.addLog(name)
        return loog

    def addCompleteLog(self, name, text):
        log.msg("addCompleteLog(%s)" % name)
        loog = self.step_status.addLog(name)
        loog.addStdout(text)
        loog.finish()

    def addHTMLLog(self, name, html):
        log.msg("addHTMLLog(%s)" % name)
        self.step_status.addHTMLLog(name, html)

    def runCommand(self, c):
        d = c.run(self, self.remote)
        return d


class ShellCommand(BuildStep):
    """I run a single shell command on the buildslave. I return FAILURE if
    the exit code of that command is non-zero, SUCCESS otherwise. To change
    this behavior, override my .commandFinished method.

    I create a single Log named 'log' which contains the output of the
    command. To create additional summary Logs, override my .createSummary
    method.

    The shell command I run (a list of argv strings) can be provided in
    several ways:

     a class-level .command attribute

     a command= parameter to my constructor (overrides .command)

     set explicitly with my .setCommand() method (overrides both)

    """

    name = "shell"
    description = None # set this to a list of short strings to override
    descriptionDone = None # alternate description when the step is complete
    command = None # set this to a command, or set in kwargs
    progressMetrics = ['output']

    def __init__(self, **kwargs):
        # most of our arguments get passed through to the RemoteShellCommand
        # that we create, but first strip out the ones that we pass to
        # BuildStep (like haltOnFailure and friends)
        self.workdir = kwargs['workdir'] # required by RemoteShellCommand
        buildstep_kwargs = {}
        for k in kwargs.keys()[:]:
            if k in BuildStep.parms:
                buildstep_kwargs[k] = kwargs[k]
                del kwargs[k]
        BuildStep.__init__(self, **buildstep_kwargs)
        kwargs['command'] = kwargs.get('command', self.command)
        self.cmd = RemoteShellCommand(**kwargs)

    def setCommand(self, command):
        self.cmd.command = command

    def describe(self, done=False):
        """Return a list of short strings to describe this step. This uses
        the first few words of the shell command. You can replace this by
        setting .description in your subclass, or by overriding this method
        to describe the step better.

        done=False is used to describe the step while it is running, so a
        single imperfect-tense verb is appropriate ('compiling', 'testing',
        etc). done=True is used when the step has finished, and the default
        getText() method adds some text, so a noun is appropriate
        ('compile', 'tests', etc)."""

        if done and self.descriptionDone is not None:
            return self.descriptionDone
        if self.description is not None:
            return self.description

        words = self.cmd.command
        if type(words) in types.StringTypes:
            words = words.split()
        if len(words) < 1:
            return ["???"]
        if len(words) == 1:
            return ["'%s'" % words[0]]
        if len(words) == 2:
            return ["'%s" % words[0], "%s'" % words[1]]
        return ["'%s" % words[0], "%s" % words[1], "...'"]

    def start(self, errorMessage=None):
        self.step_status.setColor("yellow")
        self.step_status.setText(self.describe(False))
        loog = self.addLog("log")
        if errorMessage:
            loog.addHeader(errorMessage)
        log.msg("ShellCommand.start using log", loog)
        log.msg(" for cmd", self.cmd)
        self.cmd.useLog(loog, True)
        loog.logProgressTo(self.progress, "output")
        d = self.runCommand(self.cmd)
        d.addCallbacks(self._commandComplete, self.checkDisconnect)
        d.addErrback(self.failed)

    def interrupt(self, reason):
        # TODO: consider adding an INTERRUPTED or STOPPED status to use
        # instead of FAILURE, might make the text a bit more clear.
        # 'reason' can be a Failure, or text
        self.addCompleteLog('interrupt', str(reason))
        d = self.cmd.interrupt(reason)
        return d

    def checkDisconnect(self, f):
        f.trap(error.ConnectionLost)
        self.step_status.setColor("red")
        self.step_status.setText(self.describe(True) +
                                 ["failed", "slave", "lost"])
        self.step_status.setText2(["failed", "slave", "lost"])
        return self.finished(FAILURE)

    def _commandComplete(self, cmd):
        self.commandComplete(cmd)
        self.createSummary(cmd.log)
        results = self.evaluateCommand(cmd)
        self.setStatus(cmd, results)
        return self.finished(results)

    # to refine the status output, override one or more of the following
    # methods. Change as little as possible: start with the first ones on
    # this list and only proceed further if you have to    
    #
    # createSummary: add additional Logfiles with summarized results
    # evaluateCommand: decides whether the step was successful or not
    #
    # getText: create the final per-step text strings
    # describeText2: create the strings added to the overall build status
    #
    # getText2: only adds describeText2() when the step affects build status
    #
    # setStatus: handles all status updating

    # commandComplete is available for general-purpose post-completion work.
    # It is a good place to do one-time parsing of logfiles, counting
    # warnings and errors. It should probably stash such counts in places
    # like self.warnings so they can be picked up later by your getText
    # method.

    # TODO: most of this stuff should really be on BuildStep rather than
    # ShellCommand. That involves putting the status-setup stuff in
    # .finished, which would make it hard to turn off.

    def commandComplete(self, cmd):
        """This is a general-purpose hook method for subclasses. It will be
        called after the remote command has finished, but before any of the
        other hook functions are called."""
        pass

    def createSummary(self, log):
        """To create summary logs, do something like this:
        warnings = grep('^Warning:', log.getText())
        self.addCompleteLog('warnings', warnings)
        """
        pass

    def evaluateCommand(self, cmd):
        """Decide whether the command was SUCCESS, WARNINGS, or FAILURE.
        Override this to, say, declare WARNINGS if there is any stderr
        activity, or to say that rc!=0 is not actually an error."""

        if cmd.rc != 0:
            return FAILURE
        # if cmd.log.getStderr(): return WARNINGS
        return SUCCESS

    def getText(self, cmd, results):
        if results == SUCCESS:
            return self.describe(True)
        elif results == WARNINGS:
            return self.describe(True) + ["warnings"]
        else:
            return self.describe(True) + ["failed"]

    def getText2(self, cmd, results):
        """We have decided to add a short note about ourselves to the overall
        build description, probably because something went wrong. Return a
        short list of short strings. If your subclass counts test failures or
        warnings of some sort, this is a good place to announce the count."""
        # return ["%d warnings" % warningcount]
        # return ["%d tests" % len(failedTests)]
        return [self.name]

    def maybeGetText2(self, cmd, results):
        if results == SUCCESS:
            # successful steps do not add anything to the build's text
            pass
        elif results == WARNINGS:
            if (self.flunkOnWarnings or self.warnOnWarnings):
                # we're affecting the overall build, so tell them why
                return self.getText2(cmd, results)
        else:
            if (self.haltOnFailure or self.flunkOnFailure
                or self.warnOnFailure):
                # we're affecting the overall build, so tell them why
                return self.getText2(cmd, results)
        return []

    def getColor(self, cmd, results):
        assert results in (SUCCESS, WARNINGS, FAILURE)
        if results == SUCCESS:
            return "green"
        elif results == WARNINGS:
            return "orange"
        else:
            return "red"

    def setStatus(self, cmd, results):
        # this is good enough for most steps, but it can be overridden to
        # get more control over the displayed text
        self.step_status.setColor(self.getColor(cmd, results))
        self.step_status.setText(self.getText(cmd, results))
        self.step_status.setText2(self.maybeGetText2(cmd, results))



    
class TreeSize(ShellCommand):
    name = "treesize"
    command = ["du", "-s", "."]
    kb = None

    def commandComplete(self, cmd):
        out = cmd.log.getText()
        m = re.search(r'^(\d+)', out)
        if m:
            self.kb = int(m.group(1))

    def evaluateCommand(self, cmd):
        if cmd.rc != 0:
            return FAILURE
        if self.kb is None:
            return WARNINGS # not sure how 'du' could fail, but whatever
        return SUCCESS

    def getText(self, cmd, results):
        if kb is not None:
            return ["treesize", "%d kb" % self.kb]
        return ["treesize", "unknown"]


class Source(ShellCommand):
    """This is a base class to generate a source tree in the buildslave.
    Each version control system has a specialized subclass, and is expected
    to override __init__ and implement computeSourceRevision() and
    startVC(). The class as a whole builds up the self.args dictionary, then
    starts a LoggedRemoteCommand with those arguments.

    @param workdir: a string giving the local directory (relative to the
    Builder's root) where the tree should be placed.

    @param mode: a string describing the kind of VC operation that is
    desired.

     'update' specifies that the checkout/update should be performed
     directly into the workdir. Each build is performed in the same
     directory, allowing for incremental builds. This minimizes disk space,
     bandwidth, and CPU time. However, it may encounter problems if the
     build process does not handle dependencies properly (if you must
     sometimes do a 'clean build' to make sure everything gets compiled), or
     if source files are deleted but generated files can influence test
     behavior (e.g. python's .pyc files), or when source directories are
     deleted but generated files prevent CVS from removing them.

     'copy' specifies that the source-controlled workspace should be
     maintained in a separate directory (called the 'copydir'), using
     checkout or update as necessary. For each build, a new workdir is
     created with a copy of the source tree (rm -rf workdir; cp -r copydir
     workdir). This doubles the disk space required, but keeps the bandwidth
     low (update instead of a full checkout). A full 'clean' build is
     performed each time. This avoids any generated-file build problems, but
     is still occasionally vulnerable to problems such as a CVS repository
     being manually rearranged (causing CVS errors on update) which are not
     an issue with a full checkout.

     'clobber' specifes that the working directory should be deleted each
     time, necessitating a full checkout for each build. This insures a
     clean build off a complete checkout, avoiding any of the problems
     described above, but is bandwidth intensive, as the whole source tree
     must be pulled down for each build.

     'export' is like 'clobber', except that e.g. the 'cvs export' command
     is used to create the working directory. This command removes all VC
     metadata files (the CVS/.svn/{arch} directories) from the tree, which
     is sometimes useful for creating source tarballs (to avoid including
     the metadata in the tar file). Not all VC systems support export.

    @param alwaysUseLatest: normally the Source step asks its Build for a
     list of all Changes that are supposed to go into the build, then
     computes a 'source stamp' (revision number or timestamp) that will
     cause exactly that set of changes to be present in the checked out
     tree. This is turned into, e.g., 'cvs update -D timestamp', or 'svn
     update -r revnum'. If alwaysUseLatest=True, bypass this computation and
     always update to the latest available sources for each build.

     The source stamp helps avoid a race condition in which someone commits
     a change after the master has decided to start a build but before the
     slave finishes checking out the sources. At best this results in a
     build which contains more changes than the buildmaster thinks it has
     (possibly resulting in the wrong person taking the blame for any
     problems that result), at worst is can result in an incoherent set of
     sources (splitting a non-atomic commit) which may not build at all.

    """

    # if the checkout fails, there's no point in doing anything else
    haltOnFailure = True
    notReally = False

    def __init__(self, workdir, mode='update', alwaysUseLatest=False,
                 timeout=20*60, **kwargs):
        BuildStep.__init__(self, **kwargs)
        assert mode in ("update", "copy", "clobber", "export")
        self.args = {'mode': mode,
                     'workdir': workdir,
                     'timeout': timeout,
                     'patch': None, # set during .start
                     }
        self.alwaysUseLatest = alwaysUseLatest

        self.description = ["updating"]
        self.descriptionDone = ["update"]
        if mode == "clobber":
            self.description = ["checkout"]
            # because checkingouting takes too much space
            self.descriptionDone = ["checkout"]
        elif mode == "export":
            self.description = ["exporting"]
            self.descriptionDone = ["export"]

    def computeSourceRevision(self, changes):
        """Each subclass must implement this method to do something more
        precise than -rHEAD every time. For version control systems that use
        repository-wide change numbers (SVN, P4), this can simply take the
        maximum such number from all the changes involved in this build. For
        systems that do not (CVS), it needs to create a timestamp based upon
        the latest Change, the Build's treeStableTimer, and an optional
        self.checkoutDelay value."""
        return None

    def start(self):
        if self.notReally:
            log.msg("faking %s checkout/update" % self.name)
            self.step_status.setColor("green")
            self.step_status.setText(["fake", self.name, "successful"])
            self.addCompleteLog("log",
                                "Faked %s checkout/update 'successful'\n" \
                                % self.name)
            return SKIPPED

        # can we construct a source stamp?
        #revision = None # default: use the latest sources (-rHEAD)
        revision, patch = self.build.getSourceStamp()
        # 'patch' is None or a tuple of (patchlevel, diff)
        if not revision and not self.alwaysUseLatest:
            changes = self.build.allChanges()
            revision = self.computeSourceRevision(changes)
        self.args['revision'] = revision
        self.args['patch'] = patch
        self.startVC()

class CVS(Source):
    """I do CVS checkout/update operations.

    @param cvsroot (required): a string which describes the CVS Repository
    from which the source tree should be obtained. '/home/warner/Repository'
    for local or NFS-reachable repositories, ':pserver:anon@foo.com:/cvs'
    for anonymous CVS, 'user@host.com:/cvs' for non-anonymous CVS or CVS
    over ssh. Lots of possibilities, check the CVS documentation for more.

    @param cvsmodule (required): a string giving the subdirectory of the CVS
    repository that should be retrieved.

    @param login: if not None, a string which will be provided as a password
    to the 'cvs login' command, used when a :pserver: method is used to
    access the repository. This login is only needed once, but must be run
    each time (just before the CVS operation) because there is no way for
    the buildslave to tell whether it was previously performed or not.

    @param branch: a string to be used in a '-r' argument to specify which
    named branch of the source tree should be used for this checkout.
    Defaults to 'HEAD'.

    @param checkoutDelay: if not None, the number of seconds to put between
    the last known Change and the timestamp given to the -D argument. This
    defaults to exactly half of the parent Build's .treeStableTimer, but it
    could be set to something else if your CVS change notification has
    particularly weird latency characteristics.

    @param global_options=[]: these arguments are inserted in the cvs
    command line, before the 'checkout'/'update' command word. See 'cvs
    --help-options' for a list of what may be accepted here. ['-r'] will
    make the checked out files read only. ['-r', '-R'] will also assume the
    repository is read-only (I assume this means it won't use locks to
    insure atomic access to the ,v files).

    Note: if you are doing anonymous/pserver CVS operations, you will need
    to manually do a 'cvs login' on each buildslave before the slave has any
    hope of success. XXX: fix then, take a cvs password as an argument and
    figure out how to do a 'cvs login' on each build
    """

    name = "cvs"

    #progressMetrics = ['output']
    #
    # additional things to track: update gives one stderr line per directory
    # (starting with 'cvs server: Updating ') (and is fairly stable if files
    # is empty), export gives one line per directory (starting with 'cvs
    # export: Updating ') and another line per file (starting with U). Would
    # be nice to track these, requires grepping LogFile data for lines,
    # parsing each line. Might be handy to have a hook in LogFile that gets
    # called with each complete line.

    def __init__(self, cvsroot, cvsmodule, 
                 global_options=[], branch="HEAD", checkoutDelay=None,
                 login=None,
                 clobber=0, export=0, copydir=None,
                 **kwargs):

        self.checkoutDelay = checkoutDelay

        if not kwargs.has_key('mode') and (clobber or export or copydir):
            # deal with old configs
            warnings.warn("Please use mode=, not clobber/export/copydir",
                          DeprecationWarning)
            if export:
                kwargs['mode'] = "export"
            elif clobber:
                kwargs['mode'] = "clobber"
            elif copydir:
                kwargs['mode'] = "copy"
            else:
                kwargs['mode'] = "update"

        Source.__init__(self, **kwargs)

        self.args.update({'cvsroot': cvsroot,
                          'cvsmodule': cvsmodule,
                          'global_options': global_options,
                          'branch': branch,
                          'login': login,
                          })

    def computeSourceRevision(self, changes):
        if not changes:
            return None
        lastChange = max([c.when for c in changes])
        if self.checkoutDelay is not None:
            when = lastChange + self.checkoutDelay
        else:
            when = lastChange + self.build.treeStableTimer / 2
        return formatdate(when)

    def startVC(self):
        if self.args['branch'] == "HEAD" and self.args['revision']:
            # special case. 'cvs update -r HEAD -D today' gives no files
            # TODO: figure out why, see if it applies to -r BRANCH
            self.args['branch'] = None

        # accomodate old slaves
        slavever = self.slaveVersion("cvs", "old")
        if slavever == "old":
            # 0.5.0
            if self.args['mode'] == "export":
                self.args['export'] = 1
            elif self.args['mode'] == "clobber":
                self.args['clobber'] = 1
            elif self.args['mode'] == "copy":
                self.args['copydir'] = "source"
            self.args['tag'] = self.args['branch']
            assert not self.args['patch'] # 0.5.0 slave can't do patch

        self.cmd = LoggedRemoteCommand("cvs", self.args)
        ShellCommand.start(self)



class SVN(Source):
    """I perform Subversion checkout/update operations.

    @param svnurl (required): the URL which points to the Subversion server.
    This one string combines the access method (HTTP, ssh, local file), the
    repository host/port, the repository path, the sub-tree within the
    repository, and which branch to check out.
    """

    name = 'svn'

    def __init__(self, svnurl, directory=None, **kwargs):

        if not kwargs.has_key('workdir') and directory is not None:
            # deal with old configs
            warnings.warn("Please use workdir=, not directory=",
                          DeprecationWarning)
            kwargs['workdir'] = directory
        Source.__init__(self, **kwargs)

        self.args['svnurl'] = svnurl

    def computeSourceRevision(self, changes):
        if not changes:
            return None
        lastChange = max([c.revision for c in changes])
        return lastChange

    def startVC(self):
        # accomodate old slaves
        errorMessage = None
        slavever = self.slaveVersion("svn", "old")
        assert slavever, "slave does not have the 'svn' command"
        if slavever == "old":
            # 0.5.0 compatibility
            if self.args['mode'] in ("clobber", "copy"):
                # TODO: use some shell commands to make up for the
                # deficiency, by blowing away the old directory first (thus
                # forcing a full checkout)
                errorMessage = "WARNING: this slave can only do SVN updates"
                errorMessage += ", not mode=%s\n" % self.args['mode']
                log.msg("WARNING: this slave only does mode=update")
            assert self.args['mode'] != "export" # more serious
            self.args['directory'] = self.args['workdir']
            if self.args['revision'] is not None:
                # 0.5.0 can only do HEAD
                errorMessage = "WARNING: this slave can only update to HEAD"
                errorMessage += ", not revision=%s\n" % self.args['revision']
                log.msg("WARNING: this slave only does -rHEAD")
            self.args['revision'] = "HEAD" # interprets this key differently
            assert not self.args['patch'] # 0.5.0 slave can't do patch

        self.cmd = LoggedRemoteCommand("svn", self.args)
        ShellCommand.start(self, errorMessage)

class Darcs(Source):
    """Check out a source tree from a Darcs repository at 'repourl'.

    To the best of my knowledge, Darcs has no concept of file modes. This
    means the eXecute-bit will be cleared on all source files. As a result,
    you may need to invoke configuration scripts with something like:

     s(step.Configure, command=['/bin/sh', './configure'])
    """

    name = "darcs"

    def __init__(self, repourl, **kwargs):
        assert kwargs['mode'] != "export", \
               "Darcs does not have an 'export' mode"
        Source.__init__(self, **kwargs)
        self.args['repourl'] = repourl

    def startVC(self):
        slavever = self.slaveVersion("darcs")
        assert slavever, "slave is too old, does not know about darcs"
        self.cmd = LoggedRemoteCommand("darcs", self.args)
        ShellCommand.start(self)

class Arch(Source):
    """Check out a source tree from an Arch repository at 'url'. 'version'
    specifies which version number (development line) will be used for the
    checkout: this is mostly equivalent to a branch name.
    
    This step will first register the archive, which requires a per-user
    'archive name' to correspond to the URL from which the sources can be
    fetched. The archive's default name will be used for this unless you
    override it by setting the 'archive' parameter. You might want to do
    this if, for some reason, you are hosting the archive on the same
    machine (and in the same account) as the build slave, and you don't want
    to confuse local access with remote access.

    [forgive the confusion expressed in the previous paragraph, I'm still
    trying to get my head around Arch.. -warner]
    """

    name = "arch"

    def __init__(self, url, version, archive=None, **kwargs):
        Source.__init__(self, **kwargs)
        self.args.update({'url': url,
                          'version': version,
                          'archive': archive,
                          })

    def startVC(self):
        slavever = self.slaveVersion("arch")
        assert slavever, "slave is too old, does not know about arch"
        self.cmd = LoggedRemoteCommand("arch", self.args)
        ShellCommand.start(self)

class todo_P4(Source):
    name = "p4"

    # to create the working directory for the first time:
    #  need to create a View. The 'Root' parameter will have to be filled
    #  in by the buildslave with the abspath of the basedir. Then the
    #  setup process involves 'p4 client' to set up the view. After
    #  that, 'p4 sync' does all the necessary updating.
    #  P4PORT=P4PORT P4CLIENT=name p4 client

    def __init__(self, p4port, view, **kwargs):
        Source.__init__(self, **kwargs)
        self.args.update({'p4port': p4port,
                          'view': view,
                          })

    def startVC(self):
        self.cmd = LoggedRemoteCommand("p4", self.args)
        ShellCommand.start(self)

class P4Sync(Source):
    """This is a partial solution for using a P4 source repository. You are
    required to manually set up each build slave with a useful P4
    environment, which means setting various per-slave environment variables,
    and creating a P4 client specification which maps the right files into
    the slave's working directory. Once you have done that, this step merely
    performs a 'p4 sync' to update that workspace with the newest files.

    Each slave needs the following environment:

    PATH: the 'p4' binary must be on the slave's PATH
    P4USER: each slave needs a distinct user account
    P4CLIENT: each slave needs a distinct client specification

    You should use 'p4 client' (?) to set up a client view spec which maps
    the desired files into $SLAVEBASE/$BUILDERBASE/source .
    """

    name = "p4sync"

    def __init__(self, p4port, **kwargs):
        assert kwargs['mode'] == "copy", "P4Sync can only be used in mode=copy"
        Source.__init__(self, **kwargs)
        self.args['p4port'] = p4port

    def computeSourceRevision(self, changes):
        if not changes:
            return None
        lastChange = max([c.revision for c in changes])
        return lastChange

    def startVC(self):
        slavever = self.slaveVersion("p4sync")
        assert slavever, "slave is too old, does not know about p4"
        self.cmd = LoggedRemoteCommand("p4sync", self.args)
        ShellCommand.start(self)


class Dummy(BuildStep):
    """I am a dummy no-op step that takes 5 seconds to complete.
    @param timeout: the number of seconds to delay
    """

    haltOnFailure = True
    name = "dummy"

    def __init__(self, timeout=5, **kwargs):
        BuildStep.__init__(self, **kwargs)
        self.timeout = timeout
        self.timer = None

    def start(self):
        self.step_status.setColor("yellow")
        self.step_status.setText(["delay", "%s secs" % self.timeout])
        self.timer = reactor.callLater(self.timeout, self.done)

    def interrupt(self, reason):
        if self.timer:
            self.timer.cancel()
            self.timer = None
            self.step_status.setColor("red")
            self.step_status.setText(["delay", "interrupted"])
            self.finished(FAILURE)

    def done(self):
        self.step_status.setColor("green")
        self.finished(SUCCESS)

class FailingDummy(Dummy):
    """I am a dummy step that raises an Exception after 5 seconds
    @param timeout: the number of seconds to delay
    """

    name = "failing dummy"

    def start(self):
        self.step_status.setColor("yellow")
        self.step_status.setText(["boom", "%s secs" % self.timeout])
        self.timer = reactor.callLater(self.timeout, self.done)

    def done(self):
        class Boom(Exception):
            pass
        try:
            raise Boom("boom")
        except Boom:
            f = Failure()
        self.failed(f)

class RemoteDummy(ShellCommand):
    """I am a dummy no-op step that runs on the remote side and takes 5
    seconds to complete.

    @param timeout: the number of seconds to delay
    @param results: None
    """

    haltOnFailure = True
    name = "remote dummy"

    def __init__(self, timeout=5, **kwargs):
        BuildStep.__init__(self, **kwargs)
        args = {'timeout': timeout}
        self.cmd = LoggedRemoteCommand("dummy", args)
        self.description = ["remote", "delay", "%s secs" % timeout]

class Configure(ShellCommand):

    name = "configure"
    haltOnFailure = 1
    description = ["configuring"]
    descriptionDone = ["configure"]
    command = ["./configure"]
    
class Compile(ShellCommand):

    name = "compile"
    haltOnFailure = 1
    description = ["compiling"]
    descriptionDone = ["compile"]
    command = ["make", "all"]

    OFFprogressMetrics = ['output']
    # things to track: number of files compiled, number of directories
    # traversed (assuming 'make' is being used)

    def createSummary(self, cmd):
        # TODO: grep for the characteristic GCC warning/error lines and
        # assemble them into a pair of buffers
        pass

class Test(ShellCommand):

    name = "test"
    warnOnFailure = 1
    description = ["testing"]
    descriptionDone = ["test"]
    command = ["make", "test"]
