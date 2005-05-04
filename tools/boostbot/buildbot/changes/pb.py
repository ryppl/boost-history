# -*- test-case-name: buildbot.test.test_changes -*-

import os, os.path

from twisted.application import service

from buildbot.pbutil import NewCredPerspective
from buildbot.changes.changes import Change
from buildbot.interfaces import IChangeSource

class ChangePerspective(NewCredPerspective):

    def __init__(self, changemaster, prefix, sep="/"):
        self.changemaster = changemaster
        self.prefix = prefix
        # this is the separator as used by the VC system, not the local host.
        # If for some reason you're running your CVS repository under
        # windows, you'll need to use a PBChangeSource(sep="\\")
        self.sep = sep

    def attached(self, mind):
        return self
    def detached(self, mind):
        pass
    def perspective_addChange(self, changedict):
        pathnames = []
        for path in changedict['files']:
            if self.prefix:
                bits = path.split(self.sep)
                if bits[0] == self.prefix:
                    if bits[1:]:
                        path = self.sep.join(bits[1:])
                    else:
                        path = ''
                else:
                    break
            pathnames.append(path)

        if pathnames:
            change = Change(changedict['who'],
                            pathnames,
                            changedict['comments'],
                            revision=changedict.get('revision'))
            self.changemaster.addChange(change)

class PBChangeSource(service.Service):
    __implements__ = IChangeSource, service.Service.__implements__

    def __init__(self, user="change", passwd="changepw", port=None,
                 prefix=None, sep="/"):
        # TODO: current limitations
        assert user == "change"
        assert passwd == "changepw"
        assert port == None
        self.user = user
        self.passwd = passwd
        self.port = port
        self.prefix = prefix
        self.sep = sep

    def describe(self):
        # TODO: when the dispatcher is fixed, report the specific port
        #d = "PB listener on port %d" % self.port
        d = "PBChangeSource listener on all-purpose slaveport"
        if self.prefix is not None:
            d += " (prefix '%s')" % self.prefix
        return d

    def startService(self):
        service.Service.startService(self)
        # our parent is the ChangeMaster object
        # find the master's Dispatch object and register our username
        # TODO: the passwd should be registered here too
        master = self.parent.parent
        master.dispatcher.register(self.user, self)

    def stopService(self):
        service.Service.stopService(self)
        # unregister our username
        master = self.parent.parent
        master.dispatcher.unregister(self.user)

    def getPerspective(self):
        return ChangePerspective(self.parent, self.prefix, self.sep)

