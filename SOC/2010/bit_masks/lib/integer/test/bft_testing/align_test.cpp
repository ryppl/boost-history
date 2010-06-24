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




int main() {

    // test case where alignment set the next bit to 32.
    {
        typedef bitfield_tuple<
            member<char,red,4>,
            member<unsigned char, green,5>,
            storage<std::size_t>,
            filler<3>,
            member<int, salmon, 16>,
            flag<blue>,
            bit_align<32>
        >                                       test_tuple_1;
        test_tuple_1 temp;
        BOOST_ASSERT(( test_tuple_1::bits_used::value == 32 ));
    }

    // test case where alignment is 32 and bit align attempts to set it to 32
    {
        typedef bitfield_tuple<
            storage<std::size_t>,
            bit_align<32>
        >                                       test_tuple_2;
        test_tuple_2 temp;
        BOOST_ASSERT(( test_tuple_2::bits_used::value == 0 ));
    }

    // test case where user has a field that extends past the first 8 bits
    // and they would like to align to the next 8 bit boundry.
    {
        typedef bitfield_tuple<
            storage<std::size_t>,
            member<int,red,9>,
            bit_align<8>
        >                                       test_tuple_3;
        test_tuple_3 temp;
        BOOST_ASSERT(( test_tuple_3::bits_used::value == 16));
    }

    // test case calling align 2x times in a row doesn't do anything.
    {
        typedef bitfield_tuple<
            bit_align<32>,
            bit_align<32>,
            storage<unsigned int>
        >                                       test_tuple_4;
        test_tuple_4 temp;
        BOOST_ASSERT(( test_tuple_4::bits_used::value == 0 ));
    }
    return 0;
}
