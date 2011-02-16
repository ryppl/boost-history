/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE icl::casual unit test
#include <libs/icl/test/disable_test_warnings.hpp>
#include <string>
#include <vector>
#include <boost/mpl/list.hpp>
#include "../unit_test_unwarned.hpp"
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/icl/gregorian.hpp>
#include <boost/icl/ptime.hpp>
#include <boost/icl/xint.hpp>

#include <boost/icl/interval_map.hpp>
#include <boost/icl/interval.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::icl;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace boost::xint;

BOOST_AUTO_TEST_CASE(casual)
{
    //typedef int T;
    //typedef int U;
    //typedef interval_map<T,U, total_absorber> IntervalMapT;
    //typedef interval_set<T>                   IntervalSetT;
    //typedef IntervalMapT::interval_type       IntervalT;
	namespace xopts = boost::xint::options;

	typedef boost::xint::integer_t<xopts::fixedlength<512>, xopts::secure, xopts::negative_modulus> uint512;
	typedef boost::xint::detail::integer_t_data
		<xopts::fixedlength<512>, xopts::secure, xopts::negative_modulus, 
		 parameter::void_, parameter::void_, parameter::void_> uint512_d;

	bool is_itg_xint = boost::is_integral<xint::integer>::value;

	typedef integer_t<>::datatype base_type;


	/*
	uint512* p_ui = new uint512; 
	(*p_ui) += 512;
	cout << (*p_ui) << endl;

	uint512_d* p_ui_d = p_ui;
	delete p_ui_d;
	*/

	unsigned int k = 2;
	k -= 3;
	cout << k << endl;

	//interval_set<int>* p_is = new interval_set<int>;
	//interval_set<int>::base_type* p_bis = p_is;
	//delete p_bis;

	//integer* p_itg = new integer; 
	//(*p_itg) += 42;
	//cout << (*p_itg) << endl;

	////boost::xint::detail::integer_t_data<>*
	//integer_t<>* p_itg_td = p_itg;
	//(*p_itg_td) += 1;
	//cout << (*p_itg_td) << endl;

	//delete p_itg_td;

	cout << "is_specialized: " 
		<< std::numeric_limits<boost::xint::integer>::is_specialized << endl;

    BOOST_CHECK_EQUAL(true, is_itg_xint);
}

