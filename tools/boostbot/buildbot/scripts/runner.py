#! /usr/bin/python

import os, os.path, sys, shutil
from twisted.python import usage, util

# this is mostly just a front-end for mktap, twistd, and kill(1), but in the
# future it will also provide an interface to some developer tools that talk
# directly to a remote buildmaster (like 'try' and a status client)

# the create/start/stop commands should all be run as the same user,
# preferably a separate 'buildbot' account.

def createMaster(config):
    basedir = os.path.abspath(config['basedir'])
    force = config['force']
    quiet = config['quiet']
    if os.path.exists(basedir) and not force:
        print "basedir %s already exists and --force not used" % basedir
        print "refusing to touch existing setup"
        sys.exit(1)
    if not os.path.exists(basedir):
        if not quiet: print "mkdir", basedir
        os.mkdir(basedir)

    if not quiet: print "chdir", basedir
    os.chdir(basedir)

    cmd = "mktap buildbot master --basedir %s" % basedir
    if not quiet: print cmd
    status = os.system(cmd)
    if status != 0:
        print "mktap failed, bailing.."
        sys.exit(1)

    target = "master.cfg"
    if os.path.exists(target):
        print "not touching existing master.cfg"
        print "installing sample in master.cfg.sample instead"
        target = "master.cfg.sample"
    sampleconfig = util.sibpath(__file__, "sample.cfg")
    shutil.copy(sampleconfig, target)

    target = "Makefile"
    if os.path.exists(target):
        print "not touching existing Makefile"
        print "installing sample in Makefile.sample instead"
        target = "Makefile.sample"
    samplemk = util.sibpath(__file__, "sample.mk")
    shutil.copy(samplemk, target)

    if not quiet: print "buildmaster configured in %s" % basedir
    sys.exit(0)

def createSlave(config):
    basedir = os.path.abspath(config['basedir'])
    force = config['force']
    quiet = config['quiet']
    if os.path.exists(basedir) and not force:
        print "basedir %s already exists and --force not used" % basedir
        print "refusing to touch existing setup"
        sys.exit(1)
    if not os.path.exists(basedir):
        if not quiet: print "mkdir", basedir
        os.mkdir(basedir)
    if not quiet: print "chdir", basedir
    os.chdir(basedir)
    cmd = ("mktap buildbot slave " +
           "--basedir %s --master %s --name %s --passwd %s" \
           % (basedir, config['master'], config['name'], config['passwd']))
    if not quiet: print cmd
    status = os.system(cmd)
    if status != 0:
        print "mktap failed, bailing.."
        sys.exit(1)
    if not quiet: print "buildslave configured in %s" % basedir
    sys.exit(0)

def start(config):
    basedir = config['basedir']
    quiet = config['quiet']
    os.chdir(basedir)
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
