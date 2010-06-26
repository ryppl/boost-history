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
    flag<pink>,
    bit_align<64>
>       test_tuple;


int main() {

    BOOST_ASSERT(( is_same<test_tuple::storage_type,uint_least64_t>::value ));
    return 0;
}
