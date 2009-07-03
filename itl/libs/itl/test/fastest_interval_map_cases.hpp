/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_fastest_interval_map_cases_hpp_JOFA_090702__
#define __itl_fastest_interval_map_cases_hpp_JOFA_090702__

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_fundamentals_4_ordered_types)
{            interval_map_fundamentals_4_ordered_types<INTERVAL_MAP, ordered_type_1, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_ctor_4_bicremental_types)
{            interval_map_ctor_4_bicremental_types<INTERVAL_MAP, bicremental_type_1, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_add_sub_4_bicremental_types)
{            interval_map_add_sub_4_bicremental_types<INTERVAL_MAP, bicremental_type_2, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_distinct_4_bicremental_types)
{            interval_map_distinct_4_bicremental_types<INTERVAL_MAP, bicremental_type_3, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_distinct_4_bicremental_continuous_types)
{            interval_map_distinct_4_bicremental_continuous_types<INTERVAL_MAP, continuous_type_1, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_isolate_4_bicremental_continuous_types)
{            interval_map_isolate_4_bicremental_continuous_types<INTERVAL_MAP, continuous_type_2, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_contains_4_bicremental_types)
{            interval_map_contains_4_bicremental_types<INTERVAL_MAP, bicremental_type_4, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_operators_4_bicremental_types)
{            interval_map_operators_4_bicremental_types<INTERVAL_MAP, bicremental_type_5, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_base_intersect_4_bicremental_types)
{            interval_map_base_intersect_4_bicremental_types<INTERVAL_MAP, bicremental_type_6, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_base_erase_4_bicremental_types)
{            interval_map_base_erase_4_bicremental_types<INTERVAL_MAP, bicremental_type_7, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_base_is_disjoint_4_bicremental_types)
{            interval_map_base_is_disjoint_4_bicremental_types<INTERVAL_MAP, bicremental_type_8, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_flip_4_bicremental_types)
{            interval_map_flip_4_bicremental_types<INTERVAL_MAP, bicremental_type_1, int>();}
BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_find_4_bicremental_types)
{            interval_map_find_4_bicremental_types<INTERVAL_MAP, bicremental_type_2, int>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_interval_map_set_4_bicremental_types)
{            interval_map_set_4_bicremental_types<INTERVAL_MAP, bicremental_type_3, int>();}

#endif // __itl_fastest_interval_map_cases_hpp_JOFA_090702__

