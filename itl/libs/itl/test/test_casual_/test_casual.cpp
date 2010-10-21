/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE icl::casual unit test
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
#include <boost/type_traits/is_const.hpp>
#include <boost/detail/is_incrementable.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/interprocess/containers/set.hpp>

#define BOOST_ICL_USE_STATIC_BOUNDED_INTERVALS
#define BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT right_open_interval
#define BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS 2 //0=() 1=(] 2=[) 3=[]

#include <boost/itl/interval_set.hpp>
#include <boost/itl/separate_interval_set.hpp>
#include <boost/itl/split_interval_set.hpp>
#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/itl/detail/element_iterator.hpp>
#include <boost/itl/detail/interval_morphism.hpp>
#include <boost/itl/ptime.hpp>
#include <boost/itl/type_traits/is_key_container_of.hpp>
#include <boost/itl/type_traits/codomain_type_of.hpp>
#include <boost/itl/type_traits/is_icl_container.hpp>

#include <boost/itl_xt/detail/bit_element_iterator.hpp>
#include <boost/itl_xt/interval_bitset.hpp>
#include <boost/itl_xt/list.hpp>
#include <boost/itl/set.hpp> //JODO

#include <limits>
#include <bitset>


using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::icl;


BOOST_AUTO_TEST_CASE(ad_hoc)
{
    std::set<int> set_a;
    set_a += 1;
    icl::add(set_a, 4);
    icl::add(set_a, 5);
    set_a -= 4;
    cout << set_a << endl;

    interval_set<int> itv_set_b;
    itv_set_b += interval<int>::right_open(1,3);

    std::set<int> set_b;

    segmental::atomize(set_b, itv_set_b);
    cout << set_b << endl;
}

BOOST_AUTO_TEST_CASE(intro_sample_42)
{
    interval_set<int> mySet;
    mySet.insert(42);
    bool has_answer = contains(mySet, 42);
}


BOOST_AUTO_TEST_CASE(intro_sample_telecast)
{
    // Switch on my favorite telecasts using an interval_set
    interval<int>::type news(2000, 2015);
    interval<int>::type talk_show(2245, 2330);
    interval_set<int> myTvProgram;
    myTvProgram.add(news).add(talk_show);

    // Iterating over elements (seconds) would be silly ...
    for(interval_set<int>::iterator telecast = myTvProgram.begin(); 
        telecast != myTvProgram.end(); ++telecast)
        //...so this iterates over intervals
        //TV.switch_on(*telecast);
        cout << *telecast;

    cout << endl;
}

BOOST_AUTO_TEST_CASE(interface_sample_identifiers)
{
    typedef interval_set<std::string, less, continuous_interval<std::string> > IdentifiersT;
    IdentifiersT identifiers, excluded;

    // special identifiers shall be excluded
    identifiers += continuous_interval<std::string>::right_open("a", "c");
    identifiers -= std::string("boost");
    cout << "identifiers: " << identifiers << endl;

    excluded = IdentifiersT(icl::hull(identifiers)) - identifiers;
    cout << "excluded   : " << excluded << endl;

    if(icl::contains(identifiers, std::string("boost")))
        cout << "error, identifiers.contains('boost')\n";
}


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


    BOOST_CHECK_EQUAL(0 != icl::infinity<int>::value(), true);
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

    BOOST_CHECK_EQUAL(icl::is_empty(L0T), false);
    BOOST_CHECK_EQUAL(icl::is_empty(C0T), true);
    BOOST_CHECK_EQUAL(icl::is_empty(L0D), true);
    BOOST_CHECK_EQUAL(icl::is_empty(C0D), true);


    continuous_interval<double> L0_1T = continuous_interval<double>(0.0, 1.0, interval_bounds::closed());
    continuous_interval<double> L1_2T = continuous_interval<double>(1.0, 2.0, interval_bounds::closed());
    BOOST_CHECK_EQUAL(icl::exclusive_less(L0_1T, L1_2T), false);
    BOOST_CHECK_EQUAL(icl::inner_bounds(L0_1T, L1_2T) == interval_bounds::open(), true);

    continuous_interval<double> L0_1D = continuous_interval<double>(0.0, 1.0, interval_bounds::right_open());
    BOOST_CHECK_EQUAL(icl::exclusive_less(L0_1D, L1_2T), true);
    BOOST_CHECK_EQUAL(icl::inner_bounds(L0_1D, L1_2T) == interval_bounds::right_open(), true);

    continuous_interval<double> C1_2T = continuous_interval<double>(1.0, 2.0, interval_bounds::left_open());
    BOOST_CHECK_EQUAL(icl::exclusive_less(L0_1T, C1_2T), true);
    BOOST_CHECK_EQUAL(icl::inner_bounds(L0_1T, C1_2T) == interval_bounds::left_open(), true);

    BOOST_CHECK_EQUAL(icl::exclusive_less(L0_1D, C1_2T), true);
    BOOST_CHECK_EQUAL(icl::inner_bounds(L0_1D, C1_2T) == interval_bounds::closed(), true);

    //BOOST_CHECK_EQUAL(icl::is_empty(conterval), true);
}

BOOST_AUTO_TEST_CASE(test_interval_bounds)
{
    continuous_interval<double> L0_1T = continuous_interval<double>(0.0, 1.0, interval_bounds::closed());
    continuous_interval<double> L0_1D = continuous_interval<double>(0.0, 1.0, interval_bounds::right_open());

    cout << "icl::right(L0_1T) = " <<  static_cast<int>(icl::right(L0_1T.bounds()).bits()) << endl;
    cout << "icl::right(L0_1D) = " <<  static_cast<int>(icl::right(L0_1D.bounds()).bits()) << endl;

    BOOST_CHECK_EQUAL(icl::right_bounds(L0_1D, L0_1T), interval_bounds::left_open());
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

    BOOST_CHECK_EQUAL((is_key_container_of<         int ,       icl::map<int,int> >::value), false);
    BOOST_CHECK_EQUAL((is_key_container_of<std::pair<int,int> , icl::map<int,int> >::value), false);
    BOOST_CHECK_EQUAL((is_key_container_of<icl::set<int>,       icl::set<int>     >::value), true);
    BOOST_CHECK_EQUAL((is_key_container_of<icl::set<int>,       icl::map<int,int> >::value), true);
    BOOST_CHECK_EQUAL((is_key_container_of<icl::map<int,int>,   icl::map<int,int> >::value), true);

    //BOOST_CHECK_EQUAL((is_element_container<icl::map<int,int> >::value), true);

    typedef icl::map<int,int> MapII;

    //const bool xx = is_same< typename icl::map<int,int>::codomain_type, 
    //    typename codomain_type_of<icl::map<int,int> >::type >::value;

    BOOST_CHECK_EQUAL(has_codomain_type<MapII>::value, true);
    BOOST_CHECK_EQUAL((is_same<MapII::codomain_type, int>::value), true);

    //BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII,true>::type, int>::value),  true);
    //BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII,false>::type, int>::value), false);
    //BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII,false>::type, icl::no_type>::value), true);

    //BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII, has_codomain_type<MapII>::value >::type, int>::value), true);
    //BOOST_CHECK_EQUAL((is_same<get_codomain_type<MapII, has_codomain_type<MapII>::value >::type, icl::no_type>::value), false);

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

#if defined(BOOST_ICL_USE_STATIC_BOUNDED_INTERVALS) && !defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT)
    BOOST_CHECK( (is_same<icl::interval<int   >::type, right_open_interval<int   > >::value) );
    BOOST_CHECK( (is_same<icl::interval<double>::type, right_open_interval<double> >::value) );

    BOOST_CHECK_EQUAL( icl::interval<int>::open(0,2),       icl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::left_open(0,1),  icl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::right_open(1,2), icl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::closed(1,1),     icl::construct<right_open_interval<int> >(1,2) );

    BOOST_CHECK_EQUAL( icl::interval<float>::right_open(1.0,2.0), icl::construct<right_open_interval<float> >(1.0,2.0) );
    //The next yields compiletime error: STATIC_ASSERTION_FAILURE
    //BOOST_CHECK_EQUAL( icl::interval<float>::left_open(1.0,2.0), icl::construct<right_open_interval<float> >(1.0,2.0) );
#endif

#if defined(BOOST_ICL_USE_STATIC_BOUNDED_INTERVALS) && defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT)
#   if defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 0)
    cout << "discrete_interval == open_interval\n";
    BOOST_CHECK( (is_same<icl::interval<int>::type, open_interval<int> >::value) );
    BOOST_CHECK_EQUAL( icl::interval<int>::open(0,2),       icl::construct<open_interval<int> >(0,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::left_open(0,1),  icl::construct<open_interval<int> >(0,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::right_open(1,2), icl::construct<open_interval<int> >(0,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::closed(1,1),     icl::construct<open_interval<int> >(0,2) );

#   elif defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 1)
    cout << "discrete_interval == left_open_interval\n";
    BOOST_CHECK( (is_same<icl::interval<int>::type, left_open_interval<int> >::value) );
    BOOST_CHECK_EQUAL( icl::interval<int>::open(0,2),       icl::construct<left_open_interval<int> >(0,1) );
    BOOST_CHECK_EQUAL( icl::interval<int>::left_open(0,1),  icl::construct<left_open_interval<int> >(0,1) );
    BOOST_CHECK_EQUAL( icl::interval<int>::right_open(1,2), icl::construct<left_open_interval<int> >(0,1) );
    BOOST_CHECK_EQUAL( icl::interval<int>::closed(1,1),     icl::construct<left_open_interval<int> >(0,1) );

#   elif defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 2)
    cout << "discrete_interval == right_open_interval\n";
    BOOST_CHECK( (is_same<icl::interval<int>::type, right_open_interval<int> >::value) );
    BOOST_CHECK_EQUAL( icl::interval<int>::open(0,2),       icl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::left_open(0,1),  icl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::right_open(1,2), icl::construct<right_open_interval<int> >(1,2) );
    BOOST_CHECK_EQUAL( icl::interval<int>::closed(1,1),     icl::construct<right_open_interval<int> >(1,2) );

#   elif defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS) && (BOOST_ICL_DISCRETE_STATIC_INTERVAL_BORDERS == 3)
    cout << "discrete_interval == closed_interval\n";
    BOOST_CHECK( (is_same<icl::interval<int>::type, closed_interval<int> >::value) );
    BOOST_CHECK_EQUAL( icl::interval<int>::open(0,2),       icl::construct<closed_interval<int> >(1,1) );
    BOOST_CHECK_EQUAL( icl::interval<int>::left_open(0,1),  icl::construct<closed_interval<int> >(1,1) );
    BOOST_CHECK_EQUAL( icl::interval<int>::right_open(1,2), icl::construct<closed_interval<int> >(1,1) );
    BOOST_CHECK_EQUAL( icl::interval<int>::closed(1,1),     icl::construct<closed_interval<int> >(1,1) );

#   else
    cout << "#else part, INTERVAL_BORDERS not in {0,1,2,3}\n";
#endif //defined(BOOST_ICL_USE_STATIC_BOUNDED_INTERVALS) && defined(BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT)

#else
    BOOST_CHECK( (is_same<icl::interval<int   >::type,   discrete_interval<int   > >::value) );
    BOOST_CHECK( (is_same<icl::interval<double>::type, continuous_interval<double> >::value) );
#endif

    cout << "--->\n";
    BOOST_CHECK_EQUAL( (is_set<std::set<int> >::value), true );
    BOOST_CHECK_EQUAL( (is_element_set<std::set<int> >::value), true );
    BOOST_CHECK_EQUAL( (is_map<std::set<int> >::value), false );

    BOOST_CHECK_EQUAL( (is_const<std::set<int>::key_type >::value),   false );
    BOOST_CHECK_EQUAL( (is_const<std::set<int>::value_type >::value), false );

    BOOST_CHECK_EQUAL( (is_std_set<interprocess::set<int> >::value), true );
    BOOST_CHECK_EQUAL( (is_element_set<interprocess::set<int> >::value), true );
}

