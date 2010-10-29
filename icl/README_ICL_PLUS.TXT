/*-----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2010: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

Interval Template Library (Boost.Itl) 3.2.1 Review Version Update 2
===================================================================
PLUS 
Itl_xt    Extended by generalized cross tables (cubes) AND
validate  A law based test automaton (LaBatea).

Abstract:
=========

The Interval Template Library (Itl) provides intervals 
and two kinds of interval containers: Interval_sets and 
interval_maps. Interval_sets and maps can be used just
as the sets or maps of elements. Yet they are much more 
space and time efficient when the elements occur in 
contiguous chunks. This is obviously the case in many 
problem domains, particularly in fields that deal with 
problems related to date and time.

In addition to common set and map semantics, interval
containers allow to compute with segmentation.
A segmentation, e.g. a grid of months, can be intersected
into other interval containers and then be iterated over.

Finally interval_maps allow for aggregations on 
associated values, if added intervals overlap with
intervals, that are stored in the interval_map. This 
feature is called aggregate on overlap.

Itl_xt's cubes and the law based test automaton (LaBatea)
are not yet proposed and prepared as Boost libraries.
But LaBatea has been used in the development of the
itl as test automaton and for checking sematical 
concepts.

NOTE: Neither of the tree libraries itl, itl_xt and validate
is an official boost library yet. They are named Boost.Itl,
Boost.Itl_xt and Boost.Validate here, because they are already
organized in the boost centric directory structure.

The core library Boost.Itl is submitted for review which
will probably take place end of February 2010.

Release Notes:
==============

* Boost.Itl 3.2.1 (Review Version, update 2)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.3.2, gcc-4.4.1, 
                    msvc-8.0, msvc-9.0, msvc-10.0.
+ Some additions and corrections of the documentation.
+ Added file impl_config.hpp and a macro ITL_IMPL_SPACE that allows to
  switch the implementing set amd map containers between the 
  std and the boost::interprocess implementation.

* Boost.Itl 3.2.0 (Review Version, update 1)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.3.2, msvc-8.0, msvc-9.0, msvc-10.0.
+ Addition of itl::insert_iterator and itl::add_iterator that allow to 
  copy or transform std::containers of intervals or segments into 
  interval containers.
+ Addition of element_iterators that enable an iteration on the level
  of elements. This allows to execute std::algorithms on interval 
  containers of discrete domain types.
+ Updating quickbook documentation. Added examples std_transform and 
  std_copy that demonstrates the use of itl insert and add iterators
+ Adding a more complex example project "large_bitset" that implement
  large compressed bitsets class template using interval_map.

* Boost.Itl_xt 3.2.0
------------------------------------------------------------------------
+ Addition of a class template interval_bitset that implements a 
  large compressed bitset using an interval_map.

* Boost.Validate 3.2.1
------------------------------------------------------------------------
+ Addition of a commented example project de_morgan.cpp.
+ Addition of some basic doxygen generating comments to provide minimal
  documentation.

* Boost.Validate 3.2.0
------------------------------------------------------------------------
+ Addition of a law 'AtomicEquivalence' to validate the equivalence of
  std::algorithms on element and interval containers.
+ Addition of law based tests validating itl::insert and add iterators
  element_iterators and the implementation of interval_bitset.

========================================================================


* Boost.Itl 3.1.0 (Review Version)
------------------------------------------------------------------------
+ Tested Compilers: 
  Itl: gcc-3.4.4, gcc-4.1.0, gcc-4.3.2, msvc-8.0, msvc-9.0.
  Validate (LaBatea): gcc-4.3.2, msvc-8.0, msvc-9.0.
  LaBatea does not compile with gcc-4.1.0 or below.
+ Improved efficiency of algorithms.
+ Documentation of complexity guarantees.
+ Completion of quickbook documentation.
+ LaBatea now measures time per test.

* Boost.Itl 3.0.0 (Preview 4)
------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.3.0 and msvc-9.0.
+ Interoperability between interval containers,
  segments and elements.
+ Full operator support for addition, subtraction,
  intersection and symmetric difference.
+ Boost.Quickbook documentation.

Content:
========

=== Itl =======================================================================						
itl.html                html-documentation referring to 
                        libs/itl/doc/html/index.html
doc/html/images         Images for the html docs. You may delete this, if you 
                        integrate itl_plus into an existing boost installation.

boost/itl               Interval containers, operators, algorithms
boost/itl/type_traits   Metafunctions for types
boost/itl/detail        Implementation details

libs/itl/example        Example programs
libs/itl/test           Test suite
libs/itl/doc            Quickbook documentatons
libs/itl/doc/html       html documentation from Quickbook sources for Boost.Itl
libs/itl/build/win32    vc9_all.sln. solution file for msvc-9 including LaBatea 
                        programs.
						
=== Itl extended ==============================================================						
boost/itl_xt            Headers of extended itl classes like cubes.
libs/itl_xt/example     Example programs
libs/itl_xt/test        Test programs

=== LaBatea ===================================================================
boost/validate          Headers of the law based test automaton (LaBatea).
libs/validate/example   Projects for validating Boost.Itl with LaBatea.
libs/validate/src       Sourcefiles.

=== Itl, Itl_xt and Validate =================================================
itl_plus_doxy.html      Html file that contains a link to the doygen docs:
libs/itl/doxy_doc/doxygen_output/html/index.html: html documentation generated 
                        by doxygen that covers a minimal documentation for 
                        itl_xt and validate (LaBatea).
itl.html                Html file that contains a link to the quickbook docs:
libs/itl/doc/html/index.html: html documentation generated by doxygen
                        that covers a minimal documentation for itl_xt and 
                        validate (LaBatea).

itl_plus and boost
==================
						
The interval template library comes in the boost centric project structure
<boost-root>/boost/itl/  contains all include files
<boost-root>/boost/itl/  examples, tests and documentation.
So you can integrate itl, itl_xt and validate into an existing boost 
installation by copying 
boost/{itl,itl_xt,validate} to the <boost-root>/boost directory and
libs/{itl,itl_xt,validate}  to the <boost-root>/libs directory.

In subdirectories like libs/itl/example, libs/itl/test you will find a
Jamfile.v2 that allows you to compile the related code with bjam.

To learn more about boost libraries and bjam refer to the boost web site.
http://www.boost.org/


itl_plus_standalone
===================

You can also download itl_plus_standalone. Included in itl_plus_standalone are
all sources from the boost libraries that are needed to compile itl code, 
examples and tests. For msvc-9 users it contains sln and vcproj files and a few
precompiled libraries needed by examples and test projects. If you are lucky
you can compile and run using

libs/itl/example/vc9_itl_examples.sln            Examples for the itl
libs/itl/test/vc9_itl_tests.sln                  Boost unit tests for the itl
libs/validate/examples/vc9_validate_examples.sln Law based tests for itl
libs/validate/test/vc9_validate_test.sln         Law based tests as unit tests
libs/itl/build/win32/vc9_all.sln                 Almost everything in one project

If this does not work out of the box, you can reinstall the boost date_time and
test binaries using

install_libs.bat

from the command line at the root directory of itl_plus_standalone.
If this still does not work, get a new version of bjam (boost-jam) at
http://sourceforge.net/projects/boost/files/boost-jam/

and consult documentations at
http://www.boost.org/

Enjoy
Joachim Faulhaber 

