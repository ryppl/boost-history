//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
using namespace boost;
struct red { };



typedef bitfield_tuple<
    storage<int>,
    member<int, red, 15u>,
    member<int, red, 17u>
>       bft;




int main() {

    int data_storage(0);
    typedef details::bitfield_element_<
        char,
        red,
        mpl::size_t<9>,
        mpl::size_t<8>
    >                                               element;

    typedef bft::bit_ref<element> ref_type_1;

    ref_type_1 test_1(data_storage);

    // char temp = test_1;
    char value_to_assign = 'a';
    test_1 = value_to_assign;
    BOOST_ASSERT(( data_storage >> 9 == 'a' ));

    return 0;
}
