/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2011: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

#ifndef BOOST_ICL_DOXYGEN_MAINPAGE_HPP_JOFA_091216
#define BOOST_ICL_DOXYGEN_MAINPAGE_HPP_JOFA_091216

/**
\mainpage Interval Container Library

\section header_sec Interval Container Library (aka. Interval Template Library)

Author: Joachim Faulhaber\n
Copyright (c) 2007-2011: Joachim Faulhaber\n
Copyright (c) 1999-2006: Cortex Software GmbH, Berlin\n

Distributed under the Boost Software License, Version 1.0.\n
(See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
\n \n

\section overview_sec Overview

The <b>Interval Container Library (ICL+)</b> 
formerly known as <b>Interval Template Library (ITL)</b>
is a collection of
generic c++ class templates for computations on intervals,
interval containers, large bitsets and generalized crosstables or cubes.

The complete library constists of three parts.

<ul>
<li><b>Core library (Boost.Icl):</b> Interval containers. 

The <b>core library</b> 
<a href="http://lists.boost.org/boost-announce/2010/04/0257.php">
is accepted into the Boost c++ Libraries Collection</a>
as <b>Interval Container Library Boost.Icl</b>.
A <b>detailed boost book documentation</b> for Boost.Icl is 
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/index.html">
online available here</a>.
We recommend to refer to this documentation for all informations related
to interval containers. The boost book documentation also ships with 
this release and can be found at "<boost_root>/libs/icl/doc/html/index.html".

<li><b>Extended library (icl_xt):</b>

The extended part contains class templates interval_bitset, product_history, and tupel_computer
(cubes). Those parts are not or not yet intended to be proposed for boost. 

<li><b>A Law Based Test Automaton (validate):</b>

The Law Based Test Automaton (also refered to as \b \i LaBatea) is a tool for an
automated teststing of code on the basis of laws or axioms. 
\b LaBatea has been used to validate the core library (icl)
for a number of laws.

</ul>


<b>This doxygen generated documentation</b> is provided for the 
additional library parts \b icl_xt and \b validate
that are not yet covered by a proper boost book documentation. 
\n \n


\section download_sec Download

You can download
<ul>
<li>The core library 
<a HREF="http://www.boostpro.com/vault/index.php?&direction=0&order=&directory=Containers"><b>ICL</b></a>
from the boost vault.
\n

<li>The full library 
<a HREF="http://sourceforge.net/projects/itl/"><b>ICL+</b> (core + extensions)</a>
from sourceforge.
</ul>
\n \n




\section news_sec News

<ul>

<li>Improvements for <b>Boost 1.46.0</b> (ICL+ version 4.0.1)

The <b>Interval Container Library, Boost.Icl</b> will ship with the next Boost release 1.46.0 around February-1-2011. This version of the <b>full library, ICL+ 4.0.1,</b> contains the newest core library and implements improvements for class <b>interval_bitset</b>, Boost.DateTime interoperability and improved portability for a wide range of compilers.

<li>Changes after the review (version 4.0.0)

A number of changes have been done after the review as a result of the
reviews and the related discussion.

<ul>

<li>New library name

The library's name has been changed form <b>Interval Template Library</b> 
to <b>Interval Container Library</b>. 

<li>Customizability

The library is now customizable for user defined interval types. A user
defined type can be registered and adapted by 
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/boost_icl/examples/custom_interval.html">
partially instantiating a small interval_traits class.</a>

<li>Intervals with static bounds

Intervals with static or fixed bounds can now be used with the library. 
Statically bounded intervals use less memory and perform 25% faster on average
than dynamically bounded intervals.

<li> Extraction of algorithms to concepts.

Most of the class templates member functions have been extracted to sets of namespace
global functions that represent <b>concepts</b>. Some functions have been renamed to
conform existing standards.

</ul>

<li>Interval_bitset.

The lastest addition of a container class template to the library is interval_bitsets.
Interval_bitsets use interval compression and bitset compression
to achieve a very efficient and compact representation of large
bitsets. The idea and implementation of interval_bitsets is
explained in the
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/boost_icl/projects.html#boost_icl.projects.large_bitset">
project section</a>
of the boost book documentation. 

<li>icl::add_iterator, and icl::insert_iterator

To copy or tranform data from std::containers of intervals or segments into
interval containers you can use an icl::insert_iterator or an icl::add_iterator.
See examples 
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/boost_icl/examples/std_copy.html">
std_copy</a> and 
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/boost_icl/examples/std_transform.html"> 
std_transform.</a>

<li>Element iteration.

The implementation of all interval containers including interval_bitsets
now provides iteration on the level of elements. This makes interval
containers a model of SortedAssociativeContainers.
</ul>
\n \n

\section example_sec Examples

We provide a collection of examples that demonstrate the purpose and basic
characteristics of the class templates offered by the library. To get an instant 
impression you may browse these examples first.

\subsection examples_icl_subsec Examples on Interval Containers (core library icl)

<ul>
<li>A <b>collection of examples</b> on interval containers is
included in the 
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/boost_icl/examples.html">
boost book documentation</a>.
</ul>
\n \n


\subsection examples_icl_xt_subsec Examples for the extended library (icl_xt)

<ul>
<li> <b>Interval Bitset</b>

A minimal implementation of \ref interval_bitset together with 
sample applications is included in the boost book documentation in the 
<a href="http://www.herold-faulhaber.de/boost_icl/doc/libs/icl/doc/html/boost_icl/projects.html#boost_icl.projects.large_bitset">
project section</a>. The full implementation of \ref interval_bitset is
not yet part of the core library (icl). It's in the extended part (icl_xt)
but it's integration into the core is intended.


<li> \b History

History is a more sophisticated application of an interval_map to decompose
data in time. Sample history.cpp shows how we can monitor a set of attributes in a history.
\n \n


<li> <b>Amount Cube</b>

Cubes or tuple computers allow to perform computations on values associated 
to tuples. A ICL tuple computer works similar to pivot tables (in spreadsheet programs)
olap cubes (online analytic processing) or crosstables (in statistical programs). 
Sample amount_cube.cpp presents an application where values are \e 'amounts', 
which is the simplest instance.
</ul>
\n \n

\subsection examples_validate_subsec Examples on law based Testing (validate)

<ul>
<li><b>De Morgan's law on interval sets</b>

This example de_morgan.cpp shows how to test a single law on given type. It shows
how <b>LaBatea</b> reports successful validation and what happens, if
law validations are found. The law can be modified to a valid form in
this example by choosing a different equality relation for one of the law's
templates parameters.

</ul>

*/
#endif // BOOST_ICL_DOXYGEN_MAINPAGE_HPP_JOFA_091216


