//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>


using namespace boost;
using namespace boost::bitfields;
using namespace boost::bitfields::detail::pointer_member;

typedef bits_mask< int, 2, bit_width<int>::value - 4> mask_1;
typedef bits_mask< int, 4, bit_width<int>::value - 4> mask_2;
typedef bits_mask< int, 0, bit_width<int>::value - 4> mask_3;
typedef integral_mask<int,0>                          mask_4;
int main() {
    // count leading zeros' test
    {
        // count_leading_zeros
        BOOST_TEST( count_leading_zeros<mask_1>::type::value == 2);
        BOOST_TEST( count_leading_zeros<mask_2>::type::value == 0);
        BOOST_TEST( count_leading_zeros<mask_3>::type::value == 4);
        BOOST_TEST( count_leading_zeros<mask_4>::type::value == 32);
    }

    // count trailing zeros' test
    {
        // count_trailing_zeros
        BOOST_TEST( count_trailing_zeros<mask_1>::type::value == 2);
        BOOST_TEST( count_trailing_zeros<mask_2>::type::value == 4);
        BOOST_TEST( count_trailing_zeros<mask_3>::type::value == 0);
        BOOST_TEST( count_trailing_zeros<mask_4>::type::value == 32);


    }
    return boost::report_errors();
}

