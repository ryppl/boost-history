/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::casual unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/validate/type/nat.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

/*CL
template<template<class, class>class IsCombinable,
         class LeftT, class RightT>
void check_combinable(bool truth)
{
	bool is_combinable = IsCombinable<LeftT,RightT>::value;
	BOOST_CHECK_EQUAL(is_combinable, truth);
}
*/

template<template<class, class>class IsCombinable,
         class LeftT, class RightT>
void check_combinable(bool truth, const char* type_combi)
{
	std::string type_combination = type_combi;
	bool is_combinable = IsCombinable<LeftT,RightT>::value;
	std::string combination_result = is_combinable 
		? (is_combinable == truth ? type_combination : "NOT combinable: "+type_combination)
		: (is_combinable == truth ? type_combination : "IS combinable: "+type_combination);
	BOOST_CHECK_EQUAL(type_combination, combination_result);
}

BOOST_AUTO_TEST_CASE(casual_test)
{
    typedef int T;
    typedef itl::map<int,int>  ItlMapT;
    typedef interval_map<int,int,partial_enricher>  IntervalMapT;
    typedef split_interval_map<int,int>  SplitIntervalMapT;
    typedef interval_set<int> IntervalSetT;
    typedef split_interval_set<int> SplitIntervalSetT;
    
    SplitIntervalMapT map_a, map_b;
    map_a.add(IDv(8,9,1)).add(IIv(6,11,3));
	cout << ">>> map_a = " << map_a << endl;
    map_b.add(IDv(0,9,2)).add(IIv(3,6,1));
	cout << ">>> map_b = " << map_b << endl;

	SplitIntervalMapT map_a_b = map_a + map_b;
	SplitIntervalMapT map_b_a = map_b + map_a;
	cout << ">>> map_a_b = " << map_a_b << endl;
	cout << ">>> map_b_a = " << map_b_a << endl;

    IntervalSetT set_a;
	set_a.add(I_D(2,4));
	map_a -= set_a;
	SplitIntervalMapT map_c;
	map_c = map_a - set_a;

	BOOST_CHECK_EQUAL(is_interval_container<interval_set<int> >::value, true);
	BOOST_CHECK_EQUAL(is_interval_container<split_interval_set<int> >::value, true);
	BOOST_CHECK_EQUAL(is_interval_container<separate_interval_set<int> >::value, true);
	BOOST_CHECK_EQUAL(is_interval_container<std::string >::value, false);

	bool interval_set_companion 
		= is_interval_set_companion< interval_map<int,int>, interval_set<int> >::value;
	BOOST_CHECK_EQUAL(interval_set_companion, true);

	//--------------------------------------------------------------------------
	bool is_combinable_jS_e
		= is_intra_derivative<interval_set<int>, interval_set<int>::element_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_jS_e, true);

	bool is_combinable_zS_e
		= is_intra_derivative<separate_interval_set<int>, interval_set<int>::element_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_zS_e, true);

	bool is_combinable_sS_e
		= is_intra_derivative<split_interval_set<int>, interval_set<int>::element_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_sS_e, true);
	//--------------------------------------------------------------------------
	bool is_combinable_jS_i
		= is_intra_derivative<interval_set<int>, interval_set<int>::segment_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_jS_i, true);

	bool is_combinable_zS_i
		= is_intra_derivative<separate_interval_set<int>, interval_set<int>::segment_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_zS_i, true);

	bool is_combinable_sS_i
		= is_intra_derivative<split_interval_set<int>, interval_set<int>::segment_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_sS_i, true);
	//--------------------------------------------------------------------------
	bool is_combinable_jM_e
		= is_intra_derivative<interval_map<int,int>, interval_map<int,int>::element_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_jM_e, true);

	bool is_combinable_sM_e
		= is_intra_derivative<split_interval_map<int,int>, split_interval_map<int,int>::element_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_sM_e, true);
	//--------------------------------------------------------------------------
	bool is_combinable_jM_i
		= is_intra_derivative<interval_map<int,int>, interval_map<int,int>::segment_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_jM_i, true);

	bool is_combinable_sM_i
		= is_intra_derivative<split_interval_map<int,int>, split_interval_map<int,int>::segment_type>::value;
	BOOST_CHECK_EQUAL(is_combinable_sM_i, true);
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	typedef interval_set<int>           jS;
	typedef separate_interval_set<int>  zS;
	typedef split_interval_set<int>     sS;
	typedef interval_map<int,int>       jM;
	typedef split_interval_map<int,int> sM;

	typedef interval_set<int>::element_type            jS_e;
	typedef separate_interval_set<int>::element_type   zS_e;
	typedef split_interval_set<int>::element_type      sS_e;
	typedef interval_map<int,int>::element_type        jM_b;
	typedef split_interval_map<int,int>::element_type  sM_b;

	typedef interval_set<int>::segment_type            jS_i;
	typedef separate_interval_set<int>::segment_type   zS_i;
	typedef split_interval_set<int>::segment_type      sS_i;
	typedef interval_map<int,int>::segment_type        jM_p;
	typedef split_interval_map<int,int>::segment_type  sM_p;

	typedef interval_map<int,int>::domain_type         jM_e;
	typedef split_interval_map<int,int>::domain_type   sM_e;
	typedef interval_map<int,int>::interval_type       jM_i;
	typedef split_interval_map<int,int>::interval_type sM_i;

	//     
	// jS
	// zS


	check_combinable<is_intra_derivative, jS, jS_e>(true, "jS_jS_e");
	check_combinable<is_intra_derivative, zS, zS_e>(true, "zS_zS_e");
	check_combinable<is_intra_derivative, sS, sS_e>(true, "sS_sS_e");

	check_combinable<is_intra_derivative, jS, jS_i>(true, "jS_jS_i");
	check_combinable<is_intra_derivative, zS, zS_i>(true, "zS_zS_i");
	check_combinable<is_intra_derivative, sS, sS_i>(true, "sS_sS_i");

	check_combinable<is_intra_derivative, jM, jM_b>(true, "jM_jM_b");
	check_combinable<is_intra_derivative, sM, sM_b>(true, "sM_sM_b");

	check_combinable<is_intra_derivative, jM, jM_p>(true, "jM_jM_p");
	check_combinable<is_intra_derivative, sM, sM_p>(true, "sM_sM_p");

	check_combinable<is_cross_derivative, jM, jM_e>(true, "jM_jM_e");
	check_combinable<is_cross_derivative, sM, sM_e>(true, "sM_sM_e");

	check_combinable<is_cross_derivative, jM, jM_i>(true, "jM_jM_i");
	check_combinable<is_cross_derivative, sM, sM_i>(true, "sM_sM_i");

	check_combinable<is_interval_set_companion, jS, jS>(true, "jS_jS");
	check_combinable<is_interval_set_companion, jS, zS>(true, "jS_zS");
	check_combinable<is_interval_set_companion, jS, sS>(true, "jS_sS");

	check_combinable<is_interval_set_companion, zS, jS>(true, "zS_jS");
	check_combinable<is_interval_set_companion, zS, zS>(true, "zS_zS");
	check_combinable<is_interval_set_companion, zS, sS>(true, "zS_sS");

	check_combinable<is_interval_set_companion, sS, jS>(true, "sS_jS");
	check_combinable<is_interval_set_companion, sS, zS>(true, "sS_zS");
	check_combinable<is_interval_set_companion, sS, sS>(true, "sS_sS");

	check_combinable<is_interval_set_companion, jM, jS>(true, "jM_jS");
	check_combinable<is_interval_set_companion, jM, zS>(true, "jM_zS");
	check_combinable<is_interval_set_companion, jM, sS>(true, "jM_sS");

	check_combinable<is_interval_set_companion, sM, jS>(true, "sM_jS");
	check_combinable<is_interval_set_companion, sM, zS>(true, "sM_zS");
	check_combinable<is_interval_set_companion, sM, sS>(true, "sM_sS");



	//--------------------------------------------------------------------------
    BOOST_CHECK_EQUAL(map_a_b, map_b_a);

}
