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


typedef details::bitfield_element_< char,
red,mpl::size_t<9>,mpl::size_t<8> >             element_1;

typedef details::bitfield_element_< char, 
red, mpl::size_t<0>, mpl::size_t<8> >           element_2;

typedef details::bitfield_element_< char, 
red, mpl::size_t<8>, mpl::size_t<8> >           element_3;

typedef details::bitfield_element_< char, 
red, mpl::size_t<16>, mpl::size_t<8> >          element_4;

typedef details::bitfield_element_< char, 
red, mpl::size_t<24>, mpl::size_t<8> >          element_5;
int main() {

    int data_storage(0);

    // simple test entery and retreval.
    {
    typedef bft::bit_ref<element_1> ref_type_1;

    ref_type_1 test_1( data_storage );

    char temp = 'a';
    test_1 = temp;
    BOOST_ASSERT(( test_1 == temp ));
    }
    
    data_storage = 0;

    // slightly more complex.
    // multiple fields which arn't next to one another.
    {
        typedef bft::bit_ref<element_2> ref_type_1;
        typedef bft::bit_ref<element_4> ref_type_2;

        // constructing reference types.
        ref_type_1 ref_1(data_storage);
        ref_type_2 ref_2(data_storage);
        ref_1 = 'q';
        ref_2 = 'p';

        BOOST_ASSERT(( ref_1 == 'q' ));
        BOOST_ASSERT(( ref_2 == 'p' ));
    }

    data_storage = 0;

    // case the integer storage type is completely filled with fields.
    {
        // create fake reference type to test..
        typedef bft::bit_ref<element_2> ref_type_1;
        typedef bft::bit_ref<element_3> ref_type_2;
        typedef bft::bit_ref<element_4> ref_type_3;
        typedef bft::bit_ref<element_5> ref_type_4;
        //
        ref_type_1 ref_1(data_storage);
        ref_type_2 ref_2(data_storage);
        ref_type_3 ref_3(data_storage);
        ref_type_4 ref_4(data_storage);

        ref_1 = 'p';
        ref_2 = 'q';
        ref_3 = 'r';
        ref_4 = 's';

        BOOST_ASSERT(( ref_1 == 'p' )); 
        BOOST_ASSERT(( ref_2 == 'q' )); 
        BOOST_ASSERT(( ref_3 == 'r' )); 
        BOOST_ASSERT(( ref_4 == 's' )); 
    }
    return 0;
}
