/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_FASTEST_INTERVAL_SET_CASES_HPP_JOFA_090702
#define BOOST_ICL_FASTEST_INTERVAL_SET_CASES_HPP_JOFA_090702

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_fundamentals_4_ordered_types)
{            interval_set_fundamentals_4_ordered_types<INTERVAL_SET, integral_type_1>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_ctor_4_bicremental_types)
{            interval_set_ctor_4_bicremental_types<INTERVAL_SET, integral_type_2>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_add_sub_4_bicremental_types)
{            interval_set_add_sub_4_bicremental_types<INTERVAL_SET, integral_type_3>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_distinct_4_bicremental_types)
{            interval_set_distinct_4_bicremental_types<INTERVAL_SET, integral_type_4>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_element_compare_4_bicremental_types)
{            interval_set_element_compare_4_bicremental_types<INTERVAL_SET, integral_type_5>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_contains_4_bicremental_types)
{            interval_set_contains_4_bicremental_types<INTERVAL_SET, integral_type_6>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_operators_4_bicremental_types)
{            interval_set_operators_4_bicremental_types<INTERVAL_SET, integral_type_1>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_base_intersect_4_bicremental_types)
{            interval_set_base_intersect_4_bicremental_types<INTERVAL_SET, integral_type_2>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_flip_4_bicremental_types)
{            interval_set_flip_4_bicremental_types<INTERVAL_SET, integral_type_3>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_bitset_find_4_integral_types)
{            interval_bitset_find_4_integral_types<INTERVAL_SET, integral_type_4>();}

BOOST_AUTO_TEST_CASE
(fastest_icl_interval_set_element_iter_4_discrete_types)
{            interval_set_element_iter_4_discrete_types<INTERVAL_SET, integral_type_5>();}

//JODO Check set_at, check for untested stuff


#endif // BOOST_ICL_FASTEST_INTERVAL_SET_CASES_HPP_JOFA_090702



