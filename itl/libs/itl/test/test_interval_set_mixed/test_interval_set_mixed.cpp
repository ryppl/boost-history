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

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_ctor_4_ordered_types, T, ordered_types)
{         
    T v0 = neutron<T>::value();
    T v1 = unon<T>::value();

    split_interval_set<T>    split_set(v0);
    separate_interval_set<T> sep_set(split_set);
    interval_set<T>          join_set(sep_set);

    BOOST_CHECK_EQUAL( split_set.lower(), sep_set.lower() );
    BOOST_CHECK_EQUAL( split_set.lower(), join_set.lower() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_equal_4_ordered_types, T, ordered_types)
{         
    T v0 = neutron<T>::value();
    T v1 = unon<T>::value();

    split_interval_set<T>    split_empty, split_single(v0);
    separate_interval_set<T> sep_empty, sep_single(v0);
    interval_set<T>          join_empty, join_single(v0);

    // mixed ==-equality is a strange thing. Most times is does not
    // make sense. It is better to allow only for same type == equality.
    BOOST_CHECK_EQUAL( split_empty == split_empty, true );
    BOOST_CHECK_EQUAL( sep_empty   == sep_empty,   true );
    BOOST_CHECK_EQUAL( join_empty  == join_empty,  true );

    // There were Problems with operator== and emtpy sets.
    BOOST_CHECK_EQUAL( split_empty == split_single, false );
    BOOST_CHECK_EQUAL( sep_empty   == sep_single,   false );
    BOOST_CHECK_EQUAL( join_empty  == join_single,  false );

    BOOST_CHECK_EQUAL( split_single == split_empty, false );
    BOOST_CHECK_EQUAL( sep_single   == sep_empty,   false );
    BOOST_CHECK_EQUAL( join_single  == join_empty,  false );

    BOOST_CHECK_EQUAL( is_element_equal(split_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, sep_empty),   true );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, join_empty),  true );

    BOOST_CHECK_EQUAL( is_element_equal(sep_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_empty, sep_empty),   true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_empty, join_empty),  true );

    BOOST_CHECK_EQUAL( is_element_equal(join_empty, split_empty), true );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, sep_empty),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, join_empty),  true );

    //--------------------------------------------------------------------------
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, sep_single),   false );
    BOOST_CHECK_EQUAL( is_element_equal(split_empty, join_single),  false );

    BOOST_CHECK_EQUAL( is_element_equal(sep_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(sep_empty, sep_single),   false );
    BOOST_CHECK_EQUAL( is_element_equal(sep_empty, join_single),  false );

    BOOST_CHECK_EQUAL( is_element_equal(join_empty, split_single), false );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, sep_single),   false );
    BOOST_CHECK_EQUAL( is_element_equal(join_empty, join_single),  false );

    //--------------------------------------------------------------------------
    BOOST_CHECK_EQUAL( is_element_equal(split_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(split_single, sep_empty),   false );
    BOOST_CHECK_EQUAL( is_element_equal(split_single, join_empty),  false );

    BOOST_CHECK_EQUAL( is_element_equal(sep_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(sep_single, sep_empty),   false );
    BOOST_CHECK_EQUAL( is_element_equal(sep_single, join_empty),  false );

    BOOST_CHECK_EQUAL( is_element_equal(join_single, split_empty), false );
    BOOST_CHECK_EQUAL( is_element_equal(join_single, sep_empty),   false );
    BOOST_CHECK_EQUAL( is_element_equal(join_single, join_empty),  false );

}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_assign_4_ordered_types, T, ordered_types)
{         
    T v0 = neutron<T>::value();
    T v1 = unon<T>::value();

    split_interval_set<T>    split_set;
    separate_interval_set<T> sep_set;
    interval_set<T>          join_set;
    split_set.add(v0);
    sep_set = split_set;
    join_set = sep_set;

    BOOST_CHECK_EQUAL( split_set.lower(), sep_set.lower() );
    BOOST_CHECK_EQUAL( split_set.lower(), join_set.lower() );

    split_interval_set<T>    split_self = split_interval_set<T>().add(v0);
    separate_interval_set<T> sep_self   = separate_interval_set<T>().add(v0).add(v1);
    interval_set<T>          join_self  = interval_set<T>().add(v1);

    split_self = split_self;
    sep_self = sep_self;
    join_self = join_self;

    BOOST_CHECK_EQUAL( split_self, split_self );
    BOOST_CHECK_EQUAL( sep_self, sep_self );
    BOOST_CHECK_EQUAL( join_self, join_self );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_ctor_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = rightopen_interval(v1,v3);
    interval<T> I2_4D = rightopen_interval(v2,v4);
    interval<T> I4_5D = rightopen_interval(v4,v5);

    split_interval_set<T> split_set;
    split_set.add(I1_3D).add(I2_4D).add(I4_5D);
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 4 );
    separate_interval_set<T> sep_set(split_set);
    BOOST_CHECK_EQUAL( sep_set.iterative_size(), 4 );
    interval_set<T> join_set(split_set);
    BOOST_CHECK_EQUAL( join_set.iterative_size(), 1 );

    separate_interval_set<T> sep_set2;
    sep_set2.add(I1_3D).add(I2_4D).add(I4_5D);
    BOOST_CHECK_EQUAL( sep_set2.iterative_size(), 2 );
    split_interval_set<T> split_set2(sep_set2);
    BOOST_CHECK_EQUAL( split_set2.iterative_size(), 2 );
    interval_set<T> join_set2(sep_set2);
    BOOST_CHECK_EQUAL( join_set2.iterative_size(), 1 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_assign_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = rightopen_interval(v1,v3);
    interval<T> I2_4D = rightopen_interval(v2,v4);
    interval<T> I4_5D = rightopen_interval(v4,v5);

    split_interval_set<T> split_set;
    split_set.add(I1_3D).add(I2_4D).add(I4_5D);
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 4 );
    separate_interval_set<T> sep_set;
    sep_set = split_set;
    BOOST_CHECK_EQUAL( sep_set.iterative_size(), 4 );
    interval_set<T> join_set;
    join_set = split_set;
    BOOST_CHECK_EQUAL( join_set.iterative_size(), 1 );

    separate_interval_set<T> sep_set2;
    sep_set2.add(I1_3D).add(I2_4D).add(I4_5D);
    BOOST_CHECK_EQUAL( sep_set2.iterative_size(), 2 );
    split_interval_set<T> split_set2;
    split_set2 = sep_set2;
    BOOST_CHECK_EQUAL( split_set2.iterative_size(), 2 );
    interval_set<T> join_set2;
    join_set2 = sep_set2;
    BOOST_CHECK_EQUAL( join_set2.iterative_size(), 1 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_equal_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = rightopen_interval(v1,v3);
    interval<T> I2_4D = rightopen_interval(v2,v4);
    interval<T> I4_5D = rightopen_interval(v4,v5);

    interval_set<T> join_set;
    join_set.add(I1_3D).add(I2_4D).add(I4_5D);
    interval_set<T> join_set2 = join_set;    
    BOOST_CHECK_EQUAL( join_set, join_set2 );
    BOOST_CHECK_EQUAL( is_element_equal(join_set, join_set2), true );

    separate_interval_set<T> sep_set;
    sep_set.add(I1_3D).add(I2_4D).add(I4_5D);

    separate_interval_set<T> sep_set2 = sep_set;    
    BOOST_CHECK_EQUAL( sep_set, sep_set2 );
    BOOST_CHECK_EQUAL( is_element_equal(sep_set2, sep_set), true );

    split_interval_set<T> split_set;    
    split_set.add(I1_3D).add(I2_4D).add(I4_5D);
    split_interval_set<T> split_set2 = split_set;    
    BOOST_CHECK_EQUAL( split_set, split_set2 );
    BOOST_CHECK_EQUAL( is_element_equal(split_set2, split_set), true );

    BOOST_CHECK_EQUAL( is_element_equal(split_set, join_set),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_set, sep_set),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_set,  sep_set),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_set,  split_set), true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_set,   join_set),  true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_set,   split_set), true );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_add_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);

    interval<T> I1_3D = rightopen_interval(v1,v3);
    interval<T> I2_4D = rightopen_interval(v2,v4);
    interval<T> I4_5D = rightopen_interval(v4,v5);

    split_interval_set<T> split_set;
    split_set.add(I1_3D).add(I2_4D);
    split_set += I4_5D;
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 4 );
    separate_interval_set<T> sep_set;
    sep_set += split_set;
    BOOST_CHECK_EQUAL( sep_set.iterative_size(), 4 );
    interval_set<T> join_set;
    join_set += split_set;
    BOOST_CHECK_EQUAL( join_set.iterative_size(), 1 );

    separate_interval_set<T> sep_set2;
    sep_set2.add(I1_3D).add(I2_4D);
    sep_set2 += I4_5D;
    BOOST_CHECK_EQUAL( sep_set2.iterative_size(), 2 );
    split_interval_set<T> split_set2;
    split_set2 += sep_set2;
    BOOST_CHECK_EQUAL( split_set2.iterative_size(), 2 );
    interval_set<T> join_set2;
    join_set2 += sep_set2;
    BOOST_CHECK_EQUAL( join_set2.iterative_size(), 1 );

    interval_set<T> join_set3;
    join_set3.add(v1).add(v3);
    join_set3 += v5;
    BOOST_CHECK_EQUAL( join_set3.iterative_size(), 3 );
    split_interval_set<T> split_set3;
    split_set3 += join_set3;
    BOOST_CHECK_EQUAL( split_set3.iterative_size(), 3 );
    separate_interval_set<T> sep_set3;
    sep_set3 += join_set3;
    BOOST_CHECK_EQUAL( join_set3.iterative_size(), 3 );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_subtract_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = rightopen_interval(v0,v4);
    interval<T> I2_6D = rightopen_interval(v2,v6);
    interval<T> I3_6D = rightopen_interval(v3,v6);
    interval<T> I5_7D = rightopen_interval(v5,v7);
    interval<T> I7_8D = rightopen_interval(v7,v8);
    interval<T> I8_9D = rightopen_interval(v8,v9);
    interval<T> I8_9I =    closed_interval(v8,v9);

    //JODO INSIGHT: With respect to subtraction all interval_sets are working equivalently:
    //Never does a subtract operation introduce or preserve interval borders within
    //the range of the subtracted elements. This is almost trivial.
    //LAW: y -= x == y -= x.join()
    split_interval_set<T> split_set;
    split_set.add(I0_4D).add(I2_6D).add(I5_7D).add(I7_8D).add(I8_9I);
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 7 );

    separate_interval_set<T> sep_set;
    sep_set.add(I0_4D).add(I2_6D).add(I5_7D).add(I7_8D).add(I8_9I);
    BOOST_CHECK_EQUAL( sep_set.iterative_size(), 3 );

    interval_set<T> join_set;
    join_set.add(I0_4D).add(I2_6D).add(I5_7D).add(I7_8D).add(I8_9I);
    BOOST_CHECK_EQUAL( join_set.iterative_size(), 1 );

    // Make sets to be subtracted
    split_interval_set<T> split_sub;
    split_sub.add(I3_6D).add(I8_9D);

    separate_interval_set<T> sep_sub;
    sep_sub.add(I3_6D).add(I8_9D);

    interval_set<T> join_sub;
    join_sub.add(I3_6D).add(I8_9D);

    //--------------------------------------------------------------------------
    // Test for split_interval_set
    split_interval_set<T>    split_diff = split_set;
    separate_interval_set<T> sep_diff   = sep_set;
    interval_set<T>          join_diff  = join_set;

    //subtraction combinations
    split_diff -= split_sub;
    sep_diff   -= split_sub;
    join_diff  -= split_sub;

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( sep_diff.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  3 );

    BOOST_CHECK_EQUAL( is_element_equal(split_diff, split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, sep_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff), true );

    //--------------------------------------------------------------------------
    // Test for separate_interval_set. Reinitialize
    split_diff = split_set;
    sep_diff   = sep_set;
    join_diff  = join_set;

    //subtraction combinations
    split_diff -= sep_sub;
    sep_diff   -= sep_sub;
    join_diff  -= sep_sub;

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( sep_diff.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  3 );

    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   sep_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, sep_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  sep_diff),   true );

    //--------------------------------------------------------------------------
    // Test for interval_set. Reinitialize
    split_diff = split_set;
    sep_diff   = sep_set;
    join_diff  = join_set;

    //subtraction combinations
    split_diff -= join_sub;
    sep_diff   -= join_sub;
    join_diff  -= join_sub;

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( sep_diff.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  3 );

    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  sep_diff),    true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_erase_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_4D = rightopen_interval(v0,v4);
    interval<T> I2_6D = rightopen_interval(v2,v6);
    interval<T> I3_6D = rightopen_interval(v3,v6);
    interval<T> I5_7D = rightopen_interval(v5,v7);
    interval<T> I7_8D = rightopen_interval(v7,v8);
    interval<T> I8_9D = rightopen_interval(v8,v9);
    interval<T> I8_9I =    closed_interval(v8,v9);

    //JODO INSIGHT: With respect to subtraction all interval_sets are working equivalently:
    //Never does a subtract operation introduce or preserve interval borders within
    //the range of the subtracted elements. This is almost trivial.
    //LAW: y -= x == y -= x.join()
    split_interval_set<T> split_set;
    split_set.add(I0_4D).add(I2_6D).add(I5_7D).add(I7_8D).add(I8_9I);
    BOOST_CHECK_EQUAL( split_set.iterative_size(), 7 );

    separate_interval_set<T> sep_set;
    sep_set.add(I0_4D).add(I2_6D).add(I5_7D).add(I7_8D).add(I8_9I);
    BOOST_CHECK_EQUAL( sep_set.iterative_size(), 3 );

    interval_set<T> join_set;
    join_set.add(I0_4D).add(I2_6D).add(I5_7D).add(I7_8D).add(I8_9I);
    BOOST_CHECK_EQUAL( join_set.iterative_size(), 1 );

    // Make sets to be subtracted
    split_interval_set<T> split_sub;
    split_sub.add(I3_6D).add(I8_9D);

    separate_interval_set<T> sep_sub;
    sep_sub.add(I3_6D).add(I8_9D);

    interval_set<T> join_sub;
    join_sub.add(I3_6D).add(I8_9D);

    //--------------------------------------------------------------------------
    // Test for split_interval_set
    split_interval_set<T>    split_diff = split_set;
    separate_interval_set<T> sep_diff   = sep_set;
    interval_set<T>          join_diff  = join_set;

    //subtraction combinations
    erase(split_diff, split_sub);
    erase(sep_diff,   split_sub);
    erase(join_diff,  split_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( sep_diff.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  3 );

    BOOST_CHECK_EQUAL( is_element_equal(split_diff, split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, sep_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff), true );

    //--------------------------------------------------------------------------
    // Test for separate_interval_set. Reinitialize
    split_diff = split_set;
    sep_diff   = sep_set;
    join_diff  = join_set;

    //subtraction combinations
    erase(split_diff, sep_sub);
    erase(sep_diff, sep_sub);
    erase(join_diff, sep_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( sep_diff.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  3 );

    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   sep_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   split_diff), true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   join_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(split_diff, sep_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  sep_diff),   true );

    //--------------------------------------------------------------------------
    // Test for interval_set. Reinitialize
    split_diff = split_set;
    sep_diff   = sep_set;
    join_diff  = join_set;

    //subtraction combinations
    erase(split_diff, join_sub);
    erase(sep_diff, join_sub);
    erase(join_diff, join_sub);

    BOOST_CHECK_EQUAL( split_diff.iterative_size(), 5 );
    BOOST_CHECK_EQUAL( sep_diff.iterative_size(),   4 );
    BOOST_CHECK_EQUAL( join_diff.iterative_size(),  3 );

    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  split_diff),  true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  sep_diff),    true );
    BOOST_CHECK_EQUAL( is_element_equal(sep_diff,   join_diff),   true );
    BOOST_CHECK_EQUAL( is_element_equal(join_diff,  join_diff),   true );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_basic_intersect_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_3D = rightopen_interval(v0,v3);
    interval<T> I1_3D = rightopen_interval(v1,v3);
    interval<T> I1_8D = rightopen_interval(v1,v8);
    interval<T> I2_7D = rightopen_interval(v2,v7);
    interval<T> I2_3D = rightopen_interval(v2,v3);
    interval<T> I6_7D = rightopen_interval(v6,v7);
    interval<T> I6_8D = rightopen_interval(v6,v8);
    interval<T> I6_9D = rightopen_interval(v6,v9);

    //--------------------------------------------------------------------------
    // split_interval_set
    //--------------------------------------------------------------------------
    //split_A      [0       3)       [6    9)
    //         *=      [1                8)
    //split_AB ->      [1   3)       [6  8)
    //         *=        [2             7)     
    //         ->        [2 3)       [6 7)
    split_interval_set<T>    split_A, split_B, split_AB, split_ab, split_ab2;

    split_A.add(I0_3D).add(I6_9D);
    split_AB = split_A;
    split_AB *= I1_8D;
    split_ab.add(I1_3D).add(I6_8D);

    BOOST_CHECK_EQUAL( split_AB, split_ab );

    split_AB = split_A;
    (split_AB *= I1_8D) *= I2_7D;
    split_ab2.add(I2_3D).add(I6_7D);

    BOOST_CHECK_EQUAL( split_AB, split_ab2 );


    //--------------------------------------------------------------------------
    //split_A      [0       3)       [6    9)
    //         *=       1
    //split_AB ->      [1]
    //         +=         (1             7)     
    //         ->      [1](1             7)
    split_A.add(I0_3D).add(I6_9D);
    split_AB = split_A;
    split_AB *= v1;
    split_ab.clear();
    split_ab.add(v1);

    BOOST_CHECK_EQUAL( split_AB, split_ab );

    split_AB = split_A;
    (split_AB *= v1) += open_interval<T>(v1,v7);
    split_ab2.clear();
    split_ab2 += rightopen_interval<T>(v1,v7);

    BOOST_CHECK_EQUAL( is_element_equal(split_AB, split_ab2), true );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_intersect_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_3D = rightopen_interval(v0,v3);
    interval<T> I1_2D = rightopen_interval(v1,v2);
    interval<T> I1_3D = rightopen_interval(v1,v3);
    interval<T> I2_3D = rightopen_interval(v2,v3);
    interval<T> I2_4D = rightopen_interval(v2,v4);
    interval<T> I5_8D = rightopen_interval(v5,v8);
    interval<T> I6_8D = rightopen_interval(v6,v8);
    interval<T> I6_9D = rightopen_interval(v6,v9);

    //--------------------------------------------------------------------------
    // split_interval_set
    //--------------------------------------------------------------------------
    //split_A      [0          3)       [6   9)
    //split_B  *=      [1 2)[2    4) [5    8)
    //split_AB ->      [1 2)[2 3)       [6 8)
    split_interval_set<T>    split_A, split_B, split_AB, split_ab, split_ab_jn;
    separate_interval_set<T> sep_A,   sep_B,   sep_AB,   sep_ab;
    interval_set<T>          join_A,  join_B,  join_AB,  join_ab;

    split_A.add(I0_3D).add(I6_9D);
    split_B.add(I1_2D).add(I2_4D).add(I5_8D);
    split_ab.add(I1_2D).add(I2_3D).add(I6_8D);
    split_ab_jn.add(I1_3D).add(I6_8D);
    split_AB = split_A;
    split_AB *= split_B;
    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 3 );
    BOOST_CHECK_EQUAL( split_AB, split_ab );

    //split_A      [0          3)       [6   9)
    //sep_B    *=      [1 2)[2    4) [5    8)
    //split_AB ->      [1 2)[2 3)       [6 8)
    split_AB = split_A;
    sep_B = split_B;
    split_AB *= sep_B;
    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 3 );
    BOOST_CHECK_EQUAL( split_AB, split_ab );
    
    //split_A      [0          3)       [6   9)
    //join_B   *=      [1         4) [5    8)
    //split_AB ->      [1      3)       [6 8)
    split_AB = split_A;
    join_B = split_B;
    split_AB *= join_B;

    BOOST_CHECK_EQUAL( split_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( split_AB, split_ab_jn );
    
    //--------------------------------------------------------------------------
    // separate_interval_set
    //--------------------------------------------------------------------------
    //sep_A      [0          3)       [6   9)
    //sep_B  *=      [1 2)[2    4) [5    8)
    //sep_AB ->      [1 2)[2 3)       [6 8)
    sep_ab = split_ab;
    BOOST_CHECK_EQUAL( sep_ab.iterative_size(), 3 );

    sep_AB = split_A;
    sep_B  = split_B;
    sep_AB *= sep_B;

    BOOST_CHECK_EQUAL( sep_AB.iterative_size(), 3 );
    BOOST_CHECK_EQUAL( sep_AB, sep_ab );
    
    //sep_A       [0          3)       [6   9)
    //split_B *=      [1 2)[2    4) [5    8)
    //sep_AB  ->      [1 2)[2 3)       [6 8)
    sep_AB = split_A;
    sep_AB *= split_B;

    BOOST_CHECK_EQUAL( sep_AB.iterative_size(), 3 );
    BOOST_CHECK_EQUAL( sep_AB, sep_ab );
    
    //sep_A       [0         3)        [6   9)
    //join_B *=      [1          4) [5    8)
    //sep_AB ->      [1      3)        [6 8)
    separate_interval_set<T> sep_ab_jn = split_ab_jn;
    sep_AB = split_A;
    join_B = split_B;
    sep_AB *= join_B;

    BOOST_CHECK_EQUAL( sep_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( sep_AB, sep_ab_jn );

    //--------------------------------------------------------------------------
    // separate_interval_set
    //--------------------------------------------------------------------------
    //join_A      [0          3)       [6   9)
    //join_B  *=      [1         4) [5    8)
    //join_AB ->      [1      3)       [6 8)
    join_ab = split_ab;
    BOOST_CHECK_EQUAL( join_ab.iterative_size(), 2 );

    join_AB = split_A;
    join_B  = split_B;
    join_AB *= sep_B;

    BOOST_CHECK_EQUAL( join_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( join_AB, join_ab );
    
    //join_A      [0          3)       [6   9)
    //split_B  *=     [1 2)[2    4) [5    8)
    //join_AB  ->     [1      3)       [6 8)
    join_AB = split_A;
    join_AB *= split_B;

    BOOST_CHECK_EQUAL( join_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( join_AB, join_ab );
    
    //join_A      [0          3)       [6   9)
    //sep_B    *=     [1 2)[2    4) [5    8)
    //join_AB  ->     [1      3)       [6 8)
    join_AB = split_A;
    join_AB *= sep_B;

    BOOST_CHECK_EQUAL( join_AB.iterative_size(), 2 );
    BOOST_CHECK_EQUAL( join_AB, join_ab );
    
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_set_mixed_disjoint_4_bicremental_types, T, bicremental_types)
{         
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    interval<T> I0_2D = rightopen_interval(v0,v2);
    interval<T> I2_3D = rightopen_interval(v2,v3);
    interval<T> I3_4D = rightopen_interval(v3,v4);
    interval<T> I4_4I = closed_interval(v4,v4);
    interval<T> C4_6D = open_interval(v4,v6);
    interval<T> I6_6I = closed_interval(v6,v6);

    //--------------------------------------------------------------------------
    //split_A: [0  2)          [4 4]      [6 6]
    //split_B:       [2 3)[3 4)     (4  6)
    split_interval_set<T> split_A, split_B;

    split_A.add(I0_2D).add(I4_4I).add(I6_6I);
    split_B.add(I2_3D).add(I3_4D).add(C4_6D);

    separate_interval_set<T> sep_A(split_A), sep_B(split_B);
    interval_set<T> join_A(split_A), join_B(split_B);

    BOOST_CHECK_EQUAL( is_disjoint(split_A, split_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(split_A, sep_B),   true );
    BOOST_CHECK_EQUAL( is_disjoint(split_A, join_B),  true );

    BOOST_CHECK_EQUAL( is_disjoint(sep_A,   split_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(sep_A,   sep_B),   true );
    BOOST_CHECK_EQUAL( is_disjoint(sep_A,   join_B),  true );

    BOOST_CHECK_EQUAL( is_disjoint(join_A,  split_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(join_A,  sep_B),   true );
    BOOST_CHECK_EQUAL( is_disjoint(join_A,  join_B),  true );
}

