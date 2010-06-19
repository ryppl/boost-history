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


int main() {
    // lets make some errors : )
    // bitfield_tuple < storage<int>, member<int,red,6> > temp;
    // test suite for the storage
    {
        typedef storage<int> storage_t;
        BOOST_MPL_ASSERT(( is_same<storage_t::storage_type, int> ));
    }

    // testing member type
    {
        typedef member<int, blue, 6 > member_test_1;
        BOOST_ASSERT(( is_same<member_test_1::return_type, int>::value ));
        BOOST_ASSERT(( is_same<member_test_1::name_type, blue>::value ));
        BOOST_ASSERT(( member_test_1::field_width == 6 ));
    }
    // testing bitfield_element
    {
        // bitfield_element
        typedef details::bitfield_element<
            int, red, mpl::size_t<5>, mpl::size_t<4>
        > bft_element_test_1;
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
            details::bitfield_element<
                int,
                red,
                mpl::size_t<0>,
                mpl::size_t<3> 
            > 
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
