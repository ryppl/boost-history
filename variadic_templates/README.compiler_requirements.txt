Current versions of the g++ compiler (as of 2009-08-28, that includes
gcc-4.5 and below) may fail to compile templates involving
boost::mpl::integral_c.  The workaround is applying the patch for
bug 40092 that's mentioned here:

  http://gcc.gnu.org/ml/gcc-patches/2009-08/msg01217.html

If you're using Boost.Build:

  http://www.boost.org/doc/tools/build/doc/html/bbv2/installation.html

to run tests, then your $HOME/user-config.jam file (where $HOME is
your home directory) must make this patched compiler available with
lines something like: 

#{---cut here---

  using gcc 
    : 4.4_20090630_v
    : "/home/evansl/download/gcc/4.4-20090630/install/bin/g++" 
    : <cxxflags>-I/home/evansl/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates
      <cxxflags>-std=gnu++0x
    ;
    
#}---cut here---

The <cxxflags>-I... assures the variadic_templates/boost directory
takes precedence over the boost-trunk/boost directory. Of course you
will have to change that to your particular path.

The tests can then be compiled as shown below:

#{---cut here---
-*- mode: compilation; default-directory: "~/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test/" -*-
Compilation started at Sat Aug 29 06:39:05

bjam toolset=gcc-4.4_20090630_v unpack_args
sh: icpc: not found
docutils-dir=
tools-dir= /usr/bin/rst2html.py
...patience...
...found 362 targets...
...updating 5 targets...
common.mkdir ../../../../../../../../bin.v2/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test/unpack_args.test
common.mkdir ../../../../../../../../bin.v2/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test/unpack_args.test/gcc-4.4_20090630_v
common.mkdir ../../../../../../../../bin.v2/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test/unpack_args.test/gcc-4.4_20090630_v/debug
gcc.compile.c++ ../../../../../../../../bin.v2/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test/unpack_args.test/gcc-4.4_20090630_v/debug/unpack_args.o
**passed** ../../../../../../../../bin.v2/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/libs/mpl/test/unpack_args.test/gcc-4.4_20090630_v/debug/unpack_args.test
...updated 5 targets...

Compilation finished at Sat Aug 29 06:39:10
    
#}---cut here---
