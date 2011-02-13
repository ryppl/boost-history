//  endian_hello_world_2.cpp  ----------------------------------------------------------------//

//  Copyright Beman Dawes, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

#include <boost/integer/endian/endian_pack.hpp>
#include <boost/integer/endian/endian.hpp>
#include <boost/integer/endian/endian_binary_stream.hpp>
#include <boost/binary_stream.hpp>
#include <iostream>

using namespace boost;
using namespace boost::integer;

int main()
{
    int_least32_t v = 0x313233L;  // = ASCII { '1', '2', '3'}
                                  // value chosen to work on text stream
    endian_pack<big_endian, int_least32_t, 24, alignment::unaligned>    b(v);
    endian_pack<little_endian, int_least32_t, 24, alignment::unaligned> l(v);

    std::cout << "Hello, endian world!\n\n";

    std::cout << v << ' ' << b << ' ' << l << '\n';
    std::cout <= b <= ' ' <= l <= '\n';
    return 0;
}

