/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::casual unit test
#include <string>
#include <vector>
#include <boost/mpl/list.hpp>
#include <boost/itl/iterator.hpp>
#include "../unit_test_unwarned.hpp"
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/type_traits/is_same.hpp>

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/itl/detail/element_iterator.hpp>
#include <boost/itl_xt/detail/bit_element_iterator.hpp>
#include <boost/itl_xt/interval_bitset.hpp>
#include <boost/itl_xt/list.hpp>

#include <limits>
#include <bitset>


using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;

/*CL after Proto
BOOST_AUTO_TEST_CASE(reverse_iter)
{
    interval_map<int,int> map_a;
    map_a += make_pair(interval<int>::rightopen(0,3),1);
    cout << map_a << endl;

    for(interval_map<int,int>::element_iterator elem = map_a.elements_begin();
        elem != map_a.elements_end(); elem++)
        cout << "(" << elem->first << "," << elem->second << ")";
    cout << "\n-------------------------------------\n";

    std::pair<const int, int> search_pair(2,1);

    interval_map<int,int>::element_const_iterator found 
        = std::find(map_a.elements_begin(), map_a.elements_end(), search_pair);
    cout << "(" << found->first << "," << found->second << ")\n";

    const_cast<int&>(found->second) = 2;
    cout << map_a << endl;
}

#include <limits>
#include <complex>

BOOST_AUTO_TEST_CASE(digits_to_check_numerics) //JODO Proto: properties of infinity.
{
    cout << "--- limits ---\n";
    cout << "max<char>    = " << (std::numeric_limits<char>::max)() << endl;
    cout << "max<char>    = " << static_cast<int>((std::numeric_limits<char>::max)()) << endl;
    cout << "-----------------------------------\n";
    cout << "digits<char>    = " << std::numeric_limits<char>::digits << endl;
    cout << "digits<short>   = " << std::numeric_limits<short>::digits << endl;
    cout << "digits<float>   = " << std::numeric_limits<float>::digits << endl;
    cout << "digits<double>  = " << std::numeric_limits<double>::digits << endl;
    cout << "digits<complex<double>> = " << std::numeric_limits<std::complex<double> >::digits << endl;
    cout << "digits<string>  = " << std::numeric_limits<std::string>::digits << endl;


    BOOST_CHECK_EQUAL(0 != itl::infinity<int>::value(), true);
}
*/

#include <boost/itl/continuous_interval.hpp>
#include <boost/itl/type_traits/is_continuous_interval.hpp>


BOOST_AUTO_TEST_CASE(interval_type_traits)
{
    BOOST_CHECK_EQUAL(is_interval<continuous_interval<int> >::value, true);
    BOOST_CHECK_EQUAL(is_continuous_interval<continuous_interval<int> >::value, false);
    BOOST_CHECK_EQUAL(is_continuous_interval<continuous_interval<double> >::value, true);
}

/*JODO move to proper tests??
BOOST_AUTO_TEST_CASE(casual)
{
    cout << left_bracket(interval_bounds(closed_bounded)) << endl;
    cout << left_bracket(interval_bounds(right_open)) << endl;
    cout << left_bracket(interval_bounds(left_open)) << endl;
    cout << left_bracket(interval_bounds(open_bounded)) << endl;

    cout << right_bracket(interval_bounds(closed_bounded)) << endl;
    cout << right_bracket(interval_bounds(right_open)) << endl;
    cout << right_bracket(interval_bounds(left_open)) << endl;
    cout << right_bracket(interval_bounds(open_bounded)) << endl;

    //continuous_interval<double> a_1 = continuous_interval<double>(0.0, 3.0, interval_bounds::left_open());
    continuous_interval<double> a_1 = continuous_interval<double>(-5.0, -2.3, interval_bounds::closed());
    continuous_interval<double> b_1 = continuous_interval<double>(-2.6, 4.0, interval_bounds::closed());

    split_interval_set<double> a, b, a_o_b, b_o_a;
    a_o_b += a_1;
    a_o_b += b_1;

    b_o_a += b_1;
    b_o_a += a_1;

    cout << "a+b =" << a_o_b << endl;
    cout << "b+a =" << b_o_a << endl;

    cout << "-------------------------------------------------\n";
    continuous_interval<double> c_1 = continuous_interval<double>(1.0, 3.0, interval_bounds::closed());
    continuous_interval<double> b_2 = right_subtract(b_1, c_1);

    cout << b_2 << endl;

    cout << "-------------------------------------------------\n";
    continuous_interval<double> L0T = continuous_interval<double>(0.0, 0.0, interval_bounds::closed());
    continuous_interval<double> C0T = continuous_interval<double>(0.0, 0.0, interval_bounds::left_open());
    continuous_interval<double> L0D = continuous_interval<double>(0.0, 0.0, interval_bounds::right_open());
    continuous_interval<double> C0D = continuous_interval<double>(0.0, 0.0, interval_bounds::open());

    BOOST_CHECK_EQUAL(itl::is_empty(L0T), false);
    BOOST_CHECK_EQUAL(itl::is_empty(C0T), true);
    BOOST_CHECK_EQUAL(itl::is_empty(L0D), true);
    BOOST_CHECK_EQUAL(itl::is_empty(C0D), true);


    continuous_interval<double> L0_1T = continuous_interval<double>(0.0, 1.0, interval_bounds::closed());
    continuous_interval<double> L1_2T = continuous_interval<double>(1.0, 2.0, interval_bounds::closed());
    BOOST_CHECK_EQUAL(itl::exclusive_less(L0_1T, L1_2T), false);
    BOOST_CHECK_EQUAL(itl::inner_bounds(L0_1T, L1_2T) == interval_bounds::open(), true);

    continuous_interval<double> L0_1D = continuous_interval<double>(0.0, 1.0, interval_bounds::right_open());
    BOOST_CHECK_EQUAL(itl::exclusive_less(L0_1D, L1_2T), true);
    BOOST_CHECK_EQUAL(itl::inner_bounds(L0_1D, L1_2T) == interval_bounds::right_open(), true);

    continuous_interval<double> C1_2T = continuous_interval<double>(1.0, 2.0, interval_bounds::left_open());
    BOOST_CHECK_EQUAL(itl::exclusive_less(L0_1T, C1_2T), true);
    BOOST_CHECK_EQUAL(itl::inner_bounds(L0_1T, C1_2T) == interval_bounds::left_open(), true);

    BOOST_CHECK_EQUAL(itl::exclusive_less(L0_1D, C1_2T), true);
    BOOST_CHECK_EQUAL(itl::inner_bounds(L0_1D, C1_2T) == interval_bounds::closed(), true);

    //BOOST_CHECK_EQUAL(itl::is_empty(conterval), true);
}

BOOST_AUTO_TEST_CASE(test_interval_bounds)
{
    continuous_interval<double> L0_1T = continuous_interval<double>(0.0, 1.0, interval_bounds::closed());
    continuous_interval<double> L0_1D = continuous_interval<double>(0.0, 1.0, interval_bounds::right_open());

    cout << "itl::right(L0_1T) = " <<  static_cast<int>(itl::right(L0_1T.bounds()).bits()) << endl;
    cout << "itl::right(L0_1D) = " <<  static_cast<int>(itl::right(L0_1D.bounds()).bits()) << endl;

    BOOST_CHECK_EQUAL(itl::right_bounds(L0_1D, L0_1T), interval_bounds::left_open());
}

BOOST_AUTO_TEST_CASE(casual)
{
    separate_interval_set<double> a, b;

    continuous_interval<double> L0_1T = continuous_interval<double>(0.0, 1.0, interval_bounds::closed());
    continuous_interval<double> L0_1D = continuous_interval<double>(0.0, 1.0, interval_bounds::right_open());

    a += L0_1T;
    a += L0_1D;
    cout << a << endl;

    b += L0_1D;
    b += L0_1T;
    cout << b << endl;

    cout << "double has " 
         << (numeric_limits<double>::has_infinity ? "inf: " : "NO inf: ") 
         << numeric_limits<double>::infinity()
         << endl;
}
*/


BOOST_AUTO_TEST_CASE(casual)
{
    typedef itl::set<int> TargetT;
    TargetT left, right, target;
    left.insert(1);
    left.insert(3);
    right.insert(2);
    left.insert(3);

    /*
    std::set_union(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
                   std::inserter(target,target.end()));
       std::set_difference(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
                        std::inserter(target,target.end()));

    std::set_intersection(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
                          std::inserter(target,target.end()));
    std::set_symmetric_difference(left.rbegin(), left.rend(), right.rbegin(), right.rend(), 
                                  std::inserter(target,target.end()));

    bool incl = std::includes(left.rbegin(), left.rend(), right.rbegin(), right.rend(), std::greater<int>()); //!

    */

    //int found;
    //TargetT::reverse_iterator it = std::find(left.rbegin(), left.rend(), found); //!

    bool incl = std::includes(left.rbegin(), left.rend(), right.rbegin(), right.rend(), std::greater<int>());

}

