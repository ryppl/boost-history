/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

#ifndef BOOST_ITL_DOXYGEN_MAINPAGE_HPP_JOFA_091216
#define BOOST_ITL_DOXYGEN_MAINPAGE_HPP_JOFA_091216

/**
\mainpage Interval Template Library

\section header_sec Interval Template Library

Author: Joachim Faulhaber\n
Copyright (c) 2007-2009: Joachim Faulhaber\n
Copyright (c) 1999-2006: Cortex Software GmbH, Berlin\n

Distributed under the Boost Software License, Version 1.0.\n
(See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
\n \n

\section overview_sec Overview

The <b>Interval Template Library (ITL+)</b> is a collection of
generic c++ class templates for computations on intervals,
interval containers, large bitsets and generalized crosstables or cubes.

The complete library constists of three parts.

<ul>
<li><b>Core library (itl):</b> Interval containers. 

The <b>core library (ITL)</b> is currently submitted for review at the
<a href="http://www.boost.org/community/review_schedule.html">Boost c++ Libraries Collection</a>
as Boost.IntervalContainer Library.
A <b>detailed boost book documentation</b> for Boost.IntervalContainers is 
<a href="http://www.herold-faulhaber.de/boost_itl/doc/libs/itl/doc/html/index.html">
online available here</a>.
We recommend to refer to this documentation for all informations related
to interval containers. The boost book documentation also ships with 
this release and can be found at "<boost_root>/libs/itl/doc/html/index.html".

<li><b>Extended library (itl_xt):</b>

The extended part contains class templates interval_bitset, product_history, and tupel_computer
(cubes). Those parts are not or not yet intended to be proposed for boost. 

<li><b>A Law Based Test Automaton (validate):</b>

The Law Based Test Automaton (also refered to as \b \i LaBatea) is a tool for an
automated teststing of code on the basis of laws or axioms. 
\b LaBatea has been used to validate the core library (itl)
for a number of laws.

</ul>


<b>This doxygen generated documentation</b> is provided for the 
additional library parts \b itl_xt and \b validate
that are not yet covered by a proper boost book documentation. 
\n \n

\section news_sec News

<ul>

<li>Interval_bitset.

The lastest addition to the library are interval_bitsets.
Interval_bitsets use interval compression and bitset compression
to achieve a very efficient and compact representation of large
bitsets. The idea and implementation of interval_bitsets is
explained in the
<a href="http://www.herold-faulhaber.de/boost_itl/doc/libs/itl/doc/html/boost_itl/projects.html#boost_itl.projects.large_bitset">
project section</a>
of the boost book documentation. 

<li>itl::add_iterator, and itl::insert_iterator

To copy or tranform data from std::containers of intervals or segments into
interval containers you can use an itl::insert_iterator or an itl::add_iterator.
See examples 
<a href="http://www.herold-faulhaber.de/boost_itl/doc/libs/itl/doc/html/boost_itl/examples/std_copy.html">
std_copy</a> and 
<a href="http://www.herold-faulhaber.de/boost_itl/doc/libs/itl/doc/html/boost_itl/examples/std_transform.html"> 
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

\subsection examples_itl_subsec Examples on Interval Containers (core library itl)

<ul>
<li>A <b>collection of examples</b> on interval containers is
included in the 
<a href="http://www.herold-faulhaber.de/boost_itl/doc/libs/itl/doc/html/boost_itl/examples.html">
boost book documentation</a>.
</ul>
\n \n


\subsection examples_itl_xt_subsec Examples for the extended library (itl_xt)

<ul>
<li> <b>Interval Bitset</b>

A minimal implementation of \ref interval_bitset together with 
sample applications is included in the boost book documentation in the 
<a href="http://www.herold-faulhaber.de/boost_itl/doc/libs/itl/doc/html/boost_itl/projects.html#boost_itl.projects.large_bitset">
project section</a>. The full implementation of \ref interval_bitset is
not yet part of the core library (itl). It's in the extended part (itl_xt)
but it's integration into the core is intended.


<li> \b History

History is a more sophisticated application of an interval_map to decompose
data in time. Sample history.cpp shows how we can monitor a set of attributes in a history.
\n \n


<li> <b>Amount Cube</b>

Cubes or tuple computers allow to perform computations on values associated 
to tuples. A ITL tuple computer works similar to pivot tables (in spreadsheet programs)
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
#endif // BOOST_ITL_DOXYGEN_MAINPAGE_HPP_JOFA_091216


