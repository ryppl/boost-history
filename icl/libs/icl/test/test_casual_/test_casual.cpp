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
//#include <boost/icl/interval_map.hpp>
#include <boost/icl/interval.hpp>


using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::icl;
using namespace boost::posix_time;
using namespace boost::gregorian;

void time_test()
{
    ptime t_a = time_from_string("2010-12-24 19:30");
    ptime t_b = time_from_string("2010-12-24 20:30");
    time_duration a_2_b = t_b - t_a;
    cout << "Duration of [" << t_a << "," << t_b << ") = " 
         << a_2_b << endl;  

    interval<ptime>::type a_b 
        = interval<ptime>::right_open(t_a, t_b);
    cout << "Duration of " << a_b << " = " 
         << icl::size(a_b) << endl;

	icl::size(a_b);

    time_duration half = a_2_b / 2;
    ptime t_m = t_a + half;

    cout << a_b << (icl::contains(a_b, t_m) ? 
                    " contains " : "does not contain ") 
         << t_m << endl;

	ptime day_begin = time_from_string("2010-12-24 00:00");
	time_duration d_a = t_a - day_begin;
	time_duration d_b = t_b - day_begin;
	time_duration d_m = d_b - d_a;

	interval<time_duration>::type rel_a_b 
        = interval<time_duration>::right_open(d_a, d_b);

	time_duration sz_a_b_1 = d_b - d_a;
	time_duration sz_a_b_2 = icl::size(rel_a_b);
	cout << "b-a         = " << sz_a_b_1 << endl;
	cout << "size([a,b)) = " << sz_a_b_2 << endl;
	cout << "size([a,b)) = " << (icl::size(rel_a_b)) << endl;

	cout << rel_a_b << " " << (icl::size(rel_a_b)) << endl;
}

BOOST_AUTO_TEST_CASE(casual)
{
    //typedef int T;
    //typedef int U;
    //typedef interval_map<T,U, total_absorber> IntervalMapT;
    //typedef interval_set<T>                   IntervalSetT;
    //typedef IntervalMapT::interval_type       IntervalT;

    time_test();

    BOOST_CHECK_EQUAL(true, true);
}

