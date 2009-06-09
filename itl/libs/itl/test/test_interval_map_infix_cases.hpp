/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_infix_plus_overload_4_bicremental_types, T, bicremental_types)
{         interval_map_infix_plus_overload_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_infix_pipe_overload_4_bicremental_types, T, bicremental_types)
{         interval_map_infix_pipe_overload_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_infix_minus_overload_4_bicremental_types, T, bicremental_types)
{         interval_map_infix_minus_overload_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_infix_et_overload_4_bicremental_types, T, bicremental_types)
{         interval_map_infix_et_overload_4_bicremental_types<INTERVAL_MAP, T, int>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_infix_caret_overload_4_bicremental_types, T, bicremental_types)
{         interval_map_infix_caret_overload_4_bicremental_types<INTERVAL_MAP, T, int>();}
