/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_test_itl_map_cases_hpp_JOFA_090701__
#define __itl_test_itl_map_cases_hpp_JOFA_090701__

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_itl_map_find_4_bicremental_types, T, discrete_types)
{         itl_map_find_4_bicremental_types<T, int, partial_absorber, INTERVAL_MAP>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_itl_map_inclusion_compare_4_bicremental_types, T, discrete_types)
{         itl_map_inclusion_compare_4_bicremental_types<T, int, partial_absorber, INTERVAL_MAP>();}

#endif // __itl_test_itl_map_cases_hpp_JOFA_090701__
