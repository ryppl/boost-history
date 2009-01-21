/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_interval_map_base_laws_plus_4_bicremental_types, T, bicremental_types)
{         interval_map_base_laws_plus_4_bicremental_types<T, int, interval_map, split_interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_monoid_4_bicremental_types, T, bicremental_types)
{         quantifier_map_check_monoid_4_bicremental_types<T, int, neutron_emitter>();}

