/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
// partial_absorber
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{         quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, partial_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{         quantifier_map_check_monoid_et_4_bicremental_types<T, int, partial_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{         quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, partial_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{         quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, partial_absorber, interval_map>();}


//------------------------------------------------------------------------------
// total_absorber
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_monoid_plus_4_bicremental_types_ta, T, bicremental_types)
{         quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_monoid_et_4_bicremental_types_ta, T, bicremental_types)
{         quantifier_map_check_monoid_et_4_bicremental_types<T, int, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_abelian_monoid_plus_4_bicremental_types_ta, T, bicremental_types)
{         quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_abelian_monoid_et_4_bicremental_types_ta, T, bicremental_types)
{         quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_quantifier_map_check_abelian_group_plus_4_bicremental_types_ta, T, bicremental_types)
{         quantifier_map_check_abelian_group_plus_4_bicremental_types<T, int, total_absorber, interval_map>();}

