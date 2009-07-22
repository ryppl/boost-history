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

    BOOST_CHECK_EQUAL(map_a_b, map_b_a);

}
