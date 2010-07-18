//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <iostream>

using namespace boost;
using namespace boost::bitfields;
struct rd;

typedef bitfield_tuple< pointer<int, rd> > test_type_1;
// typedef bitfield_tuple< member<int*,rd, bit_width<int*>::value - 2> > test_type_2;

int main() {
    {
    test_type_1 t1;
    int i=0;
    // std::cout << sizeof(&i) << std::endl;
    // BOOST_TEST(false);
    t1.get<rd>() = &i;
    BOOST_TEST(*t1.get<rd>() == 0);
    }
    // testing member<int*,rd, width of pointer-2>
    {

    // int i = 0;
    // test_type_2 t2;
    // main issue with this is that the pointer type may not be retrieved
    // correctly using bitfield however this shouldn't be an issue if I provide
    // a specialization for bitfield to operate on pointer types.
    // t2.get<rd>() = &i;
    // BOOST_TEST( t2.get<rd>() == &i );
    }
    return boost::report_errors();
}
