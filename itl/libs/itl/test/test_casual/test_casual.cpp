/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval_set unit test
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



BOOST_AUTO_TEST_CASE(subset_test)
{
	typedef interval_map<int,int>  IntervalMapT;
	typedef split_interval_map<int,int>  SplitIntervalMapT;
	
	SplitIntervalMapT left, right;
	left.add(CDv(2,7,5)).add(IIv(9,9,5));
	right.add(CDv(-8,-6,2)).add(CIv(2,7,5)).add(CDv(7,9,1)).add(IIv(9,11,5));
	BOOST_CHECK_EQUAL(is_contained_in(left,right), true);

	left.clear(); right.clear();
	left.add(CIv(-4,-2,1)).add(IIv(8,8,3));
	right.add(CIv(-6,-1,1)).add(CIv(6,9,3));
	is_contained_in(left,right);
	BOOST_CHECK_EQUAL(is_contained_in(left,right), true);
}

BOOST_AUTO_TEST_CASE(superset_test)
{
	typedef interval_map<int,int>  IntervalMapT;
	typedef split_interval_map<int,int>  SplitIntervalMapT;
	
	SplitIntervalMapT left, right;
	left.add(IDv(-7,-1,1)).add(IIv(8,16,9));
	right.add(CDv(-8,-4,1)).add(IIv(8,8,9));
	BOOST_CHECK_EQUAL(contains(left,right), true);
}

BOOST_AUTO_TEST_CASE(casual_test)
{
	typedef int T;
	typedef itl::map<int,int>  ItlMapT;
	typedef interval_map<int,int>  IntervalMapT;
	typedef split_interval_map<int,int>  SplitIntervalMapT;
	
	IntervalMapT left, right;

	left.add(IDv(1,3,1));
	BOOST_CHECK_EQUAL(IntervalMapT().add(IDv(1,3,1))(2), 1);
	BOOST_CHECK_EQUAL(IntervalMapT().add(IDv(1,3,1))(3) == 1, false);
	BOOST_CHECK_EQUAL(IntervalMapT().add(IDv(1,3,1))(3), 0);
	BOOST_CHECK_EQUAL(SplitIntervalMapT().add(IDv(1,3,1))(2), 1);
	BOOST_CHECK_EQUAL(SplitIntervalMapT().add(IDv(1,3,1))(3) == 1, false);
	BOOST_CHECK_EQUAL(ItlMapT().add(sK_v(2,1))(2), 1);
	BOOST_CHECK_EQUAL(ItlMapT().add(sK_v(2,1))(3) == 1, false);
}
