#! /usr/bin/python

from twisted.python import log, components, failure
from twisted.spread import pb
from twisted.internet import reactor, defer

from buildbot import interfaces
from buildbot.status.progress import Expectations
from buildbot.status import builder
from buildbot.util import now
from buildbot.process import base

class Builder(pb.Referenceable):

    """I manage all Builds of a given type.

    Each Builder is created by an entry in the config file (the c['builders']
    list), with a number of parameters.

    One of these parameters is the L{buildbot.process.factory.BuildFactory}
    object that is associated with this Builder. The factory is responsible
    for creating new L{Build<buildbot.process.base.Build>} objects. Each
    Build object defines when and how the build is performed, so a new
    Factory or Builder should be defined to control this behavior.

    The Builder holds on to a number of these Build
    objects, in various slots like C{.waiting}, C{.interlocked},
    C{.buildable}, and C{.currentBuild}. Incoming
    L{Change<buildbot.change.changes.Change>} objects are passed to the
    C{.waiting} build, and when it decides it is ready to go, I move it to
    the C{.buildable} slot. When a slave becomes available, I move it to the
    C{.currentBuild} slot and start it running.

    The Builder is also the master-side representative for one of the
    L{buildbot.slave.bot.SlaveBuilder} objects that lives in a remote
    buildbot. When a remote builder connects, I query it for command versions
    and then make it available to any Builds that are ready to run.

    I also manage Interlocks, periodic build timers, forced builds, progress
    expectation (ETA) management, and some status delivery chores.

    @type waiting: L{buildbot.process.base.Build}
    @ivar waiting: a slot for a Build waiting for its 'tree stable' timer to
                   expire

    @type interlocked: list of L{buildbot.process.base.Build}
    @ivar interlocked: a slot for the Builds that are stable, but which must
                       wait for other Builds to complete successfully before
                       they can be run.

    @type buildable: L{buildbot.process.base.Build}
    @ivar buildable: a slot for a Build that is stable and ready to build,
                     but which is waiting for a buildslave to be available.

    @type currentBuild: L{buildbot.process.base.Build}
    @ivar currentBuild: a slot for the Build that actively running

    """

    remote = None
    lastChange = None
    buildNumber = 0
    periodicBuildTimer = None
    buildable = None
    currentBuild = None
    status = "idle"
    debug = False
    wantToStartBuild = None
    expectations = None # this is created the first time we get a good build

    def __init__(self, setup, builder_status):
        """
        @type  setup: dict
        @param setup: builder setup data, as stored in
                      BuildmasterConfig['builders'].  Contains name,
                      slavename, builddir, factory.
        @type  builder_status: L{buildbot.status.builder.BuilderStatus}
        """
        self.name = setup['name']
        self.slavename = setup['slavename']
        self.builddir = setup['builddir']
        self.buildFactory = setup['factory']

        self.periodicBuildTime = setup.get('periodicBuildTime', None)

        # build/wannabuild slots: Build objects move along this sequence
        self.waiting = self.newBuild()
        self.interlocked = []

        self.interlocks = [] # I watch these interlocks to know when to build
        self.feeders = [] # I feed these interlocks

        self.builder_status = builder_status
        self.builder_status.setSlavename(self.slavename)
        self.watchers = {'attach': [], 'detach': []}

    def setBotmaster(self, botmaster):
        self.botmaster = botmaster

    def compareToSetup(self, setup):
        diffs = []
        if setup['slavename'] != self.slavename:
            diffs.append('slavename changed from %s to %s' \
                         % (self.slavename, setup['slavename']))
        if setup['builddir'] != self.builddir:
            diffs.append('builddir changed from %s to %s' \
                         % (self.builddir, setup['builddir']))
        if setup['factory'] != self.buildFactory: # compare objects
            diffs.append('factory changed')
        if setup.get('periodicBuildTime', None) != self.periodicBuildTime:
            diffs.append('periodicBuildTime changed from %s to %s' \
                         % (self.periodicBuildTime,
                            setup.get('periodicBuildTime', None)))
        return diffs

    def newBuild(self):
        """
        Create a new build from our build factory and set ourself as the
        builder.

        @rtype: L{buildbot.process.base.Build}
        """
        b = self.buildFactory.newBuild()
        b.setBuilder(self)
        return b
        
    def watchInterlock(self, interlock):
        """This builder will wait for the given interlock to open up before
        it starts."""
        self.interlocks.append(interlock)
    def stopWatchingInterlock(self, interlock):
        self.interlocks.remove(interlock)

    def feedInterlock(self, interlock):
        """The following interlocks will be fed by this build."""
        self.feeders.append(interlock)
    def stopFeedingInterlock(self, interlock):
        self.feeders.remove(interlock)


    def __repr__(self):
        return "<Builder '%s'>" % self.name

    def __getstate__(self):
        d = self.__dict__.copy()
        d['remote'] = None
        d['currentBuild'] = None # XXX: failover to a new Build
        d['periodicBuildTimer'] = None
        return d
        
    def attached(self, remote, commands):
        """This is invoked by the BotPerspective when the self.slavename bot
        registers their builder.

        @rtype : L{twisted.internet.defer.Deferred}
        @return: a Deferred that fires (with 'self') when the slave-side
                 builder is fully attached and ready to accept commands.
        """
        if self.remote == remote:
            # already attached to them
            log.msg("Builder %s already attached" % self.name)
            return defer.succeed(self)
        if self.remote:
            log.msg("WEIRD", self.remote, remote)
        self.remote = remote
        self.remoteCommands = commands # maps command name to version
        log.msg("Builder %s attached" % self.name)
        d = self.remote.callRemote("setMaster", self)
        d.addErrback(self._attachFailure, "Builder.setMaster")
        d.addCallback(self._attached2)
        return d

    def _attachFailure(self, why, where):
        assert type(where) is str
        log.msg(where)
        log.err(why)

    def _attached2(self, res):
        d = self.remote.callRemote("print", "attached")
        d.addErrback(self._attachFailure, "Builder.print 'attached'")
        d.addCallback(self._attached3)
        return d

    def _attached3(self, res):
        # now we say they're really attached
        self.builder_status.addPointEvent(['connect'])
        if self.currentBuild:
            # XXX: handle interrupted build: flunk the current buildStep,
            # see if it can be restarted. buildStep.setBuilder(self) must be
            # done to allow it to run finishStep() when it is complete.
            log.msg("interrupted build!")
            pass
        self.startPeriodicBuildTimer()
        self.maybeStartBuild()
        for w in self.watchers['attach']:
            w.callback(self)
        self.watchers['attach'] = []
        return self

    def getSlaveCommandVersion(self, command, oldversion=None):
        if self.remoteCommands is None:
            # the slave is 0.5.0 or earlier
            return oldversion
        return self.remoteCommands.get(command)

    def detached(self):
        """This is called when the connection to the bot is lost."""
        log.msg("%s.detached" % self)
        self.remote = None
        reactor.callLater(0, self._detached)
        # the current step will be stopped (via a notifyOnDisconnect
        # callback), and the build will probably stop.

    def _detached(self):
        if self.currentBuild:
            log.msg("%s._detached: killing build" % self)
            # wasn't enough
            try:
                self.currentBuild.stopBuild("slave lost")
            except:
                log.msg("currentBuild.stopBuild failed")
                log.err()
            self.currentBuild = None
            # TODO: should failover to a new Build
        self.builder_status.addPointEvent(['disconnect'])
        self.builder_status.currentlyOffline()
        self.stopPeriodicBuildTimer()
        log.msg("Builder %s detached" % self.name)
        for w in self.watchers['detach']:
            w.callback(self)
        self.watchers['detach'] = []

    def updateBigStatus(self):
        if self.currentBuild:
            return # leave it alone
        if self.buildable and self.remote:
            log.msg("(self.buildable and self.remote) shouldn't happen")
            # maybeStartBuild should have moved this to self.currentBuild
            # before we get to see it
        elif self.buildable and not self.remote:
            # TODO: make this a big-status
            log.msg("want to start build, but we don't have a remote")
        if self.interlocked:
            # TODO: list all blocked interlocks
            self.builder_status.currentlyInterlocked(self.interlocks)
        elif self.waiting and self.waiting.nextBuildTime:
            self.builder_status.currentlyWaiting(self.waiting.nextBuildTime)
            # nextBuildTime == None means an interlock failed and the
            # changes were merged into the next build, but we don't know
            # when that will be. Call this state of affairs "idle"
        elif self.remote:
            self.builder_status.currentlyIdle()
        else:
            self.builder_status.currentlyOffline()

    def filesChanged(self, change):
        """
        Tell the waiting L{buildbot.process.base.Build} that files have
        changed.

        @type  change: L{buildbot.changes.changes.Change}
        """
        # this is invoked by the BotMaster to distribute change notification
        # we assume they are added in strictly increasing order
        if not self.waiting:
            self.waiting = self.newBuild()
        self.waiting.addChange(change)
        # eventually, our buildTimerFired() method will be called
        
    def buildTimerFired(self, wb):
        """
        Called by the Build when the build timer fires.

        @type  wb: L{buildbot.process.base.Build}
        @param wb: the waiting build that fires the timer
        """

        if not self.interlocks:
            # move from .waiting to .buildable
            if self.buildable:
                self.buildable.merge(wb)
            else:
                self.buildable = wb
            self.waiting = None
            self.maybeStartBuild()
            return
        # interlocked. Move from .waiting to .interlocked[]
        self.interlocked.append(wb)
        self.waiting = None
        # tell them to ask build interlock when they can proceed
        wb.checkInterlocks(self.interlocks)
        self.updateBigStatus()
        # if the interlocks are not blocked, interlockDone may be fired
        # inside checkInterlocks
        
    def interlockPassed(self, b):
        log.msg("%s: interlockPassed" % self)
        self.interlocked.remove(b)
        if self.buildable:
            self.buildable.merge(b)
        else:
            self.buildable = b
        self.maybeStartBuild()
    def interlockFailed(self, b):
        log.msg("%s: interlockFailed" % self)
        # who do we merge to?
        assert(self.interlocked[0] == b)
        self.interlocked.remove(b)
        if self.interlocked:
            target = self.interlocked[0]
        elif self.waiting:
            target = self.waiting
        else:
            self.waiting = self.newBuild()
            target = self.waiting
        target.failMerge(b)
        self.updateBigStatus()
        
    def startPeriodicBuildTimer(self):
        self.stopPeriodicBuildTimer()
        if self.periodicBuildTime:
            t = reactor.callLater(self.periodicBuildTime,
                                  self.doPeriodicBuild)
            self.periodicBuildTimer = t

    def stopPeriodicBuildTimer(self):
        if self.periodicBuildTimer:
            self.periodicBuildTimer.cancel()
            self.periodicBuildTimer = None

    def doPeriodicBuild(self):
        self.periodicBuildTimer = None
        self.forceBuild(None, "periodic build")
        self.startPeriodicBuildTimer()

    def maybeStartBuild(self):
        if self.currentBuild:
            return # must wait
        if not self.buildable:
            self.updateBigStatus()
            return # nothing to do
        if not self.remote:
            #log.msg("want to start build, but we don't have a remote")
            self.updateBigStatus()
            return
        # move to .building, start it
        self.currentBuild = self.buildable
        self.buildable = None
        return self.startBuild(self.currentBuild)

    def startBuild(self, build):
        log.msg("starting build %s" % build)
        d = self.remote.callRemote("startBuild") # informational courtesy
        d.addErrback(self._startBuildFailed, build)

        # create the BuildStatus object that goes with the Build
        bs = self.builder_status.newBuild()

        # start the build. This will first set up the steps, then tell the
        # BuildStatus that it has started, which will announce it to the
        # world (through our BuilderStatus object, which is its parent).
        # Finally it will start the actual build process.
        d = build.startBuild(bs, self.expectations, self.remote)
        d.addCallback(self.buildFinished)
        d.addErrback(self._buildNotFinished)
        control = base.BuildControl(build)
        return control

    def _buildNotFinished(self, why):
        log.msg("_buildNotFinished")
        log.err()

    def _startBuildFailed(self, why, build):
        log.msg("I tried to tell the slave that the build %s started, but "
                "remote_startBuild failed: %s" % (build, why))

    def testsFinished(self, results):
        # XXX: add build number, datestamp, Change information
        #self.testTracker.testsFinished(results)
        pass
        
    def buildFinished(self, build):
        self.currentBuild = None
        for f in self.feeders:
            f.buildFinished(self.name, build.maxChangeNumber,
                            (build.results == builder.SUCCESS))
        self.maybeStartBuild()
        return build.results # give to whoever started the build

    def setExpectations(self, progress):
        """Mark the build as successful and update expectations for the next
        build. Only call this when the build did not fail in any way that
        would invalidate the time expectations generated by it. (if the
        compile failed and thus terminated early, we can't use the last
        build to predict how long the next one will take).
        """
        if self.expectations:
            self.expectations.update(progress)
        else:
            # the first time we get a good build, create our Expectations
            # based upon its results
            self.expectations = Expectations(progress)
        log.msg("new expectations: %s seconds" % \
                self.expectations.expectedBuildTime())

    def forceBuild(self, who, reason):
        # only add a build if there isn't anything already building
        if self.currentBuild:
            log.msg(self,
                    "forceBuild(%s,%s) ignored because a build is running" % \
                    (who, reason))
            raise interfaces.BuilderInUseError
        if not self.remote:
            log.msg(self,
                    "forceBuild(%s,%s) ignored because we have no slave" % \
                    (who, reason))
            raise interfaces.NoSlaveError
        if self.buildable:
            self.buildable.reason = reason
        else:
            self.buildable = self.newBuild()
            self.buildable.reason = reason
        return self.maybeStartBuild()

    def shutdownSlave(self):
        if self.remote:
            self.remote.callRemote("shutdown")
            

class Ping:
    def ping(self, status, remote, timeout):
        if not remote:
            status.addPointEvent(["ping", "no slave"], "red")
            return defer.succeed(False) # interfaces.NoSlaveError
        self.event = status.addEvent(["pinging"], "yellow")
        self.active = True
        self.d = defer.Deferred()
        d = remote.callRemote("print", "ping")
        d.addBoth(self._pong)

        # We use either our own timeout or the (long) TCP timeout to detect
        # silently-missing slaves. This might happen because of a NAT
        # timeout or a routing loop. If the slave just shuts down (and we
        # somehow missed the FIN), we should get a "connection refused"
        # message.
        self.timer = reactor.callLater(timeout, self.timeout)
        return self.d

    def timeout(self):
        self.timer = None
        self._pong(failure.Failure(interfaces.NoSlaveError("timeout")))

    def _pong(self, res):
        if not self.active:
            return
        self.active = False
        if self.timer:
            self.timer.cancel()
        e = self.event
        if isinstance(res, failure.Failure):
            e.text = ["ping", "failed"]
            e.color = "red"
            ponged = False
            # TODO: force the BotPerspective to disconnect, since this
            # indicates that the bot is unreachable. That will also append a
            # "disconnect" event to the builder_status, terminating this
            # "ping failed" event.
        else:
            e.text = ["ping", "success"]
            e.color = "green"
            ponged = True
        e.finish()
        self.d.callback(ponged)

class BuilderControl(components.Adapter):
    __implements__ = interfaces.IBuilderControl,
    def forceBuild(self, who, reason):
        bc = self.original.forceBuild(who, reason)
        return bc

    def getBuild(self, number):
        b =  self.original.currentBuild
        if b and b.build_status.number == number:
            return base.BuildControl(b)
        return None

    def ping(self, timeout=30):
        d = Ping().ping(self.original.builder_status,
                        self.original.remote, timeout)
        return d

components.registerAdapter(BuilderControl, Builder, interfaces.IBuilderControl)
