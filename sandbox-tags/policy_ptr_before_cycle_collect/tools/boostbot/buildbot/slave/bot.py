#! /usr/bin/python

import time, os, os.path, re, sys

from twisted.spread import pb
from twisted.python import log, usage, failure
from twisted.internet import reactor, defer
from twisted.application import service, internet
from twisted.cred import credentials

from buildbot.util import now
from buildbot.pbutil import ReconnectingPBClientFactory
from buildbot.slave import registry
# make sure the standard commands get registered
from buildbot.slave import commands

class NoCommandRunning(pb.Error):
    pass
class WrongCommandRunning(pb.Error):
    pass
class UnknownCommand(pb.Error):
    pass

class Master:
    def __init__(self, host, port, username, password):
        self.host = host
        self.port = port
        self.username = username
        self.password = password

class SlaveBuild:

    """This is an object that can hold state from one step to another in the
    same build. All SlaveCommands have access to it.
    """
    def __init__(self, builder):
        self.builder = builder
    
class SlaveBuilder(pb.Referenceable, service.Service):

    """This is the local representation of a single Builder: it handles a
    single kind of build (like an all-warnings build). It has a name and a
    home directory. The rest of its behavior is determined by the master.
    """

    stopCommandOnShutdown = True

    # remote is a ref to the Builder object on the master side, and is set
    # when they attach. We use it to detect when the connection to the master
    # is severed.
    remote = None

    # .build points to a SlaveBuild object, a new one for each build
    build = None

    # .command points to a SlaveCommand instance, and is set while the step
    # is running. We use it to implement the stopBuild method.
    command = None

    # .remoteStep is a ref to the master-side BuildStep object, and is set
    # when the step is started
    remoteStep = None

    def __init__(self, parent, name, not_really):
        #service.Service.__init__(self)
        self.name = name
        self.not_really = not_really
        
    def __repr__(self):
        return "<SlaveBuilder '%s'>" % self.name

    def setServiceParent(self, parent):
        service.Service.setServiceParent(self, parent)
        self.bot = self.parent

    def setBuilddir(self, builddir):
        assert self.parent
        self.builddir = builddir
        self.basedir = os.path.join(self.bot.basedir, self.builddir)
        if not os.path.isdir(self.basedir):
            os.mkdir(self.basedir)

    def stopService(self):
        service.Service.stopService(self)
        if self.stopCommandOnShutdown:
            self.stopCommand()

    def activity(self):
        #bot = self.parent
        #buildslave = bot.parent
        #bf = buildslave.bf
        self.parent.parent.bf.activity()

    def remote_setMaster(self, remote):
        self.remote = remote
        self.remote.notifyOnDisconnect(self.lostRemote)
    def remote_print(self, message):
        log.msg("builder '%s' message from master:" % self.name, message)

    def lostRemote(self, remote):
        log.msg("lost remote")
        self.remote = None

    def lostRemoteStep(self, remotestep):
        log.msg("lost remote step")
        self.remoteStep = None
        if self.stopCommandOnShutdown:
            self.stopCommand()
        
    # the following are Commands that can be invoked by the master-side
    # Builder
    def remote_startBuild(self):
        """This is invoked before the first step of any new build is run. It
        creates a new SlaveBuild object, which holds slave-side state from
        one step to the next."""
        self.build = SlaveBuild(self)
        log.msg("startBuild")

    def remote_startCommand(self, stepref, stepId, command, args):
        """
        This gets invoked by L{buildbot.process.step.RemoteCommand.start}, as
        part of various master-side BuildSteps, to start various commands
        that actually do the build. I return nothing. Eventually I will call
        .commandComplete() to notify the master-side RemoteCommand that I'm
        done.
        """

        self.activity()

        if self.command:
            log.msg("leftover command, dropping it")
            self.stopCommand()

        try:
            factory, version = registry.commandRegistry[command]
        except KeyError:
            raise UnknownCommand, "unrecognized SlaveCommand '%s'" % command
        self.command = factory(self, stepId, args)

        log.msg(" startCommand:%s [id %s]" % (command,stepId))
        self.remoteStep = stepref
        self.remoteStep.notifyOnDisconnect(self.lostRemoteStep)
        self.command.running = True
        d = defer.maybeDeferred(self.command.start)
        d.addCallback(lambda res: None)
        d.addBoth(self.commandComplete)
        return None

    def remote_interruptCommand(self, stepId, why):
        """Halt the current step."""
        log.msg("asked to interrupt current command: %s" % why)
        self.activity()
        if not self.command:
            # TODO: just log it, a race could result in their interrupting a
            # command that wasn't actually running
            log.msg(" .. but none was running")
            return
        self.command.interrupt()


    def stopCommand(self):
        """Make any currently-running command die, with no further status
        output. This is used when the buildslave is shutting down or the
        connection to the master has been lost. Interrupt the command,
        silence it, and then forget about it."""
        if not self.command:
            return
        log.msg("stopCommand: halting current command %s" % self.command)
        self.command.running = False # shut up!
        self.command.interrupt() # die!
        self.command = None # forget you!

    # sendUpdate is invoked by the Commands we spawn
    def sendUpdate(self, data):
        """This sends the status update to the master-side
        L{buildbot.process.step.RemoteCommand} object, giving it a sequence
        number in the process. It adds the update to a queue, and asks the
        master to acknowledge the update so it can be removed from that
        queue."""

        if not self.running:
            # .running comes from service.Service, and says whether the
            # service is running or not. If we aren't running, don't send any
            # status messages.
            return
        # the update[1]=0 comes from the leftover 'updateNum', which the
        # master still expects to receive. Provide it to avoid significant
        # interoperability issues between new slaves and old masters.
        if self.remoteStep:
            update = [data, 0]
            updates = [update]
            d = self.remoteStep.callRemote("update", updates)
            d.addCallback(self.ackUpdate)
            d.addErrback(self._ackFailed, "SlaveBuilder.sendUpdate")

    def ackUpdate(self, acknum):
        self.activity() # update the "last activity" timer

    def ackComplete(self, dummy):
        self.activity() # update the "last activity" timer

    def _ackFailed(self, why, where):
        log.msg("SlaveBuilder._ackFailed:", where)
        #log.err(why) # we don't really care


    # this is fired by the Deferred attached to each Command
    def commandComplete(self, failure):
        if failure:
            log.msg("SlaveBuilder.commandFailed", self.command)
            log.err(failure)
            # failure, if present, is a failure.Failure. To send it across
            # the wire, we must turn it into a pb.CopyableFailure.
            failure = pb.CopyableFailure(failure)
            failure.unsafeTracebacks = True
        else:
            # failure is None
            log.msg("SlaveBuilder.commandComplete", self.command)
        self.command = None
        if not self.running:
            return
        if self.remoteStep:
            self.remoteStep.dontNotifyOnDisconnect(self.lostRemoteStep)
            d = self.remoteStep.callRemote("complete", failure)
            d.addCallback(self.ackComplete)
            d.addErrback(self._ackFailed, "sendComplete")
            self.remoteStep = None


    def remote_shutdown(self):
        print "slave shutting down on command from master"
        reactor.stop()
        
        
class Bot(pb.Referenceable, service.MultiService):
    """I represent the slave-side bot."""
    usePTY = None
    name = "bot"

    def __init__(self, basedir, usePTY, not_really=0):
        service.MultiService.__init__(self)
        self.basedir = basedir
        self.usePTY = usePTY
        self.not_really = not_really
        self.builders = {}

    def startService(self):
        assert os.path.isdir(self.basedir)
        service.MultiService.startService(self)

    def remote_getDirs(self):
        return filter(lambda d: os.path.isdir(d), os.listdir(self.basedir))

    def remote_getCommands(self):
        commands = {}
        for name, (factory, version) in registry.commandRegistry.items():
            commands[name] = version
        return commands

    def remote_setBuilderList(self, wanted):
        retval = {}
        for (name, builddir) in wanted:
            b = self.builders.get(name, None)
            if b:
                if b.builddir != builddir:
                    log.msg("changing builddir for builder %s from %s to %s" \
                            % (name, b.builddir, builddir))
                    b.setBuilddir(builddir)
            else:
                b = SlaveBuilder(self, name, self.not_really)
                b.usePTY = self.usePTY
                b.setServiceParent(self)
                b.setBuilddir(builddir)
                self.builders[name] = b
            retval[name] = b
        for name in self.builders.keys():
            if not name in map(lambda a: a[0], wanted):
                log.msg("removing old builder %s" % name)
                self.builders[name].disownServiceParent()
                del(self.builders[name])
        return retval
    
    def remote_print(self, message):
        log.msg("message from master:", message)

    def remote_getSlaveInfo(self):
        """This command retrieves data from the files in SLAVEDIR/info/* and
        sends the contents to the buildmaster. These are used to describe
        the slave and its configuration, and should be created and
        maintained by the slave administrator. They will be retrieved each
        time the master-slave connection is established.
        """

        files = {}
        basedir = os.path.join(self.basedir, "info")
        if not os.path.isdir(basedir):
            return files
        for f in os.listdir(basedir):
            filename = os.path.join(basedir, f)
            if os.path.isfile(filename):
                files[f] = open(filename, "r").read()
        return files

    def debug_forceBuild(self, name):
        d = self.perspective.callRemote("forceBuild", name)
        d.addCallbacks(log.msg, log.err)

class BotFactory(ReconnectingPBClientFactory):
    # 'keepaliveInterval' serves two purposes. The first is to keep the
    # connection alive: it guarantees that there will be at least some
    # traffic once every 'keepaliveInterval' seconds, which may help keep an
    # interposed NAT gateway from dropping the address mapping because it
    # thinks the connection has been abandoned. The second is to put an upper
    # limit on how long the buildmaster might have gone away before we notice
    # it. For this second purpose, we insist upon seeing *some* evidence of
    # the buildmaster at least once every 'keepaliveInterval' seconds.
    keepaliveInterval = None # None = do not use keepalives

    # 'keepaliveTimeout' seconds before the interval expires, we will send a
    # keepalive request, both to add some traffic to the connection, and to
    # prompt a response from the master in case all our builders are idle. We
    # don't insist upon receiving a timely response from this message: a slow
    # link might put the request at the wrong end of a large build message.
    keepaliveTimeout = 30 # how long we will go without a response

    keepaliveTimer = None
    activityTimer = None
    lastActivity = 0
    unsafeTracebacks = 1
    perspective = None

    def __init__(self, keepaliveInterval, keepaliveTimeout):
        ReconnectingPBClientFactory.__init__(self)
        self.keepaliveInterval = keepaliveInterval
        self.keepaliveTimeout = keepaliveTimeout

    def startedConnecting(self, connector):
        ReconnectingPBClientFactory.startedConnecting(self, connector)
        self.connector = connector

    def gotPerspective(self, perspective):
        ReconnectingPBClientFactory.gotPerspective(self, perspective)
        self.perspective = perspective
        try:
            perspective.broker.transport.setTcpKeepAlive(1)
        except:
            log.msg("unable to set SO_KEEPALIVE")
            if not self.keepaliveInterval:
                self.keepaliveInterval = 10*60
        self.activity()
        if self.keepaliveInterval:
            log.msg("sending application-level keepalives every %d seconds" \
                    % self.keepaliveInterval)
            self.startTimers()

    def clientConnectionFailed(self, connector, reason):
        self.connector = None
        ReconnectingPBClientFactory.clientConnectionFailed(self,
                                                           connector, reason)

    def clientConnectionLost(self, connector, reason):
        self.connector = None
        self.stopTimers()
        self.perspective = None
        ReconnectingPBClientFactory.clientConnectionLost(self,
                                                         connector, reason)

    def startTimers(self):
        assert self.keepaliveInterval
        assert not self.keepaliveTimer
        assert not self.activityTimer
        # Insist that doKeepalive fires before checkActivity. Really, it
        # needs to happen at least one RTT beforehand.
        assert self.keepaliveInterval > self.keepaliveTimeout

        # arrange to send a keepalive a little while before our deadline
        when = self.keepaliveInterval - self.keepaliveTimeout
        self.keepaliveTimer = reactor.callLater(when, self.doKeepalive)
        # and check for activity too
        self.activityTimer = reactor.callLater(self.keepaliveInterval,
                                               self.checkActivity)

    def stopTimers(self):
        if self.keepaliveTimer:
            self.keepaliveTimer.cancel()
            self.keepaliveTimer = None
        if self.activityTimer:
            self.activityTimer.cancel()
            self.activityTimer = None

    def activity(self, res=None):
        self.lastActivity = now()

    def doKeepalive(self):
        # send the keepalive request. If it fails outright, the connection
        # was already dropped, so just log and ignore.
        self.keepaliveTimer = None
        log.msg("sending app-level keepalive")
        d = self.perspective.callRemote("keepalive")
        d.addCallback(self.activity)
        d.addErrback(self.keepaliveLost)

    def keepaliveLost(self, f):
        log.msg("BotFactory.keepaliveLost")

    def checkActivity(self):
        self.activityTimer = None
        if self.lastActivity + self.keepaliveInterval < now():
            log.msg("BotFactory.checkActivity: nothing from master for "
                    "%d secs" % (now() - self.lastActivity))
            self.perspective.broker.transport.loseConnection()
            return
        self.startTimers()

    def stopFactory(self):
        ReconnectingPBClientFactory.stopFactory(self)
        self.stopTimers()


class BuildSlave(service.MultiService):
    botClass = Bot

    def __init__(self, host, port, name, passwd, basedir, keepalive,
                 usePTY, keepaliveTimeout=30):
        service.MultiService.__init__(self)
        bot = self.botClass(basedir, usePTY)
        bot.setServiceParent(self)
        self.bot = bot
        if keepalive == 0:
            keepalive = None
        bf = self.bf = BotFactory(keepalive, keepaliveTimeout)
        bf.startLogin(credentials.UsernamePassword(name, passwd), client=bot)
        self.connection = c = internet.TCPClient(host, port, bf)
        c.setServiceParent(self)

    def waitUntilDisconnected(self):
        # utility method for testing. Returns a Deferred that will fire when
        # we lose the connection to the master.
        if not self.bf.perspective:
            return defer.succeed(None)
        d = defer.Deferred()
        self.bf.perspective.notifyOnDisconnect(lambda res: d.callback(None))
        return d

    def stopService(self):
        self.bf.continueTrying = 0
        service.MultiService.stopService(self)
        # now kill the TCP connection
        self.connection._connection.disconnect()

class Options(usage.Options):
    synopsis = "Usage: mktap buildbot slave --name <name> --passwd <passwd> [options]"
    
    longdesc = """
    This command creates a buildslave .tap file. The bot will use the <name>
    and <passwd> arguments to authenticate itself when connecting to the
    master. All commands are run in a build-specific subdirectory of
    <basedir>, which defaults to the working directory that mktap was run
    from. <master> is a string of the form 'hostname:port', and specifies
    where the buildmaster can be reached.

    <name>, <passwd>, and <master> will be provided by the buildmaster
    administrator for your bot.
    """
    
    optParameters = [
        ["name", "n", None, "Name for this build slave"],
        ["passwd", "p", None, "Password for this build slave"],
        ["basedir", "d", ".", "Base directory to use"],
        ["master", "m", "localhost:8007",
         "Location of the buildmaster (host:port)"],
        ["keepalive", "k", 0,
         "Interval at which keepalives should be sent (in seconds)"],
        ["usepty", None, 1, "child processes should be run in a pty"],
        ]

    def abort(self):
        print self.__str__()
        sys.exit(2)
        
    def postOptions(self):
        problems = 0
        if self['name'] == None:
            print " Must specify buildslave name with '--name myslave'"
            problems = 1
        if self['passwd'] == None:
            print " Must specify slave password with '--passwd myslave'"
            problems = 1
        self['usepty'] = int(self['usepty'])
        self['keepalive'] = int(self['keepalive'])
        if problems:
            print
            self.abort()
        
    
def makeService(config):
    basedir = config['basedir']
    master = config['master']

    if not os.path.isdir(basedir):
        raise "bot homedir '%s' is not a directory" % basedir
    basedir = os.path.abspath(basedir)
    print " Using '%s' for slave base directory" % basedir
    try:
        host, port = re.search(r'(.+):(\d+)', master).groups()
        port = int(port)
    except:
        print "unparseable master location '%s'" % master
        print " expecting something more like localhost:8007"
        raise
    print " Will connect to buildmaster on host '%s' at port '%d'" % (host,
                                                                      port)
    if config['usepty']:
        print " Will run child processes in PTYs"
    else:
        print " Will NOT run child processes in PTYs"

    s = BuildSlave(host, port,
                   config['name'], config['passwd'], basedir,
                   config['keepalive'], config['usepty'])
    s.setName("buildslave")
    return s
