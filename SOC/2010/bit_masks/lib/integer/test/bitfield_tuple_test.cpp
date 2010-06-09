//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include "test_type_list.hpp"
#include <boost/mpl/front.hpp>
#include <boost/mpl/find_if.hpp>

void ignore(...) {}

struct red { };
struct green { };
struct blue { };

template <typename T, typename U>
struct match_name
    :is_same<typename T::name_type, U>::type
{ };

int main() {
    // lets make some errors : )
    // bitfield_tuple < storage<int>, member<int,red,6> > temp;
    // test suite for the storage
    {
        typedef storage<int, std::allocator<int> > test1;
        test1 s;
        ignore( s );

        // make sure that the storage policy is the correct policy.
        BOOST_ASSERT((
            is_same<
                test1::alloc,
                std::allocator<int>
            >::value
        ));

        BOOST_ASSERT((
            is_same<
                test1::storage_type,
                int
            >::value
        ));
        typedef storage<int, std::allocator<char> > test2;
        BOOST_ASSERT((
            is_same<
                test2::alloc,
                std::allocator<int>
            >::value
        ));

        BOOST_ASSERT((
            is_same<
                int,
                test2::storage_type
            >::value
        ));

        typedef storage<char> test3;
        BOOST_ASSERT((
            is_same<
                test3::alloc,
                storage_policy_stack
            >::value
        ));

        BOOST_ASSERT((
            is_same<
                test3::storage_type,
                char
            >::value
        ));
    }
    // testing member type
    {
        typedef member<int, blue, 6 > member_test_1;
        BOOST_ASSERT(( is_same<member_test_1::return_type, int>::value ));
        BOOST_ASSERT(( is_same<member_test_1::name_type, blue>::value ));
        BOOST_ASSERT(( member_test_1::field_width == 6 ));
    }
    // testing bitfield_element_
    {
        // bitfield_element_
        typedef details::bitfield_element_<int, red, mpl::size_t<5>, mpl::size_t<4> > bft_element_test_1;
        BOOST_ASSERT(( is_same<bft_element_test_1::return_type, int>::value ));
        BOOST_ASSERT(( is_same<bft_element_test_1::name_type, red>::value ));
        BOOST_ASSERT(( bft_element_test_1::offset::value == 5 ));
        BOOST_ASSERT(( bft_element_test_1::field_width::value == 4 ));

    }

    // testing for parsing of aruments passed into template parameters.    
    {
    
    typedef bitfield_tuple< storage<int>, member<int,red,3> > bft;
    typedef bft::processed_args pa;

    // checking for storage type.
    BOOST_MPL_ASSERT((
        is_same<
            pa::storage_policy,
            storage<int>
        >
    ));

    BOOST_MPL_ASSERT((
        is_same<
            mpl::front< pa::field_vector >::type,
            details::bitfield_element_<
                int,
                red,
                mpl::size_t<0>,
                mpl::size_t<3> 
            > 
        >
    ));

    typedef mpl::vector<
        details::bitfield_element_<
            int,
            red,
            mpl::size_t<0>,
            mpl::size_t<3>
        > 
    > temp_vect;

    // tesitng so I can learn to use mpl::find_if
    typedef mpl::find_if<temp_vect, match_name<mpl::_1, red> >::type temp_located;
    
    

    }
    return 0;
}
