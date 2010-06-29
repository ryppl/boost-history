//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>


using namespace boost;

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


int main() {
    {
        using namespace ::boost::details;
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


