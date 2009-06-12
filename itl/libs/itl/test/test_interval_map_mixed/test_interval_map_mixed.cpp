/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval_map_mixed unit test
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

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_map_mixed_ctor_4_ordered_types, T, ordered_types)
{
    typedef int U;
    typedef interval_map<T,U>       IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;

    T v0 = neutron<T>::value();
    U u1 = unon<U>::value();

    SplitIntervalMapT split_map(make_pair(v0,u1));
    IntervalMapT      join_map(split_map);

    BOOST_CHECK_EQUAL( split_map.lower(), join_map.lower() );
    BOOST_CHECK_EQUAL( split_map.upper(), join_map.upper() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_map_mixed_equal_4_ordered_types, T, ordered_types)
{         
    typedef int U;
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;

    T v0 = neutron<T>::value();
    U u1 = unon<U>::value();

    SplitIntervalMapT    split_empty, split_single(make_pair(v0,u1));
    IntervalMapT         join_empty, join_single(make_pair(v0,u1));

    // mixed ==-equality is a strange thing. Most times is does not
    // make sense. It is better to allow only for same type == equality.
    BOOST_CHECK_EQUAL( split_empty == split_empty, true );
    BOOST_CHECK_EQUAL( join_empty  == join_empty,  true );

    // There were Problems with operator== and emtpy sets.
    BOOST_CHECK_EQUAL( split_empty == split_single, false );
    BOOST_CHECK_EQUAL( join_empty  == join_single,  false );

    BOOST_CHECK_EQUAL( split_single == split_empty, false );
    BOOST_CHECK_EQUAL( join_single  == join_empty,  false );

    BOOST_CHECK_EQUAL( is_element_equal(split_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, join_empty),  true );

    BOOST_CHECK_EQUAL( is_element_equal(join_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, join_empty),  true );

    //--------------------------------------------------------------------------
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, join_single),  false );

    BOOST_CHECK_EQUAL( is_element_equal(join_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, join_single),  false );

    BOOST_CHECK_EQUAL( is_element_equal(split_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(split_single, join_empty),  false );

    BOOST_CHECK_EQUAL( is_element_equal(join_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(join_single, join_empty),  false );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_map_mixed_assign_4_ordered_types, T, ordered_types)
{         
    typedef int U;
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;

    T v0 = neutron<T>::value();
    T v1 = unon<T>::value();
    U u1 = unon<U>::value();

    mapping_pair<T,U> v0_u1(v0,u1);
    mapping_pair<T,U> v1_u1(v1,u1);

    SplitIntervalMapT split_map;
    IntervalMapT      join_map;
    split_map.add(v0_u1); //NOTE: make_pair(v0,u1); fails
    join_map = split_map; //=t T& T::operator=(const P&) ...


    BOOST_CHECK_EQUAL( split_map.lower(), join_map.lower() );
    BOOST_CHECK_EQUAL( split_map.upper(), join_map.upper() );

    SplitIntervalMapT split_self = SplitIntervalMapT().add(v0_u1);
    IntervalMapT      join_self  = IntervalMapT().add(v1_u1);

    split_self = split_self;
    join_self  = join_self;

    BOOST_CHECK_EQUAL( split_self, split_self );
    BOOST_CHECK_EQUAL( join_self, join_self );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_map_mixed_ctor_4_bicremental_types, T, bicremental_types)
{         
    typedef int U;
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);


    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);

    SplitIntervalMapT split_map;
    split_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    IntervalMapT join_map(split_map);
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_map_mixed_assign_4_bicremental_types, T, bicremental_types)
{         
    typedef int U;
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);

    SplitIntervalMapT split_map;
    split_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    BOOST_CHECK_EQUAL( split_map.iterative_size(), 4 );
    IntervalMapT join_map;
    join_map = split_map;
    BOOST_CHECK_EQUAL( join_map.iterative_size(), 3 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_map_mixed_equal_4_bicremental_types, T, bicremental_types)
{         
    typedef int U;
    typedef interval_map<T,U>        IntervalMapT;
    typedef split_interval_map<T,U> SplitIntervalMapT;
    U u1 = make<U>(1);

    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I4_5D = interval<T>::rightopen(v4,v5);

    std::pair<interval<T>,U> I1_3D_1(I1_3D, u1);
    std::pair<interval<T>,U> I2_4D_1(I2_4D, u1);
    std::pair<interval<T>,U> I4_5D_1(I4_5D, u1);

    IntervalMapT join_map;
    join_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    IntervalMapT join_map2 = join_map;    
    BOOST_CHECK_EQUAL( join_map, join_map2 );
    BOOST_CHECK_EQUAL( is_element_equal(join_map, join_map2), true );

    SplitIntervalMapT split_map;    
    split_map.add(I1_3D_1).add(I2_4D_1).add(I4_5D_1);
    SplitIntervalMapT split_map2 = split_map;    
    BOOST_CHECK_EQUAL( split_map, split_map2 );
    BOOST_CHECK_EQUAL( is_element_equal(split_map2, split_map), true );

    BOOST_CHECK_EQUAL( is_element_equal(split_map, join_map),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_map,  split_map), true );
}

