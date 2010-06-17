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
    member<int, salmon, 16>
>                                       test_tuple;


int main() {
    // testing look up interface
    {
        test_tuple test_1;
        test_1.get<green>()     = 15;
        test_1.get<red>()       = 3;
        test_1.get<salmon>()    = 6;
        BOOST_ASSERT((test_1.get<green>() == 15));
        BOOST_ASSERT((test_1.get<red>() == 3));
        BOOST_ASSERT(( test_1.get<salmon>() == 6));

        BOOST_ASSERT((
            const_cast<
                test_tuple const&
            >(test_1).get<salmon>() == 6
        ));
    }

    // get using an index.
    {
        test_tuple test_2;
        test_2.get<0>() = 3;
        test_2.get<1>() = 14;
        test_2.get<2>() = 6;
        BOOST_ASSERT((test_2.get<0>() == 3));
        BOOST_ASSERT((test_2.get<1>() == 14));
        BOOST_ASSERT((test_2.get<2>() == 6));

        BOOST_ASSERT((
            const_cast<
                test_tuple const&
            >(test_2).get<2>() == 6
        ));
    }

    // negative value testing
    {
        test_tuple test_3;
        test_3.get<0>() = -1;
        BOOST_ASSERT(( test_3.get<0>() == -1 ));
    }
    return 0;
}


