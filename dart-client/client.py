#!/usr/bin/env python

import sys
import os.path
import time
import subprocess
from datetime import datetime, timedelta
from xml.dom.minidom import parseString

class Build:
    def __init__(self, id_, build_variant_, ctest_variant_):
        self.id = id_
        self.build_variant = build_variant_
        self.ctest_variant = ctest_variant_
        self.revision = -1
        self.last_start = datetime.min

    def __str__(self):
        return self.id + "/" + self.build_variant + "/" + self.ctest_variant + " r" + str(self.revision) + " last_t:" + str(self.last_start)

def svn_status_revision(srcdir):
    output = subprocess.Popen([svn, "info", "--xml", srcdir], stdout=subprocess.PIPE).communicate()[0]
    dom = parseString(output)
    rev = dom.getElementsByTagName("commit")[0].getAttribute("revision")
    return rev

def svn_update(srcdir, revision):
    try:
        retcode = subprocess.call([svn, "update", "-r", revision, srcdir])
        if retcode < 0:
            print >>sys.stderr, "Child was terminated by signal ", -retcode
        else:
            print >>sys.stderr, "Child returned", retcode
    except OSError, e:
        print >> sys.stderr, "Execution failed:", e

def svn_checkout(url, srcdir):
    try:
        retcode = subprocess.call([svn, "co", url, srcdir])
        if retcode < 0:
            print >>sys.stderr, "Child was terminated by signal ", -retcode
        else:
            print >>sys.stderr, "Child returned", retcode
    except OSError, e:
        print >> sys.stderr, "Execution failed:", e

def nextbuild(builds):
    front = Build('none', 'none', 'none')
    front_deltat = datetime.min
    front.last_start = datetime.max

    for build in builds:
        thisdt = ctest_variants[build.ctest_variant][1](build.last_start)
        print "Delta for build " + str(build) + " is: " + str(thisdt)
        if build.last_start <= front.last_start:
            front = build
    return front
    
def initbuilds():
    builds = []
    for id in urls:
        for bv in build_variants:
            for cv in ctest_variants:
                builds.append(Build(id, bv, cv))
    return builds

def print_error(msg, cmd):
    sys.stderr.write('%s: Error: %s\n' % (cmd, msg))
    sys.exit(1)

configfile = "conf.py"

def read_conf():
    if os.path.exists(configfile):
        execfile(configfile, globals())
    else:
        print_error("Config file '" + configfile + "' not found, run " + sys.argv[0] + " init first.", sys.argv[0])
        exit(1)

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

        print "Making build directories..."
        for build in builds:
            nextpath = os.path.join(topdir,prefix, build.id, build.build_variant, build.ctest_variant)
            try:
                os.makedirs(nextpath)
                print "  " + nextpath
            except Exception, e:
                print "Directory %s exists, not creating (%s)" % (nextpath, e)
            os.chdir(nextpath)
            cmd = cmake + " " + " ".join(build_variants[build.build_variant]) + " " + srcdir
            print cmd
            os.system(cmd)
    
def run(args):
    builds = initbuilds()
    while True:
        build = nextbuild(builds)
        print ">>> Doing " + str(build)
        if build.revision != -1:
            print ">>> Updating " + srcdir + " to " + str(build.revision)
            svn_update(srcdir, build.revision)
        build.last_start = datetime.now()
        os.chdir(os.path.join(topdir, prefix, build.id, build.build_variant, build.ctest_variant))
        cmd = ctest + " " + " ".join(ctest_variants[build.ctest_variant][0])
        print ">>> " + cmd
        os.system(cmd)
        srcdir = os.path.join(topdir, prefix, build.id, "src")
        rev = svn_status_revision(srcdir)
        build.revision = rev
        print ">>> New last-built revision of " + srcdir + " is " + str(rev)
        print ">>>\n>>> Finshed build " + str(build) + "\n>>>"
        time.sleep(interbuild_sleep)
                
def help(argv):
    print __name__
    
topdir = '?'
def main(argv):
    globals()['topdir'] = os.getcwd()
    print "topdir is " + topdir
    if argv[1] == "init":
        init(argv)
    elif not action_mapping.has_key(argv[1]):
        print_error("Command " + argv[1] + " not found", argv[0])
    else:
        read_conf()
        action_mapping[argv[1]](argv)

def dropenv(fn, *args):
    return lambda x: fn(*args)

action_mapping = {
    'checkout' : checkout,
    'run' : run,
    'help' : help,
    }

if __name__ == "__main__":
    main(sys.argv)
