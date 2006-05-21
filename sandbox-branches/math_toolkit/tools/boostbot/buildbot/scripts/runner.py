# -*- test-case-name: buildbot.test.test_runner -*-

# N.B.: don't import anything that might pull in a reactor yet. Some of our
# subcommands want to load modules that need the gtk reactor.
import os, os.path, sys, shutil, stat
from twisted.python import usage, util, runtime

# this is mostly just a front-end for mktap, twistd, and kill(1), but in the
# future it will also provide an interface to some developer tools that talk
# directly to a remote buildmaster (like 'try' and a status client)

# the create/start/stop commands should all be run as the same user,
# preferably a separate 'buildbot' account.

class Maker:
    def __init__(self, config):
        self.basedir = os.path.abspath(config['basedir'])
        self.force = config['force']
        self.quiet = config['quiet']

    def mkdir(self):
        if os.path.exists(self.basedir) and not self.force:
            print ("basedir %s already exists and --force not used"
                   % self.basedir)
            print "refusing to touch existing setup"
            sys.exit(1)
        if not os.path.exists(self.basedir):
            if not self.quiet: print "mkdir", self.basedir
            os.mkdir(self.basedir)

    def mkinfo(self):
        path = os.path.join(self.basedir, "info")
        if not os.path.exists(path):
            if not self.quiet: print "mkdir", path
            os.mkdir(path)
        admin = os.path.join(path, "admin")
        if not os.path.exists(admin):
            f = open(admin, "wt")
            f.write("Your Name Here <admin@youraddress.invalid>\n")
            f.close()
        host = os.path.join(path, "host")
        if not os.path.exists(host):
            f = open(host, "wt")
            f.write("Please put a description of this build host here\n")
            f.close()
        print "Please edit the files in %s appropriately." % path

    def chdir(self):
        if not self.quiet: print "chdir", self.basedir
        os.chdir(self.basedir)

    def mktap(self, cmd):
        if not self.quiet: print cmd
        status = os.system(cmd)
        if status != 0:
            print "mktap failed, bailing.."
            sys.exit(1)
        if not os.path.exists("buildbot.tap"):
            print "mktap failed to create buildbot.tap, bailing.."
            sys.exit(1)
        os.chmod("buildbot.tap", 0600)

    def makefile(self, source, cmd):
        target = "Makefile"
        if os.path.exists(target):
            print "not touching existing Makefile"
            print "installing sample in Makefile.sample instead"
            target = "Makefile.sample"
        shutil.copy(source, target)
        os.chmod(target, 0600)
        f = open(target, "a")
        f.write("\n")
        f.write("tap:\n")
        f.write("\t" + cmd + "\n")
        f.write("\n")
        f.close()

    def sampleconfig(self, source):
        target = "master.cfg"
        if os.path.exists(target):
            print "not touching existing master.cfg"
            print "installing sample in master.cfg.sample instead"
            target = "master.cfg.sample"
        shutil.copy(source, target)
        os.chmod(target, 0600)

def createMaster(config):
    m = Maker(config)
    m.mkdir()
    m.chdir()

    cmd = "mktap buildbot master --basedir %s" % m.basedir

    m.mktap(cmd)
    m.sampleconfig(util.sibpath(__file__, "sample.cfg"))
    m.makefile(util.sibpath(__file__, "sample.mk"), cmd)

    if not m.quiet: print "buildmaster configured in %s" % m.basedir
    sys.exit(0)

def createSlave(config):
    m = Maker(config)
    m.mkdir()
    m.chdir()

    cmd = ("mktap buildbot slave " +
           "--basedir %s --master %s --name %s --passwd %s" \
           % (m.basedir, config['master'], config['name'], config['passwd']))

    m.mktap(cmd)
    m.makefile(util.sibpath(__file__, "sample.mk"), cmd)
    m.mkinfo()

    if not m.quiet: print "buildslave configured in %s" % m.basedir
    sys.exit(0)

def start(config):
    basedir = config['basedir']
    quiet = config['quiet']
    os.chdir(basedir)
    if os.path.exists("/usr/bin/make") and os.path.exists("Makefile"):
        # yes, this is clunky. Preferring the Makefile lets slave admins do
        # useful things like set up environment variables for the buildslave.
        cmd = "make start"
    else:
        reactor_arg = ""
        if sys.platform == "win32":
            reactor_arg = "--reactor=win32"
        cmd = "twistd %s --no_save -f buildbot.tap" % reactor_arg
    if not quiet: print cmd
    os.system(cmd)
    sys.exit(0)

def stop(config, signal=""):
    basedir = config['basedir']
    quiet = config['quiet']
    os.chdir(basedir)
    cmd = "kill %s `cat twistd.pid`" % signal
    if not quiet: print cmd
    os.system(cmd)
    # TODO: poll once per second until twistd.pid goes away
    sys.exit(0)

def loadOptions(filename="options", here=None, home=None):
    """Find the .buildbot/FILENAME file. Crawl from the current directory up
    towards the root, and also look in ~/.buildbot . The first directory
    that's owned by the user and has the file we're looking for wins. Windows
    skips the owned-by-user test.
    
    @rtype : dict
    @return: a dictionary of names defined in the options file. If no options
    file was found, return an empty dict."""

    if here is None:
        here = os.getcwd()
    here = os.path.abspath(here)

    if home is None:
        if runtime.platformType == 'win32':
            home = os.path.join(os.environ['APPDATA'], "buildbot")
        else:
            home = os.path.expanduser("~/.buildbot")

    searchpath = []
    toomany = 20
    while True:
        searchpath.append(os.path.join(here, ".buildbot"))
        next = os.path.dirname(here)
        if next == here:
            break # we've hit the root
        here = next
        toomany -= 1 # just in case
        if toomany == 0:
            raise ValueError("Hey, I seem to have wandered up into the "
                             "infinite glories of the heavens. Oops.")
    searchpath.append(home)

    localDict = {}

    for d in searchpath:
        if os.path.isdir(d):
            if runtime.platformType != 'win32':
                if os.stat(d)[stat.ST_UID] != os.getuid():
                    print "skipping %s because you don't own it" % d
                    continue # security, skip other people's directories
            optfile = os.path.join(d, filename)
            if os.path.exists(optfile):
                try:
                    f = open(optfile, "r")
                    options = f.read()
                    exec options in localDict
                except:
                    print "error while reading %s" % optfile
                    raise
                break

    for k in localDict.keys():
        if k.startswith("__"):
            del localDict[k]
    return localDict

class Base(usage.Options):
    optFlags = [
        ['help', 'h', "Display this message"],
        ["quiet", "q", "Do not emit the commands being run"],
        ]
    opt_h = usage.Options.opt_help

    def parseArgs(self, basedir=None):
        if basedir is None:
            raise usage.UsageError("<basedir> parameter is required")
        self['basedir'] = basedir

class MasterOptions(Base):
    optFlags = [
        ["force", "f",
         "Re-use an existing directory (will not overwrite master.cfg file)"],
        ]
    def getSynopsis(self):
        return "Usage:    buildbot master [options] <basedir>"

class SlaveOptions(Base):
    optFlags = [
        ["force", "f", "Re-use an existing directory"],
#        ["nopty", None, "Do *not* run child commands under a PTY"],
        ]

    def getSynopsis(self):
        return "Usage:    buildbot slave [options] <basedir> <master> <name> <passwd>"

    def parseArgs(self, *args):
        if len(args) < 4:
            raise usage.UsageError("command needs more arguments")
        basedir, master, name, passwd = args
        self['basedir'] = basedir
        self['master'] = master
        self['name'] = name
        self['passwd'] = passwd

class StartOptions(Base):
    def getSynopsis(self):
        return "Usage:    buildbot start <basedir>"

class StopOptions(Base):
    def getSynopsis(self):
        return "Usage:    buildbot stop <basedir>"

class DebugClientOptions(usage.Options):
    optFlags = [
        ['help', 'h', "Display this message"],
        ]
    optParameters = [
        ["master", "m", None,
         "Location of the buildmaster's slaveport (host:port)"],
        ["passwd", "p", None, "Debug password to use"],
        ]

    def parseArgs(self, *args):
        if len(args) > 0:
            self['master'] = args[0]
        if len(args) > 1:
            self['passwd'] = args[1]
        if len(args) > 2:
            raise usage.UsageError("I wasn't expecting so many arguments")

def debugclient(config):
    from buildbot.clients import debug
    opts = loadOptions()

    master = config.get('master')
    if not master:
        master = opts.get('master')
    if master is None:
        raise usage.UsageError("master must be specified: on the command "
                               "line or in ~/.buildbot/options")

    passwd = config.get('passwd')
    if not passwd:
        passwd = opts.get('debugPassword')
    if passwd is None:
        raise usage.UsageError("passwd must be specified: on the command "
                               "line or in ~/.buildbot/options")

    d = debug.DebugWidget(master, passwd)
    d.run()

class StatusClientOptions(usage.Options):
    optFlags = [
        ['help', 'h', "Display this message"],
        ]
    optParameters = [
        ["master", "m", None,
         "Location of the buildmaster's status port (host:port)"],
        ]

    def parseArgs(self, *args):
        if len(args) > 0:
            self['master'] = args[0]
        if len(args) > 1:
            raise usage.UsageError("I wasn't expecting so many arguments")

def statuslog(config):
    from buildbot.clients import base
    opts = loadOptions()
    master = config.get('master')
    if not master:
        master = opts.get('masterstatus')
    if master is None:
        raise usage.UsageError("master must be specified: on the command "
                               "line or in ~/.buildbot/options")
    c = base.TextClient(master)
    c.run()

def statusgui(config):
    from buildbot.clients import gtkPanes
    opts = loadOptions()
    master = config.get('master')
    if not master:
        master = opts.get('masterstatus')
    if master is None:
        raise usage.UsageError("master must be specified: on the command "
                               "line or in ~/.buildbot/options")
    c = gtkPanes.GtkClient(master)
    c.run()

class Options(usage.Options):
    synopsis = "Usage:    buildbot <command> [command options]"

    subCommands = [
        # the following are all admin commands
        ['master', None, MasterOptions,
         "Create and populate a directory for a new buildmaster"],
        ['slave', None, SlaveOptions,
         "Create and populate a directory for a new buildslave"],
        ['start', None, StartOptions, "Start a buildmaster or buildslave"],
        ['stop', None, StopOptions, "Stop a buildmaster or buildslave"],
        ['sighup', None, StopOptions,
         "SIGHUP a buildmaster to make it re-read the config file"],

        ['debugclient', None, DebugClientOptions,
         "Launch a small debug panel GUI"],

        ['statuslog', None, StatusClientOptions,
         "Emit current builder status to stdout"],
        ['statusgui', None, StatusClientOptions,
         "Display a small window showing current builder status"],

        # TODO: 'try', 'watch'
        ]

    def postOptions(self):
        if not hasattr(self, 'subOptions'):
            raise usage.UsageError("must specify a command")


def run():
    config = Options()
    try:
        config.parseOptions()
    except usage.error, e:
        print "%s:  %s" % (sys.argv[0], e)
        print
        c = getattr(config, 'subOptions', config)
        print str(c)
        sys.exit(1)

    command = config.subCommand
    so = config.subOptions

    if command == "master":
        createMaster(so)
    elif command == "slave":
        createSlave(so)
    elif command == "start":
        start(so)
    elif command == "stop":
        stop(so)
    elif command == "sighup":
        stop(so, "-HUP")
    elif command == "debugclient":
        debugclient(so)
    elif command == "statuslog":
        statuslog(so)
    elif command == "statusgui":
        statusgui(so)
