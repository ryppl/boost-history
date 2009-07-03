/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_test_interval_set_infix_cases_hpp_JOFA_090701__
#define __itl_test_interval_set_infix_cases_hpp_JOFA_090701__

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_infix_plus_overload_4_bicremental_types, T, bicremental_types)
{         interval_set_infix_plus_overload_4_bicremental_types<INTERVAL_SET, T>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_infix_pipe_overload_4_bicremental_types, T, bicremental_types)
{         interval_set_infix_pipe_overload_4_bicremental_types<INTERVAL_SET, T>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_infix_minus_overload_4_bicremental_types, T, bicremental_types)
{         interval_set_infix_minus_overload_4_bicremental_types<INTERVAL_SET, T>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_infix_et_overload_4_bicremental_types, T, bicremental_types)
{         interval_set_infix_et_overload_4_bicremental_types<INTERVAL_SET, T>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_set_infix_caret_overload_4_bicremental_types, T, bicremental_types)
{         interval_set_infix_caret_overload_4_bicremental_types<INTERVAL_SET, T>();}

#endif // __itl_test_interval_set_infix_cases_hpp_JOFA_090701__



