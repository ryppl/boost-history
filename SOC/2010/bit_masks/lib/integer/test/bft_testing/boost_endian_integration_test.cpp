//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/endian.hpp>
#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>




struct red;
struct green;
struct blue;
using namespace boost;
using namespace boost::integer;
using namespace boost::bitfields;




typedef bitfield_tuple<
    storage< big32_t >,
    member<unsigned char, red, 5>,
    member<unsigned char, green, 6>,
    member<unsigned char, blue, 5>
>                               rgb565_t;

int main() {
    {
        // integer::bitfield<
    }
    #if 1
    rgb565_t rgb565;
    rgb565.get<red>() = 3;
    BOOST_TEST(rgb565.get<red>() == 3);
    rgb565.get<0>() = 4;
    BOOST_TEST(rgb565.get<0>() == 4);
    #endif
    return boost::report_errors();
}
