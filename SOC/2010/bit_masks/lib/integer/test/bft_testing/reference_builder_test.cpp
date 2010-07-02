//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>


using namespace boost;

struct red;
struct green;
struct salmon;
struct blue;

typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    filler<3>,
    member<int, salmon, 16>,
    flag<blue>,
    bit_align<32>
>                  test_tuple_1;

int main() {
    {
        typedef details::disable_if_reference_type_by_name<
            const test_tuple_1,
            salmon
        >::type                 ref_t_1;
        std::size_t  store_house = 0;

        typedef details::disable_if_reference_type_by_name<
            test_tuple_1,
            salmon
        >::type                 ref_t_2;
        ref_t_2 non_const_ref(store_house);
        non_const_ref = 3;
        ref_t_1 const_ref(store_house);
        BOOST_ASSERT(( const_ref == 3));
    }
    


    return 0;
}