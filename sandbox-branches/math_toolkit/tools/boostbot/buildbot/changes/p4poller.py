#! /usr/bin/python

# Many thanks to Dave Peticolas for contributing this module

from twisted.application import service
from twisted.internet import defer
from twisted.internet.utils import getProcessOutput
from twisted.internet.task import LoopingCall

from buildbot import util
from buildbot.interfaces import IChangeSource
from buildbot.changes.changes import Change

class P4Source(service.Service, util.ComparableMixin):
    """This source will poll a perforce repository for changes and submit
    them to the change master."""

    __implements__ = IChangeSource, service.Service.__implements__
    compare_attrs = ["p4port", "p4user", "p4client", "p4base",
                     "p4bin", "pollinterval", "histmax"]

    parent = None # filled in when we're added
    last_change = None
    loop = None
    volatile = ['loop']

    def __init__(self, p4port, p4user, p4client, p4base,
                 p4bin='p4', pollinterval=60 * 10, histmax=100):
        """
        @type  p4port:       string
        @param p4port:       p4 port definition (host:portno)
        @type  p4user:       string
        @param p4user:       p4 user
        @type  p4client:     string
        @param p4client:     name of p4 client to poll
        @type  p4base:       string
        @param p4base:       p4 file specification to limit a poll to
                             (i.e., //...)
        @type  p4bin:        string
        @param p4bin:        path to p4 binary, defaults to just 'p4'
        @type  pollinterval: int
        @param pollinterval: interval in seconds between polls
        @type  histmax:      int
        @param histmax:      maximum number of changes to look back through
        """

        self.p4port = p4port
        self.p4user = p4user
        self.p4client = p4client
        self.p4base = p4base
        self.p4bin = p4bin
        self.pollinterval = pollinterval
        self.histmax = histmax

    def startService(self):
        self.loop = LoopingCall(self.checkp4)
        self.loop.start(self.pollinterval)
        service.Service.startService(self)

    def stopService(self):
        self.loop.stop()
        return service.Service.stopService(self)

    def describe(self):
        return "p4source %s:%s %s" % (self.p4port, self.p4client, self.p4base)

    def checkp4(self):
        d = self._get_changes()
        d.addCallback(self._process_changes)
        d.addCallback(self._handle_changes)

    def _get_changes(self):
        args = ['changes', '-m', str(self.histmax), self.p4base]
        env = {'P4PORT' : self.p4port,
               'P4USER' : self.p4user,
               'P4CLIENT' : self.p4client}
        return getProcessOutput(self.p4bin, args, env)

    def _process_changes(self, result):
        last_change = self.last_change
        changelists = []
        for line in result.split('\n'):
            line = line.strip()
            if not line: continue
            _, num, _, date, _, user, _ = line.split(' ', 6)
            if last_change is None:
                self.last_change = num
                return []
            if last_change == num: break
            change = {'num' : num, 'date' : date, 'user' : user.split('@')[0]}
            changelists.append(change)
        changelists.reverse() # oldest first
        ds = [self._get_change(c) for c in changelists]
        return defer.DeferredList(ds)

    def _get_change(self, change):
        args = ['describe', '-s', change['num']]
        env = {'P4PORT' : self.p4port,
               'P4CLIENT' : self.p4client}
        d = getProcessOutput(self.p4bin, args, env)
        d.addCallback(self._process_change, change)
        return d

    def _process_change(self, result, change):
        lines = result.split('\n')
        comments = ''
        while not lines[0].startswith('Affected files'):
            comments += lines.pop(0) + '\n'
        change['comments'] = comments
        lines.pop(0) # affected files
        files = []
        while lines:
            line = lines.pop(0).strip()
            if not line: continue
            files.append(line.split(' ')[1])
        change['files'] = files
        return change

    def _handle_changes(self, result):
        for success, change in result:
            if not success: continue
            c = Change(change['user'], change['files'], change['comments'],
                       revision=change['num'])
            self.parent.addChange(c)
            self.last_change = change['num']
