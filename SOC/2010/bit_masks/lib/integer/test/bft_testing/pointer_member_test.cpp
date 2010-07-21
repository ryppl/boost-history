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
// this is first half long long
typedef bitfield_tuple<
    storage<long long>,
    pointer<int, rd>,
    flag<b1>,
    flag<b2>
>                   test_type_3;

#if( _LP64 != 1)
// This is second half long long
// note this only works on 32 bit systems Need to remove this from the test
// in the event that its being test on a 64 bit architecture
typedef bitfield_tuple<
    storage<long long>,
    member<int, short, 32>,
    pointer<int, rd>,
    flag<b1>,
    flag<b2>
>                   test_type_4;

typedef bitfield_tuple<
    storage<unsigned long long>,
    member<int, short, 32>,
    pointer<int, rd>,
    flag<b1>,
    flag<b2>
>                   test_type_5;
#endif
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
    // NOTE this test may fail on 64 bit machines but I need to test it
    // either way.
#if( _LP64 != 1)
    if(sizeof(void*) == 4) {
        
        test_type_3 t3;
        int i = 30;
        t3.get<rd>() = &i;
        BOOST_TEST(*t3.get<rd>() == 30 );
    }

    // this test will also fail on 64 bit machines.
    if(sizeof(void*) == 4) {
        test_type_4 t4;
        int i = 70;
        t4.get<rd>() = &i;
        BOOST_TEST( *t4.get<rd>() == 70 );
    }

    // this test will also fail on 64 bit machines.
    if(sizeof(void*) == 4) {
        test_type_5 t5;
        int i = 70;
        t5.get<rd>() = &i;
        BOOST_TEST( *t5.get<rd>() == 70 );
    }
#endif
    return boost::report_errors();
}
