/*-----------------------------------------------------------------------------+
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_fastest_set_itl_set_cases_hpp_JOFA_090703__
#define __itl_fastest_set_itl_set_cases_hpp_JOFA_090703__

//------------------------------------------------------------------------------
// interval_set
//------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE
(fastest_itl_itl_set_check_monoid_plus_4_bicremental_types)
{            itl_set_check_monoid_plus_4_bicremental_types<discrete_type_1, interval_set>();}
                                                   //MEMO: interval_set
// is used here pragmatically to be able to recycle test code for initializing
// sets. These interval_set are then converted to itl::set by atomize.

BOOST_AUTO_TEST_CASE
(fastest_itl_itl_set_check_abelian_monoid_plus_4_bicremental_types)
{            itl_set_check_abelian_monoid_plus_4_bicremental_types<discrete_type_2, interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_itl_set_check_abelian_monoid_et_4_bicremental_types)
{            itl_set_check_abelian_monoid_et_4_bicremental_types<discrete_type_3, interval_set>();}

BOOST_AUTO_TEST_CASE
(fastest_itl_itl_set_check_partial_invertive_monoid_plus_4_bicremental_types)
{            itl_set_check_partial_invertive_monoid_plus_4_bicremental_types<discrete_type_4, interval_set>();}

#endif // __itl_fastest_set_itl_set_cases_hpp_JOFA_090703__


