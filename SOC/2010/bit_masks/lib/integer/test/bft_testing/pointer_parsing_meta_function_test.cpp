//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iomanip>
#include <iostream>
using namespace ::boost::bitfields::detail::pointer_member;

int main() {
    // count leading zeros metafunction test.
    {
        // Testing to make sure that if there is a bit at the first position
        // that 0 is correctly returned.
        typedef count_leading_zeros< boost::bits_mask<
            unsigned int, 31> >::type test_t1;
        BOOST_TEST((test_t1::value == 0 ));

        // testing to make sure that the test passes if the last bit is 1.
        typedef count_leading_zeros<boost::bits_mask<
            unsigned int,0> >::type test_t2;
        BOOST_TEST( test_t2::value == 31 );
    }

    // count trailing zeros test.
    {
        typedef count_trailing_zeros<boost::bits_mask<
            unsigned int,31> >::type test_1;
        BOOST_TEST( test_1::value == 31 );

        typedef count_trailing_zeros<boost::bits_mask<
            unsigned int,0> >::type test_2;
        BOOST_TEST( test_2::value == 0 );
    }
    return boost::report_errors();
}
