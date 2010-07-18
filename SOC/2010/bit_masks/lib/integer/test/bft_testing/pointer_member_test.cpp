//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>
using namespace boost;
using namespace boost::bitfields;

struct rd;
struct b1;
struct b2;

typedef bitfield_tuple< pointer<int, rd> > test_type_1;
typedef bitfield_tuple< pointer<int, rd>, flag<b1>, flag<b2> > test_type_2;
// typedef bitfield_tuple< member<int*,rd, bit_width<int*>::value - 2> > test_type_2;

int main() {
    {
    test_type_1 t1;
    int i=0;

    t1.get<rd>() = &i;
    BOOST_TEST(*t1.get<rd>() == 0);
    i = 5;
    BOOST_TEST(*t1.get<rd>() == 5);
    }

    // double boolean test.
    {
    test_type_2 t2;
    int i=0;
    t2.get<rd>() = &i;
    BOOST_TEST(*t2.get<rd>() == 0);
    i = 5;
    BOOST_TEST(*t2.get<rd>() == 5);
    BOOST_TEST(t2.get<b1>() == false );
    BOOST_TEST(t2.get<b2>() == false );
    t2.get<b1>() = true;
    t2.get<b2>() = true;

    BOOST_TEST(*t2.get<rd>() == 5);
    BOOST_TEST(t2.get<b1>() );
    BOOST_TEST(t2.get<b2>() );
    
    }
    return boost::report_errors();
}
