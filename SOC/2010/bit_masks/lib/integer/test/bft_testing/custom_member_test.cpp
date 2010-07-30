//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace boost;
using namespace boost::bitfields;

struct cust;
struct i1;
struct i2;

struct b1;
struct b2;


// Most of the testing of this is done inside of pointer test because
// the are both based on the exact same code.
int main() {
    {
        typedef bitfield_tuple<
            custom<
                unsigned int,
                cust,
                bits_mask<unsigned int, 8,16>
            >
        >                       custom_t1;

        custom_t1 t1;
        unsigned int i = 0x00ffff00;
        t1.get<cust>() = i;
        BOOST_TEST(t1.get<cust>() == i);
    }
    {
        typedef bitfield_tuple<
            custom<
                unsigned int,
                cust,
                bits_mask<unsigned int, 8,16>
            >
        >                       custom_t1;
    }
    return 0;
}

