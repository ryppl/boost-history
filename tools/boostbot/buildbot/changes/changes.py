#! /usr/bin/python

from __future__ import generators
import string, os, os.path, time, types
try:
    import cPickle as pickle
except ImportError:
    import pickle

from twisted.python import log, components
from twisted.spread import pb
from twisted.application import service
from twisted.cred import portal
from twisted.web import html

from buildbot import interfaces, util

html_tmpl = """
<p>Changed by: <b>%(who)s</b><br />
Changed at: <b>%(at)s</b><br />
%(revision)s
<br />

Changed files:
%(files)s

Comments:
%(comments)s
</p>
"""

class Change:
    """I represent a single change to the source tree. This may involve
    several files, but they are all changed by the same person, and there is
    a change comment for the group as a whole.

    If the version control system supports sequential repository- (or
    branch-) wide change numbers (like SVN, P4, and Arch), then revision=
    should be set to that number. The highest such number will be used at
    checkout time to get the correct set of files.

    If it does not (like CVS), when= should be set to the timestamp (seconds
    since epoch, as returned by time.time()) when the change was made. when=
    will be filled in for you (to the current time) if you omit it, which is
    suitable for ChangeSources which have no way of getting more accurate
    timestamps.

    Changes should be submitted to ChangeMaster.addChange() in
    chronologically increasing order. Out-of-order changes will probably
    cause the html.Waterfall display to be corrupted."""

    __implements__ = interfaces.IStatusEvent,
    links = []
    number = None
    revision = None # used to create a source-stamp

    def __init__(self, who, files, comments, isdir=0, links=[],
                 revision=None, when=None):
        self.who = who
        self.files = files
        self.comments = comments
        self.isdir = isdir
        self.links = links
        self.revision = revision
        if when is None:
            when = util.now()
        self.when = when

    def asText(self):
        data = ""
        data += self.getFileContents() 
        data += "At: %s\n" % self.getTime()
        data += "Changed By: %s\n" % self.who
        data += "Comments: %s\n\n" % self.comments
        return data

    def asHTML(self):
        links = []
        for file in self.files:
            link = filter(lambda s: s.find(file) != -1, self.links)
            if len(link) == 1:
                # could get confused
                links.append('<a href="%s"><b>%s</b></a>' % (link[0], file))
            else:
                links.append('<b>%s</b>' % file)
        revision = ""
        if self.revision:
            revision = "Revision: <b>%s</b><br />\n" % self.revision

        kwargs = { 'who'     : html.escape(self.who),
                   'at'      : self.getTime(),
                   'files'   : html.UL(links) + '\n',
                   'revision': revision,
                   'comments': html.PRE(self.comments) }
        return html_tmpl % kwargs

    def getTime(self):
        if not self.when:
            return "?"
        return time.strftime("%a %d %b %Y %H:%M:%S",
                             time.localtime(self.when))

    def getTimes(self):
        return (self.when, None)

    def getText(self):
        return [html.escape(self.who)]
    def getColor(self):
        return "white"
    def getLogs(self):
        return {}

    def getFileContents(self):
        data = ""
        if len(self.files) == 1:
            if self.isdir:
                data += "Directory: %s\n" % self.files[0]
            else:
                data += "File: %s\n" % self.files[0]
        else:
            data += "Files:\n"
            for f in self.files:
                data += " %s\n" % f
        return data
        
class ChangeMaster(service.MultiService):

    """This is the master-side service which receives file change
    notifications from CVS. It keeps a log of these changes, enough to
    provide for the HTML waterfall display, and to tell
    temporarily-disconnected bots what they missed while they were
    offline.

    Change notifications come from two different kinds of sources. The first
    is a PB service (servicename='changemaster', perspectivename='change'),
    which provides a remote method called 'addChange', which should be
    called with a dict that has keys 'filename' and 'comments'.

    The second is a list of objects derived from the ChangeSource class.
    These are added with .addSource(), which also sets the .changemaster
    attribute in the source to point at the ChangeMaster. When the
    application begins, these will be started with .start() . At shutdown
    time, they will be terminated with .stop() . They must be persistable.
    They are expected to call self.changemaster.addChange() with Change
    objects.

    There are several different variants of the second type of source.
    
     MaildirSource watches a maildir for CVS commit mail. It uses DNotify if
     available, or polls every 10 seconds if not. It parses incoming mail to
     determine what files were changed.

     FreshCVSSource makes a PB connection to the CVSToys 'freshcvs' daemon
     and relays any changes it announces.
    
    """

    debug = 0
    # todo: use Maildir class to watch for changes arriving by mail

    def __init__(self):
        service.MultiService.__init__(self)
        self.changes = []
        # self.basedir must be filled in by the parent
        # self.botmaster too
        self.nextNumber = 1
        self.sources = [] # for status page: what sources are being monitored

    def addSource(self, source):
        assert components.implements(source, interfaces.IChangeSource)
        assert components.implements(source, service.IService)
        if self.debug:
            print "ChangeMaster.addSource", source
        source.setServiceParent(self)
        self.sources.append(source)

    def removeSource(self, source):
        if self.debug:
            print "ChangeMaster.removeSource", source
        source.disownServiceParent()
        self.sources.remove(source)

    def addChange(self, change):
        """Deliver a file change event. The event should be a Change object.
        This method will timestamp the object as it is received."""
        log.msg("adding change, who %s, %d files, rev=%s, comments %s" %
                (change.who, len(change.files), change.revision,
                 change.comments))
        change.number = self.nextNumber
        self.nextNumber += 1
        self.changes.append(change)
        self.botmaster.addChange(change)
        # TODO: call pruneChanges after a while

    def pruneChanges(self):
        self.changes = self.changes[-100:] # or something

    def eventGenerator(self):
        for i in range(len(self.changes)-1, -1, -1):
            c = self.changes[i]
            yield c

    def getChangeNumbered(self, num):
        if not self.changes:
            return None
        first = self.changes[0].number
        if first + len(self.changes)-1 != self.changes[-1].number:
            log.msg(self,
                    "lost a change somewhere: [0] is %d, [%d] is %d" % \
                    (self.changes[0].number,
                     len(self.changes) - 1,
                     self.changes[-1].number))
            for c in self.changes:
                log.msg("c[%d]: " % c.number, c)
            return None
        offset = num - first
        log.msg(self, "offset", offset)
        return self.changes[offset]

    def __getstate__(self):
        d = service.MultiService.__getstate__(self)
        del d['parent']
        del d['services'] # lose all children
        del d['namedServices']
        del d['botmaster']
        del d['sources']
        return d

    def __setstate__(self, d):
        self.__dict__ = d
        # self.basedir must be set by the parent
        # self.botmaster too
        self.sources = [] # will be filled when the config file is read
        self.services = [] # they'll be repopulated by readConfig
        self.namedServices = {}


    def saveYourself(self):
        filename = os.path.join(self.basedir, "changes.pck")
        tmpfilename = filename + ".tmp"
        try:
            pickle.dump(self, open(tmpfilename, "w"))
            os.rename(tmpfilename, filename)
        except Exception, e:
            log.msg("unable to save changes")
            log.err()

    def stopService(self):
        self.saveYourself()
        return service.MultiService.stopService(self)
