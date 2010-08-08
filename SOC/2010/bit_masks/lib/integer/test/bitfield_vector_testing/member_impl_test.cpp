//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost::detail;

int main() {
    // Covering trivail case for next_allocation size
    // width = to width of a char.
    {
        BOOST_TEST(next_allocation_size<8>()(0,0) == 1);
        BOOST_TEST(next_allocation_size<8>()(2,0) == 4);

        BOOST_TEST(next_allocation_size<16>()(0,0) == 2);
        BOOST_TEST(next_allocation_size<16>()(4,0) == 8);
    }
    // more complex case where the width is NOT divisible by number of bits in
    // char.
    {
        // less then 8.
        BOOST_TEST(next_allocation_size<3>()(0,0) == 1);
        BOOST_TEST(next_allocation_size<3>()(1,3) == 1);
        BOOST_TEST(next_allocation_size<3>()(2,6) == 2);

        // greater then 8
        BOOST_TEST(next_allocation_size<9>()(0,0) == 2);
        BOOST_TEST(next_allocation_size<9>()(1,9) == 3);
        BOOST_TEST(next_allocation_size<9>()(2,18) == 5);
    }
    return boost::report_errors();
}
