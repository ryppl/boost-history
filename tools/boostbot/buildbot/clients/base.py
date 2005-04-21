#! /usr/bin/python

import sys, re

from twisted.spread import pb
from twisted.cred import credentials
from twisted.internet import reactor

from buildbot.pbutil import ReconnectingPBClientFactory

from buildbot.status import event

class UpdatingEvent(event.RemoteEvent):
    def observe_setFiles(self, filesdict):
        event.RemoteEvent.observe_setFiles(self, filesdict)
        pass
    def observe_update(self, **kwargs):
        event.RemoteEvent.observe_update(self, **kwargs)
        print "%s: update: %s" % (self.builder.name, kwargs)
    def observe_addFile(self, name, file):
        event.RemoteEvent.observe_addFile(self, name, file)
        pass
    def observe_finish(self):
        event.RemoteEvent.observe_finish(self)
        pass
pb.setUnjellyableForClass(event.Event, UpdatingEvent)

from twisted.python import log
#log.startLogging(sys.stdout)

class Builder(pb.Referenceable):
    def __init__(self, name, remote, parent):
        self.name = name
        self.remote = remote
        self.parent = parent
        self.current = None
        remote.notifyOnDisconnect(self.disconnect)
        self.setup()
    def setup(self):
        pass
    def subscribe(self):
        self.remote.callRemote("subscribe", self)
        # after this, we will get all methods except appendFiles
        #self.remote.callRemote("subscribeFiles")
    def goAway(self):
        self.unsubscribe()
    def disconnect(self, remote):
        self.parent.removeBuilder(self.name)
    def unsubscribe(self):
        self.remote.callRemote("unsubscribeFiles")
        self.remote.callRemote("unsubscribe")
        self.remote.dontNotifyOnDisconnect(self.disconnect)

    # last-build-status
    def remote_newLastBuildStatus(self, event):
        print "%s: newLastBuildStatus: %s" % (self.name, event)

    # current-activity-big
    def remote_currentlyOffline(self):
        print "%s: big=offline" % self.name
    def remote_currentlyIdle(self):
        print "%s: big=idle" % self.name
    def remote_currentlyWaiting(self, seconds):
        print "%s: big=waiting" % self.name, seconds
    def remote_currentlyInterlocked(self):
        print "%s: big=interlocked" % self.name
    def remote_currentlyBuilding(self, eta):
        print "%s: big=building" % self.name, eta
        if eta:
            d = eta.callRemote("subscribe", self, 1)
        # TODO: addCallback
        # results in _progress, _finished messages

    # current-activity-small
    def remote_newEvent(self, event):
        assert(event.__class__ == UpdatingEvent)
        print "%s: newEvent: %s" % (self.name, event)
        self.current = event
        event.builder = self

    # from the BuildProgress object
    def remote_progress(self, seconds):
        print "%s: progress: %s" % (self.name, seconds)
    def remote_finished(self, eta):
        print "%s: finished" % self.name
        eta.callRemote("unsubscribe", self)

class Client(pb.Referenceable):
    """To use this, call my .connected method with a RemoteReference to the
    buildmaster's status port object.
    """
    BuilderClass = Builder

    def __init__(self):
        self.builders = {}

    def done(*args):
        reactor.stop()

    def addBuilder(self, name, builder):
        self.builders[name] = builder
        #builder.subscribe()
    def removeBuilder(self, name):
        del self.builders[name]

    def remote_builderAdded(self, buildername, builder):
        # get get a remote interface to an IBuilderStatus
        #b = self.BuilderClass(name, remote, self)
        print "builderAdded", buildername
        #self.addBuilder(name, b)
        #return b

    def remote_builderRemoved(self, buildername):
        print "builderRemoved", buildername
        #self.builders[name].goAway()
        #self.removeBuilder(name)

    def remote_builderChangedState(self, buildername, state, eta):
        print "builderChangedState", buildername, state, eta

    def remote_buildStarted(self, buildername, build):
        print "buildStarted", buildername

    def remote_buildFinished(self, buildername, build, results):
        print "buildFinished", results

    def remote_buildETAUpdate(self, buildername, build, eta):
        print "ETA", buildername, eta

    def remote_stepStarted(self, buildername, build, stepname, step):
        print "stepStarted", buildername, stepname

    def remote_stepFinished(self, buildername, build, stepname, step, results):
        print "stepFinished", buildername, stepname, results

    def remote_stepETAUpdate(self, buildername, build, stepname, step,
                             eta, expectations):
        print "stepETA", buildername, stepname, eta

    def remote_logStarted(self, buildername, build, stepname, step,
                          logname, log):
        print "logStarted", buildername, stepname

    def remote_logFinished(self, buildername, build, stepname, step,
                           logname, log):
        print "logFinished", buildername, stepname

    def remote_logChunk(self, buildername, build, stepname, step, logname, log,
                        channel, text):
        ChunkTypes = ["STDOUT", "STDERR", "HEADER"]
        print "logChunk[%s]: %s" % (ChunkTypes[channel], text)

    def connected(self, remote):
        print "connected"
        self.remote = remote
        remote.callRemote("subscribe", "logs", 3, self)

    def startConnecting(self, master):
        try:
            host, port = re.search(r'(.+):(\d+)', master).groups()
            port = int(port)
        except:
            print "unparseable master location '%s'" % master
            print " expecting something more like localhost:8007"
            raise
        cf = ClientFactory()
        cf.client = self
        cf.startLogin(credentials.UsernamePassword("statusClient",
                                                   "clientpw"))
        reactor.connectTCP(host, port, cf)


class ClientFactory(ReconnectingPBClientFactory):
    def gotPerspective(self, perspective):
        self.client.connected(perspective)

def main():
    master = "localhost:8007"
    if len(sys.argv) > 1:
        master = sys.argv[1]
    c = Client()
    c.startConnecting(master)
    reactor.run()

if __name__ == '__main__':
    main()
