/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_test_interval_map_cases_hpp_JOFA_090701__
#define __itl_test_interval_map_cases_hpp_JOFA_090701__

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_fundamentals_4_ordered_types, T, ordered_types)
{         interval_map_fundamentals_4_ordered_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_ctor_4_bicremental_types, T, bicremental_types)
{         interval_map_ctor_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_add_sub_4_bicremental_types, T, bicremental_types)
{         interval_map_add_sub_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_distinct_4_bicremental_types, T, bicremental_types)
{         interval_map_distinct_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_distinct_4_bicremental_continuous_types, T, bicremental_continuous_types)
{         interval_map_distinct_4_bicremental_continuous_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_isolate_4_bicremental_continuous_types, T, bicremental_continuous_types)
{         interval_map_isolate_4_bicremental_continuous_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_contains_4_bicremental_types, T, bicremental_types)
{         interval_map_contains_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_contains_key_objects_4_bicremental_types, T, bicremental_types)
{         interval_map_contains_key_objects_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_operators_4_bicremental_types, T, bicremental_types)
{         interval_map_operators_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_base_intersect_4_bicremental_types, T, bicremental_types)
{         interval_map_base_intersect_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_base_erase_4_bicremental_types, T, bicremental_types)
{         interval_map_base_erase_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_base_is_disjoint_4_bicremental_types, T, bicremental_types)
{         interval_map_base_is_disjoint_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_flip_4_bicremental_types, T, bicremental_types)
{         interval_map_flip_4_bicremental_types<INTERVAL_MAP, T, int>();}
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_find_4_bicremental_types, T, bicremental_types)
{         interval_map_find_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_set_4_bicremental_types, T, bicremental_types)
{         interval_map_set_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_inclusion_compare_4_bicremental_types, T, bicremental_types)
{         interval_map_inclusion_compare_4_bicremental_types<T, int, partial_absorber, INTERVAL_MAP>();}

#endif // __itl_test_interval_map_cases_hpp_JOFA_090701__

