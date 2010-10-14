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
#include <boost/detail/is_incrementable.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#define ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS
#define BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT closed_interval
#define BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS 3

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/itl/detail/element_iterator.hpp>
#include <boost/itl/ptime.hpp>
#include <boost/itl/type_traits/is_key_container_of.hpp>
#include <boost/itl/type_traits/codomain_type_of.hpp>
#include <boost/itl/type_traits/is_icl_container.hpp>

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
    map_a += make_pair(interval<int>::right_open(0,3),1);
    cout << map_a << endl;

    for(interval_map<int,int>::element_iterator elem = elements_begin(map_a);
        elem != elements_end(map_a); elem++)
        cout << "(" << elem->first << "," << elem->second << ")";
    cout << "\n-------------------------------------\n";

    std::pair<const int, int> search_pair(2,1);

    interval_map<int,int>::element_const_iterator found 
        = std::find(elements_begin(map_a), elements_end(map_a), search_pair);
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
    BOOST_CHECK_EQUAL(is_continuous_interval<continuous_interval<double> >::value, true);

    BOOST_CHECK_EQUAL((is_interval_set<interval_map<int,int> >::value), false);
    BOOST_CHECK_EQUAL((is_interval_map<interval_map<int,int> >::value), true);

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

typedef bool (*void_2_bool)();
bool callit(){ cout << "called\n"; return true; }
bool nope(){ cout << "nope\n"; return false; }

//template<class Type> struct functions
//{
//    typedef bool (*Type_to_bool)(Type);
//};
//
//template<class Type>
//bool minimum_is_less_than(Type value)
//{
//    cout << "minimum_is_less_than\n";
//    return (numeric_limits<Type>::min)() < value; 
//}
//
//template<class Type>
//bool always_true(Type)
//{ 
//    cout << "always_true\n";
//    return true; 
//}

BOOST_AUTO_TEST_CASE(casual)
{
    typedef int T;
    typedef int U;
    typedef interval_map<T,U, total_absorber> IntervalMapT;
    typedef interval_set<T>                   IntervalSetT;
    typedef IntervalMapT::interval_type       IntervalT;

    BOOST_CHECK_EQUAL((is_key_container_of<         int ,       itl::map<int,int> >::value), false);
    BOOST_CHECK_EQUAL((is_key_container_of<std::pair<int,int> , itl::map<int,int> >::value), false);
    BOOST_CHECK_EQUAL((is_key_container_of<itl::set<int>,       itl::set<int>     >::value), true);
    BOOST_CHECK_EQUAL((is_key_container_of<itl::set<int>,       itl::map<int,int> >::value), true);
    BOOST_CHECK_EQUAL((is_key_container_of<itl::map<int,int>,   itl::map<int,int> >::value), true);

    //BOOST_CHECK_EQUAL((is_element_container<itl::map<int,int> >::value), true);

    typedef itl::map<int,int> MapII;

    //const bool xx = is_same< typename itl::map<int,int>::codomain_type, 
    //    typename codomain_type_of<itl::map<int,int> >::type >::value;

    BOOST_CHECK_EQUAL(has_codomain_type<MapII>::value, true);
    BOOST_CHECK_EQUAL((is_same<MapII::codomain_type, int>::value), true);

    BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII,true>::type, int>::value),  true);
    BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII,false>::type, int>::value), false);
    BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII,false>::type, itl::no_type>::value), true);

    BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII, has_codomain_type<MapII>::value >::type, int>::value), true);
    BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII, has_codomain_type<MapII>::value >::type, itl::no_type>::value), false);

    BOOST_CHECK_EQUAL((is_map<MapII>::value), true);
    BOOST_CHECK_EQUAL((is_icl_container<MapII>::value), true);

    BOOST_CHECK_EQUAL((is_fragment_of<IntervalSetT::element_type, IntervalSetT>::value), true);
    BOOST_CHECK_EQUAL((is_fragment_of<IntervalSetT::segment_type, IntervalSetT>::value), true);
    //BOOST_CHECK_EQUAL((is_fragment_of<discrete_interval<T>, IntervalSetT>::value), true);
    BOOST_CHECK_EQUAL((is_fragment_of<double, IntervalSetT>::value), false);


    BOOST_CHECK_EQUAL((boost::detail::is_incrementable<int>::value), true);
    BOOST_CHECK_EQUAL((boost::detail::is_incrementable<double>::value), true);
    BOOST_CHECK_EQUAL((boost::detail::is_incrementable<std::string>::value), false);

    BOOST_CHECK_EQUAL((is_floating_point<long double>::value), true);
    BOOST_CHECK_EQUAL((is_floating_point<double>::value), true);
    BOOST_CHECK_EQUAL((is_floating_point<float>::value), true);


    BOOST_CHECK( (is_same<key_type_of<std::set<int> >::type, int>::value) );
    BOOST_CHECK( (is_same<value_type_of<std::set<int> >::type, int>::value) );

    BOOST_CHECK_EQUAL( is_std_set<std::set<int> >::value,      true  );
    BOOST_CHECK_EQUAL( is_std_set<interval_set<int> >::value,  false );
    BOOST_CHECK_EQUAL((is_std_set<std::map<int,int> >::value), false );
    BOOST_CHECK_EQUAL( is_element_set<std::set<int> >::value,  true  );
    BOOST_CHECK_EQUAL( is_interval_set<std::set<int> >::value, false );
    BOOST_CHECK_EQUAL( is_interval_set<std::set<int> >::value, false );

#if defined(ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS) && !defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT)
    BOOST_CHECK( (is_same<_interval<int   >::type, right_open_interval<int   > >::value) );
    BOOST_CHECK( (is_same<_interval<double>::type, right_open_interval<double> >::value) );

    BOOST_CHECK_EQUAL( _interval<int>::open(0,2),       itl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( _interval<int>::left_open(0,1),  itl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( _interval<int>::right_open(1,2), itl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( _interval<int>::closed(1,1),     itl::construct<right_open_interval<int> >(1,2) );

    BOOST_CHECK_EQUAL( _interval<float>::right_open(1.0,2.0), itl::construct<right_open_interval<float> >(1.0,2.0) );
    //The next yields compiletime error: STATIC_ASSERTION_FAILURE
    //BOOST_CHECK_EQUAL( _interval<float>::left_open(1.0,2.0), itl::construct<right_open_interval<float> >(1.0,2.0) );
#endif

#if defined(ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS) && defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT)
#   if defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 0)
    cout << "discrete_interval == open_interval\n";
    BOOST_CHECK( (is_same<_interval<int>::type, open_interval<int> >::value) );
    BOOST_CHECK_EQUAL( _interval<int>::open(0,2),       itl::construct<open_interval<int> >(0,2) );
    BOOST_CHECK_EQUAL( _interval<int>::left_open(0,1),  itl::construct<open_interval<int> >(0,2) );
    BOOST_CHECK_EQUAL( _interval<int>::right_open(1,2), itl::construct<open_interval<int> >(0,2) );
    BOOST_CHECK_EQUAL( _interval<int>::closed(1,1),     itl::construct<open_interval<int> >(0,2) );

#   elif defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 1)
    cout << "discrete_interval == left_open_interval\n";
    BOOST_CHECK( (is_same<_interval<int>::type, left_open_interval<int> >::value) );
    BOOST_CHECK_EQUAL( _interval<int>::open(0,2),       itl::construct<left_open_interval<int> >(0,1) );
    BOOST_CHECK_EQUAL( _interval<int>::left_open(0,1),  itl::construct<left_open_interval<int> >(0,1) );
    BOOST_CHECK_EQUAL( _interval<int>::right_open(1,2), itl::construct<left_open_interval<int> >(0,1) );
    BOOST_CHECK_EQUAL( _interval<int>::closed(1,1),     itl::construct<left_open_interval<int> >(0,1) );

#   elif defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 2)
    cout << "discrete_interval == right_open_interval\n";
    BOOST_CHECK( (is_same<_interval<int>::type, right_open_interval<int> >::value) );
    BOOST_CHECK_EQUAL( _interval<int>::open(0,2),       itl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( _interval<int>::left_open(0,1),  itl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( _interval<int>::right_open(1,2), itl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( _interval<int>::closed(1,1),     itl::construct<right_open_interval<int> >(1,2) );

#   elif defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 3)
    cout << "discrete_interval == closed_interval\n";
    BOOST_CHECK( (is_same<_interval<int>::type, closed_interval<int> >::value) );
    BOOST_CHECK_EQUAL( _interval<int>::open(0,2),       itl::construct<closed_interval<int> >(1,1) );
    BOOST_CHECK_EQUAL( _interval<int>::left_open(0,1),  itl::construct<closed_interval<int> >(1,1) );
    BOOST_CHECK_EQUAL( _interval<int>::right_open(1,2), itl::construct<closed_interval<int> >(1,1) );
    BOOST_CHECK_EQUAL( _interval<int>::closed(1,1),     itl::construct<closed_interval<int> >(1,1) );

#   else
    cout << "#else part, INTERVAL_BORDERS not in {0,1,2,3}\n";
#endif //defined(ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS) && defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT)

#else
    BOOST_CHECK( (is_same<_interval<int   >::type,   discrete_interval<int   > >::value) );
    BOOST_CHECK( (is_same<_interval<double>::type, continuous_interval<double> >::value) );
#endif
}

