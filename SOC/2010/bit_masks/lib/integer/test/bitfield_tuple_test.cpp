//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include "test_type_list.hpp"
#include <boost/mpl/front.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>

void ignore(...) {}

struct red { };
struct green { };
struct blue { };

template <typename T>
void test_get_data(T const& x, int value) {
    BOOST_ASSERT(( x.get_data() == value ));
}

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
        typedef details::bitfield_element_<
            int, red, mpl::size_t<5>, mpl::size_t<4>
        > bft_element_test_1;
        BOOST_ASSERT(( is_same<bft_element_test_1::return_type, int>::value ));
        BOOST_ASSERT(( is_same<bft_element_test_1::name_type, red>::value ));
        BOOST_ASSERT(( bft_element_test_1::offset::value == 5 ));
        BOOST_ASSERT(( bft_element_test_1::field_width::value == 4 ));

    }

    // stack allocation policy for bitfield_tuple
    {
        // default constructor.
        details::stack_alloc_base_policy<int> sap_test_1;
        BOOST_ASSERT(( sap_test_1._data == 0));

        // value constructor
        details::stack_alloc_base_policy<int> sap_test_2( 2 );
        BOOST_ASSERT(( sap_test_2._data == 2));

        // copy constructor
        details::stack_alloc_base_policy<int> sap_test_3( sap_test_2 );
        BOOST_ASSERT(( sap_test_3._data == 2));

        // assignement.
        details::stack_alloc_base_policy<int> sap_test_4;
        sap_test_4 = sap_test_3;
        BOOST_ASSERT(( sap_test_4._data == 2));
        sap_test_4 = sap_test_1;
        BOOST_ASSERT(( sap_test_4._data == 0));

        // const test for get data
        test_get_data( sap_test_4, 0 );

        // non const test for get data
        BOOST_ASSERT(( sap_test_4.get_data() == 0));

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
    // for looking up while useing the get<> function in side bitfield_tuple.
    typedef mpl::find_if<temp_vect, match_name<mpl::_1, red> >::type temp_located;
    BOOST_MPL_ASSERT_NOT((
        is_same<
            temp_located,
            mpl::end<temp_vect>::type
        >
    ));


    }

    // bitfield_tuple runtime tests.
    {
        // default constructor.
        typedef bitfield_tuple< storage<int>, member<int,red,3> > bft1;
        bft1 bft_test_1;
        BOOST_ASSERT(( bft_test_1.data() == 0 ));

        // value constructor
        bft1 bft_test_2(5);
        BOOST_ASSERT(( bft_test_2.data() == 5 ));

    }
    return 0;
}
