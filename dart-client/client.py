#!/usr/bin/python

import sys
import pysvn
import os.path
import time
from datetime import datetime, timedelta

class Build:
    def __init__(self, id_, build_variant_, ctest_variant_):
        self.id = id_
        self.build_variant = build_variant_
        self.ctest_variant = ctest_variant_
        self.revision = -1
        self.avg_time = datetime.min
        self.last_start = datetime.min

    def __str__(self):
        return "id:" + self.id + " bv:" + self.build_variant + " cv:" + self.ctest_variant + " rev:" + str(self.revision) + " avg_t:" + str(self.avg_time) + " last_t:" + str(self.last_start)

builds = []
buildqueue = []

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
    sys.stderr.write(style.BOLDRED('Error: %s' % msg) + '\nRun "%s --help" for help.\n' % cmd)
    sys.exit(1)

configfile = "conf.py"

def read_conf():
    if os.path.exists(configfile):
        execfile(configfile, globals())
    else:
        print_error("Config file '" + configfile + "' not found, run " + sys.argv[0] + " init first.", sys.argv[0])
        exit(1)

def checkout(argv):
    client = pysvn.Client()
    client.exception_style = 0
    builds = initbuilds()
    for id, url in urls.items():
        srcdir = os.path.join(topdir,prefix,id,"src")
        try:
            os.mkdir(srcdir)
        except:
            print "Directory %s exists, not creating." % id
            
        try:
            print "Checking out " + id
            client.checkout(url, srcdir)
        except pysvn.ClientError, e:
            print "Error:\n" + str(e)

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
    client = pysvn.Client()
    client.exception_style = 0
    builds = initbuilds()
    while True:
        build = nextbuild(builds)
        print "*******\n******* " + str(build) + " *******\n*******" 
        build.last_start = datetime.now()
        os.chdir(os.path.join(topdir, prefix, build.id, build.build_variant, build.ctest_variant))
        cmd = ctest + " " + " ".join(ctest_variants[build.ctest_variant][0])
        srcdir = os.path.join(topdir, prefix, build.id, "src")
        if build.revision != -1:
            print ">>> Updating " + srcdir + " to " + str(build.revision)
            client.update(srcdir, recurse=True,
                          revision=pysvn.Revision(pysvn.opt_revision_kind.number, build.revision))
            time.sleep(0)
        print ">>> " + cmd
        os.system(cmd)
        status_list = client.status(os.path.join(topdir, prefix, build.id, "src"), recurse=False)
        rev = status_list[0].entry.revision.number
        build.revision = rev
#        print "status: " + str(status_list)
        time.sleep(0)
                
def init(argv):
    print "Writing " + configfile + "\n" 
    initfile = open(configfile, 'w')
    initfile.write("prefix = 'prefix'\nurls = { 'boost_1_34_0' : 'http://svn.boost.org/svn/boost/sandbox-branches/boost-cmake/boost_1_34_0/tools/build/CMake' }\n")
    initfile.write("build_variants = { 'debug' : ['BUILD_RELEASE:BOOL=OFF','BUILD_DEBUG:BOOL=ON'],\n 'release': ['BUILD_RLEASE:BOOL=OFF','BUILD_DEBUG:BOOL=ON'] }\n")
    initfile.close()

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
    'init' : init,
    'checkout' : checkout,
    'run' : run,
    'help' : help,
    'initbuilds' : dropenv(initbuilds),
    }

if __name__ == "__main__":
    main(sys.argv)
