//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
#include <iostream>

using namespace boost;
using namespace boost::bitfields;

struct red;
struct green;
struct pink;
struct blue;
struct salmon;

typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    filler<3>,
    member<int, salmon, 16>,
    flag<blue>
>                                       test_tuple;
#ifdef BOOST_MSVC


#endif

int main() {
    {
#ifdef BOOST_MSVC
        using namespace ::boost::detail;
        // Checking to make sure that the indexs work correctly.
        BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,0>::is_valid_index ));
        BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,1>::is_valid_index ));
        BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,2>::is_valid_index ));
        BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,3>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,4>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,5>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,6>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,7>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,8>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,9>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,10>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,11>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,12>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,13>::is_valid_index ));
        BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,14>::is_valid_index ));
        // BOOST_ASSERT((false));
        // checking to make sure that the if statement part of it works correctly.


        BOOST_MPL_ASSERT(( is_same<get_create_parameter<test_tuple,0>::type, char> ));
        BOOST_MPL_ASSERT(( is_same<get_create_parameter<test_tuple,1>::type, unsigned char> ));
        BOOST_MPL_ASSERT(( is_same<get_create_parameter<test_tuple,2>::type, int> ));
        BOOST_MPL_ASSERT(( is_same<get_create_parameter<test_tuple,3>::type, bool> ));
        BOOST_MPL_ASSERT(( is_same<get_create_parameter<test_tuple,4>::type, mpl::void_*> ));
#endif
    }
    {
#if 1
        using namespace ::boost::detail;
        BOOST_MPL_ASSERT((
            is_same<
                get_create_parameter<test_tuple,0>::type,
                char
            >
        ));
        BOOST_MPL_ASSERT((
            is_same<
                get_create_parameter<test_tuple,2>::type,
                int
            >
        ));
#endif
    }
    {
       
        test_tuple bft = make_bitfield_tuple<test_tuple>(1,2,4,true);
        BOOST_ASSERT(( bft.get<red>() == 1 ));
        BOOST_ASSERT(( bft.get<green>() == 2 ));
        BOOST_ASSERT(( bft.get<salmon>() == 4 ));
        BOOST_ASSERT(( bft.get<blue>() ));
    }
    return 0;
}


