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
    filler<3>,
    member<int, salmon, 16>,
    flag<blue>
>                                       test_tuple;


int main() {
    
    int temp = mpl::at_c<test_tuple::members, 2>::type::offset::value - (mpl::at_c<test_tuple::members, 1>::type::offset::value +
        mpl::at_c<test_tuple::members, 1>::type::field_width::value);
    
    BOOST_ASSERT(( temp == 3 ));
    return 0;
}


