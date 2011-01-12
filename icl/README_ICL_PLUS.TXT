/*-----------------------------------------------------------------------------+
Interval Container Library (Boost.Icl)
Author: Joachim Faulhaber
Copyright (c) 2007-2011: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

Interval Container Library (Boost.Icl) 4.0.1 Inclusion Version for boost 1.46 
                                             (update 1) 
=============================================================================
PLUS 
Icl_xt    Extended by generalized cross tables (cubes) AND
validate  A law based test automaton (LaBatea).

Abstract:
=========

The Boost Interval Container Library (Icl) formerly known 
as Interval Template Library (Itl) provides intervals 
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

Icl_xt's cubes and the law based test automaton (LaBatea)
are not yet proposed and prepared as Boost libraries.
But LaBatea has been used in the development of the
icl as test automaton and for checking sematical 
concepts.


NOTE: The core library, Boost.Icl, has been accepted into 
the Boost Library Collection on April 18, 2010. This is 
the library version prepared for inclusion into Boost 1.46.0.

The extended parts icl_xt and validate are not Boost libraries.


Release Notes:
==============

* Boost.Icl 4.0.1 (Version for Boost 1.46.0 update 1)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4 thru gcc-4.5.0, 
                    msvc-7.1, msvc-8.0, msvc-9.0, msvc-10.0,
                    intel-10.1, 11.0, 11.1,
                    darwin-4.0.1, 4.2.1, 4.4.0,
                    (clang-2.9 *)
  see:http://www.boost.org/development/tests/release/developer/icl.html
  * For clang-2.9 compilation on most tests failed for features of the 
    tests. The library itself will probably compile

  The extended parts boost/icl_xt and boost/validate were tested with
  gcc and msvc compilers only.
   
+ Portability has been tested and improved by night tests of the boost
  test system.
  
+ Intervals and interval containers have been tested and improved to 
  work with intervals of pointers and intervals of date and time
  durations from Boost.DateTime.
  
+ Some errors in the documentation that were overlooked after changes 
  form version 3.2.1 to 4.0.0 have been fixed.
  
+ The quickbook-documents are built with a new quickbook version.

+ Some bugs due to changes in the extended library specifically in the
  interval_bitset class template have been fixed.
  
+ Functions and tests have been added for interval_bitset which is now
  compatible with other icl::sets w.r.t. the interface.


* Boost.Icl 4.0.0 (Inclusion Version for Boost 1.46.0)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.4.1, 
                    msvc-8.0, msvc-9.0, msvc-10.0.

+ The library has been renamed from
  Interval Template Library (ITL) to
  Interval Container Library (ICL)
  Its namespace is now icl instead of itl.
  
+ Introduction of dynamically bounded and statically bounded intervals
  - The 'universal' interval class itl::interval has been replaced by
  - dynamically bounded discrete_interval and continuous_interval
  - statically bounded right_open_interval, left_open_interval
    icl::closed_interval and icl::open_interval and a uniform set of
    namespace global functions or interval_concept for them.
  - Added functions 'distance' and 'inner_complement' for intervals.
    
+ User defined interval types are now customizable
  - A interval_trait and dynamic_interval_trait template describes
    the generic properties of the interval concept ...
  - and allows to customize the icl library for user defined interval 
    types.

+ Extraction of namespace global functions.
  - Member functions have been extracted form class templates as long
    as they are not needed to maintain type invariants.
  - The sets of namespace global functions build concepts
    icl::element_set, icl::element_map, 
    icl::interval_set, icl::interval_map
  - Functions have been renamed to conform existing standards
    (ISO/OGC).
  - std::set and other stl conformant set types are models of the
    icl's element_set concept. Class template icl::set has been 
    removed.
  - Member functions for element iterations are extracted from
    the class templates.
	
+ Names, refered to as 'colorful terminology' have been renamed
  - neutron -> identity_element
  - unon -> unit_element
  - ... and also their occurrence in composite identifiers


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

=== Icl =======================================================================						
icl.html                html-documentation referring to 
                        libs/icl/doc/html/index.html
doc/html/images         Images for the html docs. You may delete this, if you 
                        integrate icl_plus into an existing boost installation.

boost/icl               Interval containers, operators, algorithms
boost/icl/type_traits   Metafunctions for types
boost/icl/concept       Sets of namespace global functions that make up icl 
                        concepts.
boost/icl/predicates    Some boolean function objects
boost/icl/detail        Implementation details

libs/icl/example        Example programs
libs/icl/test           Test suite
libs/icl/doc            Quickbook documentatons
libs/icl/doc/html       html documentation from Quickbook sources for Boost.Icl
						
=== Icl extended ==============================================================						
boost/icl_xt            Headers of extended icl classes like cubes.
libs/icl_xt/example     Example programs
libs/icl_xt/test        Test programs

=== LaBatea ===================================================================
boost/validate          Headers of the law based test automaton (LaBatea).
libs/validate/example   Projects for validating Boost.Icl with LaBatea.
libs/validate/src       Sourcefiles.

=== Icl, Icl_xt and Validate =================================================
icl_plus_doxy.html      Html file that contains a link to the doygen docs:
libs/icl/doxy_doc/doxygen_output/html/index.html: html documentation generated 
                        by doxygen that covers a minimal documentation for 
                        icl_xt and validate (LaBatea).
icl.html                Html file that contains a link to the quickbook docs:
libs/icl/doc/html/index.html: html documentation generated by doxygen
                        that covers a minimal documentation for icl_xt and 
                        validate (LaBatea).

icl_plus and boost
==================
						
The interval template library comes in the boost centric project structure
<boost-root>/boost/icl/  contains all include files
<boost-root>/boost/icl/  examples, tests and documentation.
So you can integrate icl, icl_xt and validate into an existing boost 
installation by copying 
boost/{icl,icl_xt,validate} to the <boost-root>/boost directory and
libs/{icl,icl_xt,validate}  to the <boost-root>/libs directory.

In subdirectories like libs/icl/example, libs/icl/test you will find a
Jamfile.v2 that allows you to compile the related code with bjam.

To learn more about boost libraries and bjam refer to the boost web site.
http://www.boost.org/


icl_plus_standalone
===================

You can also download icl_plus_standalone. Included in icl_plus_standalone are
all sources from the boost libraries that are needed to compile icl code, 
examples and tests. For msvc-9 users it contains sln and vcproj files and a few
precompiled libraries needed by examples and test projects. If you are lucky
you can compile and run using

libs/icl/example/vc9_icl_examples.sln            Examples for the icl
libs/icl/test/vc9_icl_tests.sln                  Boost unit tests for the icl
libs/validate/examples/vc9_validate_examples.sln Law based tests for icl
libs/validate/test/vc9_validate_test.sln         Law based tests as unit tests
libs/icl/build/win32/vc9_all.sln                 Almost everything in one project

If this does not work out of the box, you can reinstall the boost date_time and
test binaries using

install_libs.bat

from the command line at the root directory of icl_plus_standalone.
If this still does not work, get a new version of bjam (boost-jam) at
http://sourceforge.net/projects/boost/files/boost-jam/

and consult documentations at
http://www.boost.org/

Enjoy
Joachim Faulhaber 

