# -*- test-case-name: buildbot.test.test_interlock -*-

from twisted.python import log
from buildbot.util import CancelableDeferred

class Interlock:
    """I am a build interlock. I can stall a build until after some other
    builds have successfully built the same set of Changes. I receive reports
    from my feeders, I trigger my watchers. My watchers request those triggers
    by calling my .hasPassed() method, which gives them a Deferred.
    """

    debug = 0

    def __init__(self, name, feeders, watchers):
        """
        @type  name: string
        @param name: name of the interlock
        @type  feeders: list of strings
        @param feeders: names of the builders that feed this interlock
        @type  watchers: list of strings
        @param watchers: names of the builders that wait on this interlock
        """
        self.name = name
        self.feederNames = feeders  # these feed me
        self.watcherNames = watchers  # these watch me
        self.feeders = {}
        for n in feeders:
            self.feeders[n] = (None, 0)
        # feeders.keys are .names of Builder objects that we watch
        # feeders.values are Change numbers that have been successfully built
        self.watchers = {}
        # watchers.keys are Change numbers that a builder cares about
        # watchers.values are the Deferreds that those builders are waiting
        #  for
        self.active = False

    def __repr__(self):
        return "<Interlock '%s': %s->%s>" % (self.name,
                                             ",".join(self.feederNames),
                                             ",".join(self.watcherNames))

    def tryToActivate(self, builders):
        assert not self.active
        builderNames = builders.keys()
        for bname in self.feederNames + self.watcherNames:
            if bname not in builderNames:
                if self.debug: print "interlock is inactive", self
                log.msg("interlock '%s' references non-existent builder %s" \
                        % (self.name, bname))
                self.active = False
                return
        self.active = True
        for feederName in self.feederNames:
            builders[feederName].feedInterlock(self)
        for watcherName in self.watcherNames:
            builders[watcherName].watchInterlock(self)

    def deactivate(self, builders):
        assert self.active
        self.active = False
        # TODO!: fail any builds waiting on this interlock
        for feederName in self.feederNames:
            builders[feederName].stopFeedingInterlock(self)
        for watcherName in self.watcherNames:
            builders[watcherName].stopWatchingInterlock(self)

    # FIXME: maybe rename to changeProcessed instead ?
    # that way it can be used for builders with unimportant changes
    # to pass interlocks
    def buildFinished(self, feederName, changenum, successful):
        """
        Tell the interlock the build on the given builder with the given
        changenum has finished.
        In practice, this just informs the interlock that the given builder
        has processed changes up to the given changenum, with the given
        result.

        @type  feederName: string
        @param feederName: name of the builder that finished a build
        @type  changenum:  int
        @param changenum:  number of the L{buildbot.changes.changes.Change}
                           that this builder finished a build for
        @type  successful: bool
        @param successful: whether the build succeeded
        """
        # we assume that any given build will process Changes in strict order
        self.feeders[feederName] = (changenum, successful)
        if self.debug:
            print "Interlock.buildFinished(%s=%d, passed=%d)" % (feederName,
                                                                 changenum,
                                                                 successful)
        watched = self.watchers.keys()
        watched.sort()
        # flunk all watchers who are looking for a lower change number.
        # We just don't know whether or not those trees are good
        while watched and watched[0] < changenum:
            flunked = watched[0]
            watched.pop(0)
            for d in self.watchers[flunked]:
                d.callback(0) # don't build
            del self.watchers[flunked]

        # if this build failed, flunk any watchers waiting for it
        if not successful and self.watchers.has_key(changenum):
            for d in self.watchers[changenum]:
                d.callback(0) # don't build
            del self.watchers[changenum]
            return

        # build was successful. See if everyone agrees.
        # now see if all feeder builds agree that this is a good tree
        agree = 1
        for (c,good) in self.feeders.values():
            if (c,good) != (changenum,1):
                agree = 0
        if self.debug:
            print " Interlock.buildFinished(agree=%d)" % agree

        if not agree:
            return
        # if so, fire any watchers of this change
        if self.watchers.has_key(changenum):
            for d in self.watchers[changenum]:
                d.callback(1) # do build
            del self.watchers[changenum]
            return

    def hasPassed(self, testChange):
        if self.debug:
            print "Interlock.hasPassed(%d)" % testChange
        d = CancelableDeferred()
        done = 1
        for change,good in self.feeders.values():
            if change != None:
                # if any feeders are beyond this change, flunk the watcher
                if change > testChange:
                    if self.debug:
                        print " no: feeder is beyond"
                    d.callback(0) # don't build
                    return d
                # a failed feeder at the given change is also reason to flunk
                if change == testChange and not good:
                    if self.debug:
                        print " no: failed build"
                    d.callback(0) # don't build
                    return d
                # if all feeders are at this level and are good, pass now
                if change < testChange:
                    done = 0
            else:
                done = 0
        if done:
            if self.debug:
                print " yes"
            d.callback(1) # do build
        else:
            if self.debug:
                print " no, adding Deferred to self.watchers[%d]" % testChange
            l = self.watchers.get(testChange, [])
            self.watchers[testChange] = l
            l.append(d)
        return d
    
