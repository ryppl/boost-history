#!/usr/bin/env python

#
#  Continuous/nightly testing script.
#
#  This script runs continuous and nightly tests in a continuous loop.
#
#  'client.py checkout' will check out the necessary source and set up
#  the necessary directories.
#
#  'client.py run'  will run the tests in an infinite loop.
#
#  The script runs the test which was run least recently.  It
#  determines how long it was since each test run with the *_dt(time)
#  functions in conf.py.  Nightly tests are run only once per day
#  because nightly_dt returns zero if its time argument occurs on the
#  same day as the current time.
#

import sys
import os.path
import time
import subprocess
from datetime import datetime, timedelta
from xml.dom.minidom import parseString

configfile = "conf.py"

#
# Build class holds the information about each build.
#
# build_variant:  as specified in build_variants in the configuration file.
# ctest_variant:  as specified in ctest_variants in the configuration file (continuous/nightly).  
# revision:       svn revision of the local checkout as of the last time this test was run.
# last_start:     time this test was last started. 
#
class Build:
    def __init__(self, id_, build_variant_, ctest_variant_, revision_):
        self.id = id_
        self.build_variant = build_variant_
        self.ctest_variant = ctest_variant_
        self.revision = revision_
        self.last_start = datetime.now()

    def __str__(self):
        return self.id + "/" + self.build_variant + "/" + self.ctest_variant + " r" + str(self.revision) + " last_start @ " + str(self.last_start)

#
# Get current svn revision number of srcdir
#
def svn_status_revision(srcdir):
    output = subprocess.Popen([svn, "info", "--xml", srcdir], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()[0]
    dom = parseString(output)
    rev = dom.getElementsByTagName("commit")[0].getAttribute("revision")
    return rev

#
# svn update "srcdir" to revision "revision"
#
def svn_update(srcdir, revision):
    try:
        retcode = subprocess.call([svn, "update", "-r", revision, srcdir])
        if retcode < 0:
            print >>sys.stderr, "Child was terminated by signal ", -retcode
        else:
            print >>sys.stderr, "Child returned", retcode
    except OSError, e:
        print >> sys.stderr, "Execution failed:", e

#
# svn checkout "url" to local directory "srcdir"
#
def svn_checkout(url, srcdir):
    try:
        retcode = subprocess.call([svn, "co", url, srcdir])
        if retcode < 0:
            print >>sys.stderr, "Child was terminated by signal ", -retcode
        else:
            print >>sys.stderr, "Child returned", retcode
    except OSError, e:
        print >> sys.stderr, "Execution failed:", e

#
# return the build that was run longest ago
# as reported by the *_dt functions in the config file.
#
def nextbuild(builds):
    nextbuild = Build('none', 'none', 'none', -1)
    nextbuild_deltat = timedelta.min

    for b in builds:
        b_dt = ctest_variants[b.ctest_variant][1](b.last_start)
        if b_dt > nextbuild_deltat:
            nextbuild_deltat = b_dt
            nextbuild = b
    return nextbuild
    
#
# Create list of builds (used in initialization)
#
def initbuilds():
    builds = []
    for id in urls:
        srcdir = os.path.join(topdir, prefix, id, "src")
        try:
            rev = svn_status_revision(srcdir)
        except:
            rev = -1
        for bv in build_variants:
            for cv in ctest_variants:
                build = Build(id, bv, cv, rev)
                builds.append(build)
                print "Initialized build " + str(build)
    return builds

#
# print error message
#
def print_error(msg, cmd):
    sys.stderr.write('%s: Error: %s\n' % (cmd, msg))
    sys.exit(1)

#
# read the configuration file "configfile" into the current environment
#
def read_conf():
    if os.path.exists(configfile):
        execfile(configfile, globals())
    else:
        print_error("Config file '" + configfile + "' not found, run " + sys.argv[0] + " init first.", sys.argv[0])
        exit(1)

#
# create local cache directories
#
def create_caches(argv):
    print "Making build directories..."
    for build in initbuilds():
        buildpath = os.path.join(topdir,prefix, build.id, build.build_variant, build.ctest_variant)
        srcpath = os.path.join(topdir, prefix, build.id, "src")
        try:
            os.makedirs(buildpath)
            print ">>> Initializing " + buildpath
        except Exception, e:
            print "Directory %s exists, not creating (%s)" % (buildpath, e)
        os.chdir(buildpath)
        cmd = cmake + " " + " ".join(build_variants[build.build_variant]) + " " + srcpath
        print ">>> Executing " + cmd
        os.system(cmd)
    
#
# run when './client.py checkout' is specified.  Create the necessary
# directories for the various build/test variants and checkout the
# source.
#
def checkout(argv):
    builds = initbuilds()
    for id, url in urls.items():
        srcdir = os.path.join(topdir,prefix,id,"src")
        try:
            os.mkdir(srcdir)
        except:
            print "Directory %s exists, not creating." % id
            
        print "Checking out " + id
        svn_checkout(url, srcdir)
    create_caches(argv)

#
#  Do the builds in an infinite loop.
#
def run(args):
    builds = initbuilds()
    while True:
        build = nextbuild(builds)
        print ">>> Starting " + str(build)
        srcdir = os.path.join(topdir, prefix, build.id, "src")
        if build.revision != -1:
            print ">>> Updating " + srcdir + " to " + str(build.revision)
            svn_update(srcdir, build.revision)
        build.last_start = datetime.now()
        os.chdir(os.path.join(topdir, prefix, build.id, build.build_variant, build.ctest_variant))
        cmd = ctest + " " + " ".join(ctest_variants[build.ctest_variant][0])
        os.system(cmd)
        rev = svn_status_revision(srcdir)
        build.revision = rev
        print ">>> Finished %s/%s/%s, now at r%s" % (build.id, build.build_variant, build.ctest_variant, build.revision)
        print ">>> Sleeping %s seconds..." % interbuild_sleep
        time.sleep(interbuild_sleep)

def srcdir_path(build):
    return os.path.join(topdir, prefix, build.id, "src")
#
#  eh.
#
def help(argv):
    print "Usage:\n\n  " + argv[0] + " (checkout|run)\n\ncheckout:  checks out source and sets up build environment.  Do this first.\nrun:       run regression tests in a loop.\n\n"
    sys.exit(1)
    
topdir = '?'
def main(argv):
    globals()['topdir'] = os.getcwd()
    if argv[1] == "init":
        init(argv)
    elif not action_mapping.has_key(argv[1]):
        print_error("Command " + argv[1] + " not found", argv[0])
    else:
        read_conf()
        action_mapping[argv[1]](argv)

#
# map command-line strings to functions
#
action_mapping = {
    'checkout' : checkout,
    'create_caches' : create_caches,
    'run' : run,
    'help' : help,
    }

if __name__ == "__main__":
    if len(sys.argv) != 2:
        help(sys.argv)
    main(sys.argv)
