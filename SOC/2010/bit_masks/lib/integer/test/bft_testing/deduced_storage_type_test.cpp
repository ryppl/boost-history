//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>


using namespace boost;

struct red;
struct green;
struct pink;
struct blue;
struct salmon;


typedef bitfield_tuple<
    flag<green>,
    bit_align<16>
>       test_16;

typedef bitfield_tuple<
    flag<pink>,
    bit_align<8>
>       test_8;

typedef bitfield_tuple<
    flag<red>,
    bit_align<32>
>       test_32;



int main() {
    BOOST_MPL_ASSERT(( is_same<test_8::storage_type,  uint_least8_t> ));
    BOOST_MPL_ASSERT(( is_same<test_16::storage_type, uint_least16_t> ));
    BOOST_MPL_ASSERT(( is_same<test_32::storage_type, uint_least32_t> ));
    return 0;
}
