# -*- test-case-name: buildbot.test.test_util -*-

from twisted.internet.defer import Deferred
from twisted.python import log
from twisted.spread import pb
import time

def now():
    #return int(time.time())
    return time.time()

def earlier(old, new):
    # minimum of two things, but "None" counts as +infinity
    if old:
        if new < old:
            return new
        return old
    return new

def later(old, new):
    # maximum of two things, but "None" counts as -infinity
    if old:
        if new > old:
            return new
        return old
    return new

class CancelableDeferred(Deferred):
    """I am a version of Deferred that can be canceled by calling my
    .cancel() method. After being canceled, no callbacks or errbacks will be
    executed.
    """
    def __init__(self):
        Deferred.__init__(self)
        self.canceled = 0
    def cancel(self):
        self.canceled = 1
    def _runCallbacks(self):
        if self.canceled:
            self.callbacks = []
            return
        Deferred._runCallbacks(self)

from twisted.internet import reactor
import pickle, os

# objects that can be swapped out to disk when not used
class Swappable:
    """I am an object with contents that are swapped out to disk when they
    haven't been used for a while. Create me with a timeout and a filename
    where the contents should be pickled. Use .set/.get to manipulate the
    data. You must use .delete to explicitly delete the data.

    When this is persisted (or any time __getstate__ is called), the data
    will be swapped to disk first.
    
    t = Swappable(filename='foo')
    t.set(lotsOfData)
    d = t.get()
    t.delete()
    del t
    """

    timeout = 5*60 # can be overridden with s.timeout=seconds
    
    def __init__(self):
        self.data = None
        self.timer = None
        self.filed = 0
        self.filename = None
        self.parent = None
    def setFilename(self, parent=None, filename=None):
        log.msg("Swappable.setFilename(%s,%s)" % (parent, filename))
        self.filename = filename
        self.parent = parent
        if self.filename is not None and self.data is not None:
            self.startTimer()
    def computeFilename(self):
        """Most Swappable objects live (in RAM) in a tree of some sort. When
        they move out (to disk), the files they are pickled into are in a
        corresponding tree. If the .parent attribute is set, the object
        will ask it (by calling .getSwappableName()) for a directory to use.
        """
        if self.parent:
            return os.path.join(self.parent.getSwappableName(),
                                self.filename)
        return self.filename

    def get(self):
        """This will raise an IndexError if the data is unavailable (meaning
        the disk version disappeared).
        """
        log.msg(self, "Swappable.get")
        if self.filed:
            self.load()
        if self.filed:
            # wasn't able to load it
            raise IndexError, "Unable to swap in contents"
        if self.filename is not None:
            self.startTimer()
        return self.data
    def set(self, value):
        if self.filed:
            self.load()
        self.data = value
        if self.filename is not None:
            self.startTimer()
    def delete(self):
        self.stopTimer()
        del self.data
        if self.filed:
            filename = None
            try:
                filename = self.computeFilename()
                self.filed = 0
                os.unlink(filename)
            except:
                log.msg("unable to delete Swappable at '%s'" % filename)

    def startTimer(self):
        self.stopTimer()
        self.timer = reactor.callLater(self.timeout, self.fireTimer)
    def stopTimer(self):
        if self.timer:
            self.timer.cancel()
            self.timer = None
    def fireTimer(self):
        self.timer = None
        if not self.filed:
            self.save()
            
    def save(self):
        filename = None
        try:
            filename = self.computeFilename()
            log.msg(self, "Swappable.save(%s)" % filename)
            d = os.path.dirname(filename)
            if d and not os.path.isdir(d):
                os.makedirs(d)
            f = open(filename, "w")
            pickle.dump(self.data, f, 1)
            f.close()
            self.data = None
            self.filed = 1
        except:
            log.msg("unable to save Swappable to '%s'" % filename)
            log.deferr()
    def load(self):
        filename = None
        try:
            filename = self.computeFilename()
            log.msg(self, "Swappable.load(%s)" % filename)
            f = open(filename, "r")
            d = pickle.load(f)
            f.close()
            self.data = d
            self.filed = 0
            os.unlink(filename)
        except:
            log.msg("unable to load Swappable from '%s'" % filename)
            # ignore the error, may happen normally when files are pruned
            
    def __getstate__(self):
        if not self.filed:
            self.stopTimer()
            self.save()
        return self.__dict__

def ignoreStaleRefs(failure):
    """d.addErrback(util.ignoreStaleRefs)"""
    r = failure.trap(pb.DeadReferenceError, pb.PBConnectionLost)
    return None

class _None:
    pass

class ComparableMixin:
    """Specify a list of attributes that are 'important'. These will be used
    for all comparison operations."""

    compare_attrs = []

    def __hash__(self):
        alist = [self.__class__] + \
                [getattr(self, name, _None) for name in self.compare_attrs]
        return hash(tuple(alist))

    def __cmp__(self, them):
        if cmp(type(self), type(them)):
            return cmp(type(self), type(them))
        if cmp(self.__class__, them.__class__):
            return cmp(self.__class__, them.__class__)
        assert self.compare_attrs == them.compare_attrs
        self_list= [getattr(self, name, _None) for name in self.compare_attrs]
        them_list= [getattr(them, name, _None) for name in self.compare_attrs]
        return cmp(self_list, them_list)
