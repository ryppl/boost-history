/*-----------------------------------------------------------------------------+
Interval Container Library (Boost.Icl)
Author: Joachim Faulhaber
Copyright (c) 2007-2010: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

Interval Container Library (Boost.Icl) 4.0.1 Inclusion Version for boost 1.46
                                             (update 1) 
=============================================================================
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


NOTE: Boost.Icl has been accepted into the Boost Library
Collection on April 18, 2010. This is the library version
(4.0.1) that is also included into Boost 1.46.0.



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
   
+ Portability has been tested and improved by night tests of the boost
  test system.
  
+ Intervals and interval containers have been tested and improved to 
  work with intervals of pointers and intervals of date and time
  durations from Boost.DateTime.
  
+ Some errors in the documentation that were overlooked after changes 
  form version 3.2.1 to 4.0.0 have been fixed.
  
+ The quickbook-documents are built with a new quickbook version.

* Boost.Icl 4.0.0 (Inclusion Version for Boost 1.46.0)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.5.0, 
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
	
+ Names, referred to as 'colorful terminology' have been renamed
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
+ Addition of element_iterators that allow for an iteration on the level
  of elements. This allows to execute std::algorithms on interval 
  containers of discrete domain types.
+ Updating quickbook documentation. Added examples std_transform and 
  std_copy that demonstrates the use of itl insert and add iterators
+ Adding a more complex example project "large_bitset" that implements
  a large compressed bitset class template using interval_map.

* Boost.Itl 3.1.0 (Review Version)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.1.0, gcc-4.3.2,
  msvc-8.0, msvc-9.0.
+ Improved efficiency of algorithms.
+ Documentation of complexity guarantees.
+ Completion of quickbook documentation.

* Boost.Itl 3.0.0 (Preview 4)
------------------------------------------------------------------------
+ Tested Compilers: gcc-3.4.4, gcc-4.3.0 and msvc-9.0.
+ Interoperability between interval containers,
  segments and elements.
+ Full operator support for addition, subtraction,
  intersection and symmetric difference.
+ Boost.Quickbook documentation.

Content:
========

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
libs/icl/doc            Quickbook documentation
libs/icl/doc/html       html documentation (Generated by Quickbook)


Enjoy
Joachim Faulhaber 
