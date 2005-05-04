# -*- test-case-name: buildbot.test.test_slavecommand -*-

import os, os.path, re, signal, shutil, types

from twisted.internet.protocol import ProcessProtocol
from twisted.internet import reactor, defer
from twisted.python import log, failure, runtime

from buildbot.slave.interfaces import ISlaveCommand
from buildbot.slave.registry import registerSlaveCommand

cvs_ver = '$Revision$'[1+len("Revision: "):-2]

# version history:
#  >=1.17: commands are interruptable
#  >=1.28: Arch understands 'revision', added Bazaar

class CommandInterrupted(Exception):
    pass
class TimeoutError(Exception):
    pass

class AbandonChain(Exception):
    """A series of chained steps can raise this exception to indicate that
    one of the intermediate ShellCommands has failed, such that there is no
    point in running the remainder. 'rc' should be the non-zero exit code of
    the failing ShellCommand."""

    def __repr__(self):
        return "<AbandonChain rc=%s>" % self.args[0]

class ShellCommandPP(ProcessProtocol):
    debug = False

    def __init__(self, command):
        self.command = command

    def connectionMade(self):
        if self.debug:
            log.msg("ShellCommandPP.connectionMade")
        if self.command.stdin:
            if self.debug: log.msg(" writing to stdin")
            self.transport.write(self.command.stdin)

        # TODO: maybe we shouldn't close stdin when using a PTY. I can't test
        # this yet, recent debian glibc has a bug which causes thread-using
        # test cases to SIGHUP trial, and the workaround is to either run
        # the whole test with /bin/sh -c " ".join(argv)  (way gross) or to
        # not use a PTY. Once the bug is fixed, I'll be able to test what
        # happens when you close stdin on a pty. My concern is that it will
        # SIGHUP the child (since we are, in a sense, hanging up on them).
        # But it may well be that keeping stdout open prevents the SIGHUP
        # from being sent.
        #if not self.command.usePTY:

        if self.debug: log.msg(" closing stdin")
        self.transport.closeStdin()

    def outReceived(self, data):
        if self.debug:
            log.msg("ShellCommandPP.outReceived")
        self.command.addStdout(data)

    def errReceived(self, data):
        if self.debug:
            log.msg("ShellCommandPP.errReceived")
        self.command.addStderr(data)

    def processEnded(self, status_object):
        if self.debug:
            log.msg("ShellCommandPP.processEnded", status_object)
        # status_object is a Failure wrapped around an
        # error.ProcessTerminated or and error.ProcessDone.
        # requires twisted >= 1.0.4 to overcome a bug in process.py
        sig = status_object.value.signal
        rc = status_object.value.exitCode
        self.command.finished(sig, rc)

class ShellCommand:
    # This is a helper class, used by SlaveCommands to run programs in a
    # child shell.

    notreally = False

    def __init__(self, builder, command,
                 workdir, environ=None,
                 sendStdout=True, sendStderr=True, sendRC=True,
                 timeout=None, stdin=None, keepStdout=False):
        self.builder = builder
        self.command = command
        self.sendStdout = sendStdout
        self.sendStderr = sendStderr
        self.sendRC = sendRC
        self.workdir = workdir
        self.environ = os.environ.copy()
        if environ:
            if (self.environ.has_key('PYTHONPATH')
                and environ.has_key('PYTHONPATH')):
                # special case, prepend the builder's items to the existing
                # ones. This will break if you send over empty strings, so
                # don't do that.
                environ['PYTHONPATH'] = (environ['PYTHONPATH']
                                         + os.pathsep
                                         + self.environ['PYTHONPATH'])
                # this will proceed to replace the old one
            self.environ.update(environ)
        self.stdin = stdin
        self.timeout = timeout
        self.timer = None
        self.interrupted = 0
        self.keepStdout = keepStdout

        # usePTY=True is a convenience for cleaning up all children and
        # grandchildren of a hung command. Fall back to usePTY=False on
        # systems where ptys cause problems.

        self.usePTY = self.builder.usePTY
        if runtime.platformType != "posix":
            self.usePTY = False # PTYs are posix-only
        if stdin is not None:
            # for .closeStdin to matter, we must use a pipe, not a PTY
            self.usePTY = False

    def __repr__(self):
        return "<slavecommand.ShellCommand '%s'>" % self.command

    def sendStatus(self, status):
        self.builder.sendUpdate(status)

    def start(self):
        # return a Deferred which fires (with the exit code) when the command
        # completes
        if self.keepStdout:
            self.stdout = ""
        self.deferred = defer.Deferred()
        try:
            self._startCommand()
        except:
            log.msg("error in ShellCommand._startCommand")
            log.err()
            # pretend it was a shell error
            self.deferred.errback(AbandonChain(-1))
        return self.deferred

    def _startCommand(self):
        msg = "command '%s' in dir %s" % (self.command, self.workdir)
        if self.timeout:
            msg += " (timeout %d secs)" % self.timeout
        log.msg("  " + msg)
        self.sendStatus({'header': msg+"\n"})
        log.msg("  env is: %s" % self.environ)
        if self.notreally:
            self.sendStatus({'header': "(not really)\n"})
            self.finished(None, 0)
            return

        self.pp = ShellCommandPP(self)

        if type(self.command) in types.StringTypes:
            if runtime.platformType  == 'win32':
                argv = [os.environ['COMSPEC'], '/c', self.command]
            else:
                # for posix, use /bin/sh. for other non-posix, well, doesn't
                # hurt to try
                argv = ['/bin/sh', '-c', self.command]
        else:
            if runtime.platformType  == 'win32':
                argv = [os.environ['COMSPEC'], '/c'] + list(self.command)
            else:
                argv = self.command

        # self.stdin is handled in ShellCommandPP.connectionMade

        self.process = reactor.spawnProcess(self.pp, argv[0], argv,
                                            self.environ,
                                            self.workdir,
                                            usePTY=self.usePTY)

        # connectionMade also closes stdin as long as we're not using a PTY.
        # This is intended to kill off inappropriately interactive commands
        # better than the (long) hung-command timeout. ProcessPTY should be
        # enhanced to allow the same childFDs argument that Process takes,
        # which would let us connect stdin to /dev/null .


        if self.timeout:
            self.timer = reactor.callLater(self.timeout, self.doTimeout)

    def addStdout(self, data):
        if self.interrupted: return
        if self.sendStdout: self.sendStatus({'stdout': data})
        if self.keepStdout: self.stdout += data
        if self.timer: self.timer.reset(self.timeout)

    def addStderr(self, data):
        if self.interrupted: return
        if self.sendStderr: self.sendStatus({'stderr': data})
        if self.timer: self.timer.reset(self.timeout)

    def finished(self, sig, rc):
        log.msg("command finished with signal %s, exit code %s" % (sig,rc))
        if self.interrupted:
            return
        if sig is not None:
            rc = -1
        if self.sendRC:
            if sig is not None:
                self.sendStatus(
                    {'header': "process killed by signal %d\n" % sig})
            self.sendStatus({'rc': rc})
        if self.timer:
            self.timer.cancel()
            self.timer = None
        self.deferred.callback(rc)

    def doTimeout(self):
        msg = "command timed out: %d seconds without output" % self.timeout
        self.kill(msg)

    def kill(self, msg):
        msg += ", killing pid %d" % self.process.pid
        log.msg(msg)
        self.sendStatus({'header': "\n" + msg + "\n"})

        hit = 0
        if runtime.platformType == "posix":
            try:
                # really want to kill off all child processes too. Process
                # Groups are ideal for this, but that requires
                # spawnProcess(usePTY=1). Try both ways in case process was
                # not started that way.
                log.msg("trying os.kill(-pid, signal.SIGKILL)")
                os.kill(-self.process.pid, signal.SIGKILL)
                log.msg(" successful")
                hit = 1
            except OSError:
                # probably no-such-process, maybe because there is no process
                # group
                pass
        if not hit:
            try:
                log.msg("trying process.signalProcess('KILL')")
                self.process.signalProcess('KILL')
                log.msg(" successful")
                hit = 1
            except OSError:
                # could be no-such-process, because they finished very recently
                pass
        if not hit:
            log.msg("signalProcess/os.kill failed both times")
        # finished ought to be called momentarily
        self.timer = reactor.callLater(5, self.doBackupTimeout) # just in case

    def doBackupTimeout(self):
        # we tried to kill the process, and it wouldn't die. Finish anyway.
        self.sendStatus({'header': "SIGKILL failed to kill process\n"})
        self.timer = None
        self.pp.command = None # take away its voice
        # note, if/when the command finally does complete, an exception will
        # be raised as pp tries to send status through .command
        self.commandFailed(TimeoutError("SIGKILL failed to kill process"))


class Command:
    __implements__ = ISlaveCommand,

    """This class defines one command that can be invoked by the build master.
    The command is executed on the slave side, and always sends back a
    completion message when it finishes. It may also send intermediate status
    as it runs (by calling builder.sendStatus). Some commands can be
    interrupted (either by the build master or a local timeout), in which
    case the step is expected to complete normally with a status message that
    indicates an error occurred.

    These commands are used by BuildSteps on the master side. Each kind of
    BuildStep uses a single Command. The slave must implement all the
    Commands required by the set of BuildSteps used for any given build:
    this is checked at startup time.

    All Commands are constructed with the same signature:
     c = CommandClass(builder, args)
    where 'builder' is the parent SlaveBuilder object, and 'args' is a
    dict that is interpreted per-command.

    The setup(args) method is available for setup, and is run from __init__.

    The Command is started with start(). This method must be implemented in a
    subclass, and it should return a Deferred. When your step is done, you
    should fire the Deferred (the results are not used). If the command is
    interrupted, it should fire the Deferred anyway.

    While the command runs. it may send status messages back to the
    buildmaster by calling self.sendStatus(statusdict). The statusdict is
    interpreted by the master-side BuildStep however it likes.

    A separate completion message is sent when the deferred fires, which
    indicates that the Command has finished, but does not carry any status
    data. If the Command needs to return an exit code of some sort, that
    should be sent as a regular status message before the deferred is fired .
    Once builder.commandComplete has been run, no more status messages may be
    sent.

    If interrupt() is called, the Command should attempt to shut down as
    quickly as possible. Child processes should be killed, new ones should
    not be started. The Command should send some kind of error status update,
    then complete as usual by firing the Deferred.

    .interrupted should be set by interrupt(), and can be tested to avoid
    sending multiple error status messages.

    If .running is False, the bot is shutting down (or has otherwise lost the
    connection to the master), and should not send any status messages. This
    is checked in Command.sendStatus .

    """

    # builder methods:
    #  sendStatus(dict) (zero or more)
    #  commandComplete() or commandInterrupted() (one, at end)

    debug = False
    interrupted = False
    running = False # set by Builder, cleared on shutdown or when the
                    # Deferred fires

    def __init__(self, builder, stepId, args):
        self.builder = builder
        self.stepId = stepId # just for logging
        self.args = args
        self.setup(args)

    def setup(self, args):
        """Override this in a subclass to extract items from the args dict."""
        pass
        
    def start(self):
        """Start the command. self.running will be set just before this is
        called. This method should return a Deferred that will fire when the
        command has completed. The Deferred's argument will be ignored.

        This method should be overridden by subclasses."""
        raise NotImplementedError, "You must implement this in a subclass"

    def sendStatus(self, status):
        """Send a status update to the master."""
        if self.debug:
            log.msg("sendStatus", status)
        if not self.running:
            log.msg("would sendStatus but not .running")
            return
        self.builder.sendUpdate(status)

    def interrupt(self):
        """Override this in a subclass to allow commands to be interrupted.
        May be called multiple times, test and set self.interrupted=True if
        this matters."""
        pass

    # utility methods, mostly used by SlaveShellCommand and the like

    def _abandonOnFailure(self, rc):
        if type(rc) is not int:
            log.msg("weird, _abandonOnFailure was given rc=%s (%s)" % \
                    (rc, type(rc)))
        assert type(rc) is int
        if rc != 0:
            raise AbandonChain(rc)
        return rc

    def _sendRC(self, res):
        self.sendStatus({'rc': 0})

    def _checkAbandoned(self, why):
        log.msg("_checkAbandoned", why)
        why.trap(AbandonChain)
        log.msg(" abandoning chain", why.value)
        self.sendStatus({'rc': why.value.args[0]})
        return None


class SlaveShellCommand(Command):
    """This is a Command which runs a shell command. The args dict contains
    the following keys:

        - ['command'] (required): a shell command to run. If this is a string,
          it will be run with /bin/sh (['/bin/sh', '-c', command]). If it is a
          list (preferred), it will be used directly.
        - ['workdir'] (required): subdirectory in which the command will be run,
          relative to the builder dir
        - ['env']: a dict of environment variables to augment/replace os.environ
        - ['want_stdout']: 0 if stdout should be thrown away
        - ['want_stderr']: 0 if stderr should be thrown away
        - ['not_really']: 1 to skip execution and return rc=0
        - ['timeout']: seconds of silence to tolerate before killing command

    ShellCommand creates the following status messages:
        - {'stdout': data} : when stdout data is available
        - {'stderr': data} : when stderr data is available
        - {'header': data} : when headers (command start/stop) are available
        - {'rc': rc} : when the process has terminated
    """

    def start(self):
        args = self.args
        sendStdout = args.get('want_stdout', True)
        sendStderr = args.get('want_stderr', True)
        # args['workdir'] is relative to Builder directory, and is required.
        assert args['workdir'] is not None
        workdir = os.path.join(self.builder.basedir, args['workdir'])
        timeout = args.get('timeout', None)

        c = ShellCommand(self.builder, args['command'],
                         workdir, environ=args.get('env'),
                         timeout=timeout,
                         sendStdout=sendStdout, sendStderr=sendStderr,
                         sendRC=True)
        self.command = c
        d = self.command.start()
        return d

    def interrupt(self):
        self.interrupted = True
        self.command.kill("command interrupted")


registerSlaveCommand("shell", SlaveShellCommand, cvs_ver)


class DummyCommand(Command):
    """
    I am a dummy no-op command that by default takes 5 seconds to complete.
    See L{buildbot.process.step.RemoteDummy}
    """
    
    def start(self):
        self.d = defer.Deferred()
        log.msg("  starting dummy command [%s]" % self.stepId)
        self.timer = reactor.callLater(1, self.doStatus)
        return self.d

    def interrupt(self):
        if self.interrupted:
            return
        self.timer.cancel()
        self.timer = None
        self.interrupted = True
        self.finished()

    def doStatus(self):
        log.msg("  sending intermediate status")
        self.sendStatus({'stdout': 'data'})
        timeout = self.args.get('timeout', 5) + 1
        self.timer = reactor.callLater(timeout - 1, self.finished)

    def finished(self):
        log.msg("  dummy command finished [%s]" % self.stepId)
        if self.interrupted:
            self.sendStatus({'rc': 1})
        else:
            self.sendStatus({'rc': 0})
        self.d.callback(0)

registerSlaveCommand("dummy", DummyCommand, cvs_ver)


class SourceBase(Command):
    """Abstract base class for Version Control System operations (checkout
    and update). This class extracts the following arguments from the
    dictionary received from the master:

        - ['workdir']:  (required) the subdirectory where the buildable sources
                        should be placed

        - ['mode']:     one of update/copy/clobber/export, defaults to 'update'

        - ['revision']: If not None, this is an int or string which indicates
                        which sources (along a time-like axis) should be used.
                        It is the thing you provide as the CVS -r or -D
                        argument.

        - ['patch']:    If not None, this is a tuple of (striplevel, patch)
                        which contains a patch that should be applied after the
                        checkout has occurred. Once applied, the tree is no
                        longer eligible for use with mode='update', and it only
                        makes sense to use this in conjunction with a
                        ['revision'] argument. striplevel is an int, and patch
                        is a string in standard unified diff format. The patch
                        will be applied with 'patch -p%d <PATCH', with
                        STRIPLEVEL substituted as %d. The command will fail if
                        the patch process fails (rejected hunks).

        - ['timeout']:  seconds of silence tolerated before we kill of the
                        command
    """

    def setup(self, args):
        self.workdir = args['workdir']
        self.mode = args.get('mode', "update")
        self.revision = args.get('revision')
        self.patch = args.get('patch')
        self.timeout = args.get('timeout', 120)
        # VC-specific subclasses should override this to extract more args.
        # Make sure to upcall!

    def start(self):
        self.sendStatus({'header': "starting " + self.header + "\n"})
        self.command = None

        # self.srcdir is where the VC system should put the sources
        if self.mode == "copy":
            self.srcdir = "source" # hardwired directory name, sorry
        else:
            self.srcdir = self.workdir

        d = defer.succeed(None)
        # do we need to clobber anything?
        if self.mode in ("copy", "clobber", "export"):
            d.addCallback(self.doClobber, self.workdir)
        if not self.sourcedirIsUpdateable():
            # the directory cannot be updated, so we have to clobber it.
            # Perhaps the master just changed modes from 'export' to
            # 'update'.
            d.addCallback(self.doClobber, self.srcdir)

        d.addCallback(self.doVC)

        if self.mode == "copy":
            d.addCallback(self.doCopy)
        if self.patch:
            d.addCallback(self.doPatch)
        d.addCallbacks(self._sendRC, self._checkAbandoned)
        return d

    def interrupt(self):
        self.interrupted = True
        if self.command:
            self.command.kill("command interrupted")

    def doVC(self, res):
        if self.sourcedirIsUpdateable():
            d = self.doVCUpdate()
            d.addCallback(self.maybeDoVCFallback)
        else:
            d = self.doVCFull()
        d.addCallback(self._abandonOnFailure)
        return d

    def doVCUpdate(self):
        raise NotImplementedError("this must be implemented in a subclass")

    def doVCFull(self):
        raise NotImplementedError("this must be implemented in a subclass")

    def maybeDoVCFallback(self, rc):
        if type(rc) is int and rc == 0:
            return rc
        if self.interrupted:
            raise AbandonChain(1)
        msg = "update failed, clobbering and trying again"
        self.sendStatus({'header': msg + "\n"})
        log.msg(msg)
        d = self.doClobber(None, self.srcdir)
        d.addCallback(self.doVCFallback2)
        return d

    def doVCFallback2(self, res):
        msg = "now retrying VC operation"
        self.sendStatus({'header': msg + "\n"})
        log.msg(msg)
        d = self.doVCFull()
        d.addCallback(self._abandonOnFailure)
        return d

    def doClobber(self, dummy, dirname):
        # TODO: remove the old tree in the background
##         workdir = os.path.join(self.builder.basedir, self.workdir)
##         deaddir = self.workdir + ".deleting"
##         if os.path.isdir(workdir):
##             try:
##                 os.rename(workdir, deaddir)
##                 # might fail if deaddir already exists: previous deletion
##                 # hasn't finished yet
##                 # start the deletion in the background
##                 # TODO: there was a solaris/NetApp/NFS problem where a
##                 # process that was still running out of the directory we're
##                 # trying to delete could prevent the rm-rf from working. I
##                 # think it stalled the rm, but maybe it just died with
##                 # permission issues. Try to detect this.
##                 os.commands("rm -rf %s &" % deaddir)
##             except:
##                 # fall back to sequential delete-then-checkout
##                 pass
        d = os.path.join(self.builder.basedir, dirname)
        if runtime.platformType != "posix":
            # if we're running on w32, use rmtree instead. It will block,
            # but hopefully it won't take too long.
            shutil.rmtree(d, ignore_errors=1)
            return defer.succeed(0)
        command = ["rm", "-rf", d]
        c = ShellCommand(self.builder, command, self.builder.basedir,
                         sendRC=0, timeout=self.timeout)
        self.command = c
        # sendRC=0 means the rm command will send stdout/stderr to the
        # master, but not the rc=0 when it finishes. That job is left to
        # _sendRC
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        return d

    def doCopy(self, res):
        # now copy tree to workdir
        fromdir = os.path.join(self.builder.basedir, self.srcdir)
        todir = os.path.join(self.builder.basedir, self.workdir)
        if runtime.platformType != "posix":
            shutil.copytree(fromdir, todir)
            return defer.succeed(0)
        command = ['cp', '-r', '-p', fromdir, todir]
        c = ShellCommand(self.builder, command, self.builder.basedir,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        return d

    def doPatch(self, res):
        patchlevel, diff = self.patch
        command = ['patch', '-p%d' % patchlevel]
        dir = os.path.join(self.builder.basedir, self.workdir)
        # mark the directory so we don't try to update it later
        open(os.path.join(dir, ".buildbot-patched"), "w").write("patched\n")
        # now apply the patch
        c = ShellCommand(self.builder, command, dir,
                         sendRC=False, timeout=self.timeout,
                         stdin=diff)
        self.command = c
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        return d


class CVS(SourceBase):
    """CVS-specific VC operation. In addition to the arguments handled by
    SourceBase, this command reads the following keys:

    ['cvsroot'] (required): the CVSROOT repository string
    ['cvsmodule'] (required): the module to be retrieved
    ['branch']: a '-r' tag or branch name to use for the checkout/update
    ['login']: a string for use as a password to 'cvs login'
    ['global_options']: a list of strings to use before the CVS verb
    """

    header = "cvs operation"

    def setup(self, args):
        SourceBase.setup(self, args)
        self.cvsroot = args['cvsroot']
        self.cvsmodule = args['cvsmodule']
        self.global_options = args.get('global_options', [])
        self.branch = args.get('branch')
        self.login = args.get('login')

    def sourcedirIsUpdateable(self):
        if os.path.exists(os.path.join(self.builder.basedir,
                                       self.srcdir, ".buildbot-patched")):
            return False
        return os.path.isdir(os.path.join(self.builder.basedir,
                                          self.srcdir, "CVS"))

    def start(self):
        if self.login is not None:
            # need to do a 'cvs login' command first
            d = self.builder.basedir
            command = (['cvs', '-d', self.cvsroot] + self.global_options
                       + ['login'])
            c = ShellCommand(self.builder, command, d,
                             sendRC=False, timeout=self.timeout,
                             stdin=self.login+"\n")
            self.command = c
            d = c.start()
            d.addCallback(self._abandonOnFailure)
            d.addCallback(self._didLogin)
            return d
        else:
            return self._didLogin(None)

    def _didLogin(self, res):
        # now we really start
        return SourceBase.start(self)

    def doVCUpdate(self):
        d = os.path.join(self.builder.basedir, self.srcdir)
        command = ['cvs', '-z3'] + self.global_options + ['update', '-dP']
        if self.branch:
            command += ['-r', self.branch]
        if self.revision:
            command += ['-D', self.revision]
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

    def doVCFull(self):
        d = self.builder.basedir
        if self.mode == "export":
            verb = "export"
        else:
            verb = "checkout"
        command = (['cvs', '-d', self.cvsroot, '-z3'] +
                   self.global_options +
                   [verb, '-d', self.srcdir])
        if self.branch:
            command += ['-r', self.branch]
        if self.revision:
            command += ['-D', self.revision]
        command += [self.cvsmodule]
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

registerSlaveCommand("cvs", CVS, cvs_ver)

class SVN(SourceBase):
    """Subversion-specific VC operation. In addition to the arguments
    handled by SourceBase, this command reads the following keys:

    ['svnurl'] (required): the SVN repository string
    """

    header = "svn operation"

    def setup(self, args):
        SourceBase.setup(self, args)
        self.svnurl = args['svnurl']

    def sourcedirIsUpdateable(self):
        if os.path.exists(os.path.join(self.builder.basedir,
                                       self.srcdir, ".buildbot-patched")):
            return False
        return os.path.isdir(os.path.join(self.builder.basedir,
                                          self.srcdir, ".svn"))

    def doVCUpdate(self):
        revision = self.args['revision'] or 'HEAD'
        # update: possible for mode in ('copy', 'update')
        d = os.path.join(self.builder.basedir, self.srcdir)
        command = ['svn', 'update', '--revision', str(revision)]
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

    def doVCFull(self):
        revision = self.args['revision'] or 'HEAD'
        d = self.builder.basedir
        if self.mode == "export":
            command = ['svn', 'export', '--revision', str(revision),
                       self.svnurl, self.srcdir]
        else:
            # mode=='clobber', or copy/update on a broken workspace
            command = ['svn', 'checkout', '--revision', str(revision),
                       self.svnurl, self.srcdir]
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

registerSlaveCommand("svn", SVN, cvs_ver)

class Darcs(SourceBase):
    """Darcs-specific VC operation. In addition to the arguments
    handled by SourceBase, this command reads the following keys:

    ['repourl'] (required): the Darcs repository string
    """

    header = "darcs operation"

    def setup(self, args):
        SourceBase.setup(self, args)
        self.repourl = args['repourl']

    def sourcedirIsUpdateable(self):
        if os.path.exists(os.path.join(self.builder.basedir,
                                       self.srcdir, ".buildbot-patched")):
            return False
        return os.path.isdir(os.path.join(self.builder.basedir,
                                          self.srcdir, "_darcs"))

    def doVCUpdate(self):
        # TODO: revision
        # update: possible for mode in ('copy', 'update')
        d = os.path.join(self.builder.basedir, self.srcdir)
        command = ['darcs', 'pull', '--all', '--verbose']
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

    def doVCFull(self):
        # TODO: revision
        # checkout or export
        d = self.builder.basedir
        command = ['darcs', 'get', '--verbose', '--partial',
                   '--repo-name', self.srcdir,
                   self.repourl]
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

registerSlaveCommand("darcs", Darcs, cvs_ver)

class Arch(SourceBase):
    """Arch-specific (tla-specific) VC operation. In addition to the
    arguments handled by SourceBase, this command reads the following keys:

    ['url'] (required): the repository string
    ['version'] (required): which version (i.e. branch) to retrieve
    ['revision'] (optional): the 'patch-NN' argument to check out
    ['archive']: the archive name to use. If None, use the archive's default
    ['build-config']: if present, give to 'tla build-config' after checkout
    """

    arch_command = "tla"
    header = "arch operation"
    buildconfig = None

    def setup(self, args):
        SourceBase.setup(self, args)
        self.archive = args.get('archive')
        self.url = args['url']
        self.version = args['version']
        self.revision = args.get('revision')

    def sourcedirIsUpdateable(self):
        if os.path.exists(os.path.join(self.builder.basedir,
                                       self.srcdir, ".buildbot-patched")):
            return False
        return os.path.isdir(os.path.join(self.builder.basedir,
                                          self.srcdir, "{arch}"))

    def doVCUpdate(self):
        # update: possible for mode in ('copy', 'update')
        d = os.path.join(self.builder.basedir, self.srcdir)
        command = [self.arch_command, 'replay']
        if self.revision:
            command.append(self.revision)
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

    def doVCFull(self):
        # to do a checkout, we must first "register" the archive by giving
        # the URL to tla, which will go to the repository at that URL and
        # figure out the archive name. tla will tell you the archive name
        # when it is done, and all further actions must refer to this name.

        command = [self.arch_command, 'register-archive', '--force', self.url]
        c = ShellCommand(self.builder, command, self.builder.basedir,
                         sendRC=False, keepStdout=True,
                         timeout=self.timeout)
        self.command = c
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        d.addCallback(self._didRegister, c)
        return d

    def _didRegister(self, res, c):
        # find out what tla thinks the archive name is. If the user told us
        # to use something specific, make sure it matches.
        r = re.search(r'Registering archive: (\S+)\s*$', c.stdout)
        if r:
            msg = "tla reports archive name is '%s'" % r.group(1)
            log.msg(msg)
            self.builder.sendUpdate({'header': msg+"\n"})
            if self.archive and r.group(1) != self.archive:
                msg = (" mismatch, we wanted an archive named '%s'"
                       % self.archive)
                log.msg(msg)
                self.builder.sendUpdate({'header': msg+"\n"})
                raise AbandonChain(-1)
            self.archive = r.group(1)
        assert self.archive, "need archive name to continue"
        return self._doGet()

    def _doGet(self):
        ver = self.version
        if self.revision:
            ver += "--%s" % self.revision
        command = [self.arch_command, 'get', '--archive', self.archive,
                   '--no-pristine',
                   ver, self.srcdir]
        c = ShellCommand(self.builder, command, self.builder.basedir,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        if self.buildconfig:
            d.addCallback(self._didGet)
        return d

    def _didGet(self, res):
        d = os.path.join(self.builder.basedir, self.srcdir)
        command = [self.arch_command, 'build-config', self.buildconfig]
        c = ShellCommand(self.builder, command, d,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        return d

registerSlaveCommand("arch", Arch, cvs_ver)

class Bazaar(Arch):
    """Bazaar (/usr/bin/baz) is an alternative client for Arch repositories.
    It is mostly option-compatible, but archive registration is different
    enough to warrant a separate Command.

    ['archive'] (required): the name of the archive being used
    """

    arch_command = "baz"

    def setup(self, args):
        Arch.setup(self, args)
        # baz doesn't emit the repository name after registration (and
        # grepping through the output of 'baz archives' is too hard), so we
        # require that the buildmaster configuration to provide both the
        # archive name and the URL.
        self.archive = args['archive'] # required for Baz

    # in _didRegister, the regexp won't match, so we'll stick with the name
    # in self.archive

    def _doGet(self):
        # baz prefers ARCHIVE/VERSION. This will work even if
        # my-default-archive is not set.
        ver = self.archive + "/" + self.version
        if self.revision:
            ver += "--%s" % self.revision
        command = [self.arch_command, 'get', '--no-pristine',
                   ver, self.srcdir]
        c = ShellCommand(self.builder, command, self.builder.basedir,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        d = c.start()
        d.addCallback(self._abandonOnFailure)
        if self.buildconfig:
            d.addCallback(self._didGet)
        return d



registerSlaveCommand("bazaar", Bazaar, cvs_ver)

class P4Sync(SourceBase):
    """A partial P4 source-updater. Requires manual setup of a per-slave P4
    environment. The only thing which comes from the master is P4PORT.
    'mode' is required to be 'copy'.

    ['p4port'] (required): host:port to put in env['P4PORT']
    """

    header = "p4 sync"

    def setup(self, args):
        SourceBase.setup(self, args)
        self.p4port = args['p4port']
        
    def sourcedirIsUpdateable(self):
        return True

    def doVCUpdate(self):
        # TODO: revision
        d = os.path.join(self.builder.basedir, self.srcdir)
        command = ['p4', 'sync']
        env = {'P4PORT': self.p4port}
        c = ShellCommand(self.builder, command, d, environ=env,
                         sendRC=False, timeout=self.timeout)
        self.command = c
        return c.start()

    def doVCFull(self):
        return self.doVCUpdate()

registerSlaveCommand("p4sync", P4Sync, cvs_ver)
