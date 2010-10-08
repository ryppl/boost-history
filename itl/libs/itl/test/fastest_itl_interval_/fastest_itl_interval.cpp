/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval unit test
#include <string>
#include <boost/mpl/list.hpp>
#include "../unit_test_unwarned.hpp"

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

#include "../test_itl_interval_shared.hpp"
#include "../test_icl_interval.hpp"
#include "../test_icl_dynamic_interval.hpp"
#include "../test_icl_discrete_interval.hpp"
#include "../test_icl_continuous_interval.hpp"
#include "../test_icl_static_interval.hpp"

#include <boost/itl/right_open_interval.hpp>
#include <boost/itl/left_open_interval.hpp>
#include <boost/itl/closed_interval.hpp>
#include <boost/itl/open_interval.hpp>

#include <boost/itl/discrete_interval.hpp>
#include <boost/itl/continuous_interval.hpp>

//==============================================================================
//= Traits
//==============================================================================
BOOST_AUTO_TEST_CASE
(fastest_itl_discrete_interval_traits)
{            discrete_interval_traits<discrete_type_1, discrete_interval<discrete_type_1> >(); }

//==============================================================================

//- sta.asy.{dis|con} ----------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_right_open_interval_ctor_4_ordered_types)
{                      interval_ctor_4_ordered_types<right_open_interval<ordered_type_1> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_right_open_interval_4_ordered_types)
{         singelizable_interval_4_ordered_types<right_open_interval<discrete_type_1> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_right_open_interval_4_bicremental_types)
{         singelizable_interval_4_bicremental_types<right_open_interval<discrete_type_2> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_left_open_interval_ctor_4_ordered_types)
{                     interval_ctor_4_ordered_types<left_open_interval<ordered_type_2> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_left_open_interval_4_ordered_types_singelizable)
{        singelizable_interval_4_ordered_types<left_open_interval<signed_discrete_type_1> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_left_open_interval_4_bicremental_types)
{        singelizable_interval_4_bicremental_types<left_open_interval<discrete_type_4> >(); }


//- dyn.dis --------------------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_discrete_interval_ctor_4_discrete_types_base)
{                     interval_ctor_4_ordered_types<discrete_interval<discrete_type_1> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_discrete_interval_ctor_4_discrete_types_dynamic)
{             dynamic_interval_ctor_4_ordered_types<discrete_interval<discrete_type_2> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_discrete_interval_4_ordered_types)
{        singelizable_interval_4_ordered_types<discrete_interval<discrete_type_3> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_discrete_interval_4_bicremental_types)
{        singelizable_interval_4_bicremental_types<discrete_interval<discrete_type_3> >(); }

//- dyn.con --------------------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_continuous_interval_ctor_4_continuous_types_base)
{                       interval_ctor_4_ordered_types<continuous_interval<continuous_type_1> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_continuous_interval_ctor_4_continuous_types_dynamic)
{               dynamic_interval_ctor_4_ordered_types<continuous_interval<continuous_type_2> >(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_continuous_interval_4_continuous_types_singelizable)
{          singelizable_interval_4_ordered_types<continuous_interval<continuous_type_3> >(); }

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE
(fastest_itl_distant_intervals_4_discrete_types)
{            distant_intervals_4_discrete_types<discrete_type_1, std::less>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_distant_intervals_4_numeric_continuous_types)
{            distant_intervals_4_numeric_continuous_types<numeric_continuous_type_1, std::less>(); }

/*
BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_4_bicremental_types)
{            interval_ctor_4_bicremental_types<bicremental_type_2>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_4_integral_types)
{            interval_ctor_4_integral_types<integral_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_ctor_specific)
{            interval_ctor_specific(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_equal_4_integral_types)
{            interval_equal_4_integral_types<integral_type_2>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_less_4_integral_types)
{            interval_less_4_integral_types<integral_type_3>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_equal_4_bicremental_continuous_types)
{            interval_equal_4_bicremental_continuous_types<bicremental_continuous_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_touches_4_bicremental_types)
{            interval_touches_4_bicremental_types<bicremental_type_1>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_touches_4_integral_types)
{            interval_touches_4_integral_types<integral_type_4>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_inplace_intersect_4_bicremental_types)
{            interval_inplace_intersect_4_bicremental_types<bicremental_type_3>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_infix_intersect_4_bicremental_types)
{            interval_infix_intersect_4_bicremental_types<bicremental_type_4>(); }

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_subtract_4_bicremental_types)
{            interval_subtract_4_bicremental_types<bicremental_type_5>(); }
*/

/*JODO
BOOST_AUTO_TEST_CASE
(fastest_itl_interval_inner_complement_4_bicremental_types)
{            interval_inner_complement_4_bicremental_types<bicremental_type_6>(); }
*/
