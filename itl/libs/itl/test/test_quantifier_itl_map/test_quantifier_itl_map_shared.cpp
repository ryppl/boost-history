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
(test_itl_partial_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{                 quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, partial_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{                 quantifier_map_check_monoid_et_4_bicremental_types<T, int, partial_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{                 quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, partial_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{                 quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, partial_absorber, interval_map>();}

// x - x = 0 | partial absorber
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_quantifier_map_check_partial_invertive_monoid_plus_4_bicremental_types, T, bicremental_types)
{                 quantifier_map_check_partial_invertive_monoid_plus_4_bicremental_types<T, int, partial_absorber, interval_map>();}


//------------------------------------------------------------------------------
// total_absorber
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_monoid_et_4_bicremental_types<T, int, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, total_absorber, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, total_absorber, interval_map>();}

// (0-x) + x = 0 | total absorber
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_quantifier_map_check_abelian_group_plus_4_bicremental_types, T, bicremental_types)
{               quantifier_map_check_abelian_group_plus_4_bicremental_types<T, int, total_absorber, interval_map>();}

//------------------------------------------------------------------------------
// partial_enricher
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_enricher_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{                          quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, partial_enricher, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_enricher_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{                          quantifier_map_check_monoid_et_4_bicremental_types<T, int, partial_enricher, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_enricher_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{                          quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, partial_enricher, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_enricher_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{                          quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, partial_enricher, interval_map>();}

// x - x =p= 0 | partial enricher
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_partial_enricher_quantifier_map_check_partial_invertive_monoid_plus_prot_inv_4_bicremental_types, T, bicremental_types)
{                          quantifier_map_check_partial_invertive_monoid_plus_prot_inv_4_bicremental_types<T, int, partial_enricher, interval_map>();}


//------------------------------------------------------------------------------
// total_enricher
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_monoid_plus_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_monoid_plus_4_bicremental_types<T, std::string, total_enricher, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_monoid_et_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_monoid_et_4_bicremental_types<T, int, total_enricher, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_abelian_monoid_plus_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_abelian_monoid_plus_4_bicremental_types<T, std::string, total_enricher, interval_map>();}

BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_abelian_monoid_et_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_abelian_monoid_et_4_bicremental_types<T, int, total_enricher, interval_map>();}

// (0-x) + x =p= 0 | total absorber
BOOST_AUTO_TEST_CASE_TEMPLATE
(test_itl_total_enricher_quantifier_map_check_abelian_group_plus_prot_inv_4_bicremental_types, T, bicremental_types)
{                        quantifier_map_check_abelian_group_plus_prot_inv_4_bicremental_types<T, int, total_enricher, interval_map>();}


//            absorber      enricher
// partial    x - x == 0    x - x =p= 0   partiality of subtraction   
// total    (-x)+ x == 0  (-x)+ x =p= 0   totality   of subtraction

