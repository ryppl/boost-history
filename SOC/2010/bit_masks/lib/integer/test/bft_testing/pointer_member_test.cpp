//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <iostream>
#include <iomanip>

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

typedef bitfield_tuple<
    storage<unsigned long long>,
    filler<2>,
    pointer<int, rd,
        bits_mask<
            detail::ptr_mask_type,
            2,
            bit_width<
                detail::ptr_mask_type
            >::value - 4
        >
    >,
    flag<b1>,
    flag<b2>
>                   test_type_6;

typedef bitfield_tuple<
    storage<unsigned long long>,
    filler<3>,
    pointer<int, rd,
        bits_mask<
            detail::ptr_mask_type,
            2,
            bit_width<
                detail::ptr_mask_type
            >::value - 4
        >
    >,
    flag<b1>,
    flag<b2>
>                   test_type_7;
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
    {
        
        test_type_3 t3;
        int i = 30;
        t3.get<rd>() = &i;
        BOOST_TEST(*t3.get<rd>() == 30 );
    }

    // this test will also fail on 64 bit machines.
    {
        test_type_4 t4;
        int i = 70;
        t4.get<rd>() = &i;
        BOOST_TEST( *t4.get<rd>() == 70 );
    }

    // this test will also fail on 64 bit machines.
    {
        test_type_5 t5;
        int i = 70;
        t5.get<rd>() = &i;
        BOOST_TEST( *t5.get<rd>() == 70 );
    }
#endif
    {
        test_type_6 t6;
        int i = 70;
        
        detail::ptr_mask_type ptr = high_bits_mask<
            detail::ptr_mask_type,
            2
        >::value
          &
        detail::ptr_mask_type(&i);

        std::cout<< "value of &i: " << std::hex << &i << std::endl;
        std::cout<< "2 bits of &i: " << std::hex << ptr << std::endl;

        t6.get<rd>() = &i;

        int* temp_ptr = t6.get<rd>();
        std::cout<< "Value of temp_ptr: " << std::hex << temp_ptr << std::endl;
        detail::ptr_mask_type temp_int = detail::ptr_mask_type(temp_ptr); 
        temp_int |= detail::ptr_mask_type( ptr );
        typedef int* intptr_t;
        temp_ptr = intptr_t(temp_int);

        std::cout<< "Value of temp_int: " << std::hex<< temp_int << std::endl;
        std::cout<< "Value returned from t6.get<rd>(): " << std::hex <<
            temp_ptr << std::endl;

        BOOST_TEST( * temp_ptr == 70 );
    }
    {
        test_type_7 t7;
        int i = 70;
        
        detail::ptr_mask_type ptr = high_bits_mask<
            detail::ptr_mask_type,
            2
        >::value
          &
        detail::ptr_mask_type(&i);

        std::cout<< "value of &i: " << std::hex << &i << std::endl;
        std::cout<< "2 bits of &i: " << std::hex << ptr << std::endl;

        t7.get<rd>() = &i;

        int* temp_ptr = t7.get<rd>();
        std::cout<< "Value of temp_ptr: " << std::hex << temp_ptr << std::endl;
        detail::ptr_mask_type temp_int = detail::ptr_mask_type(temp_ptr); 
        temp_int |= detail::ptr_mask_type( ptr );
        typedef int* intptr_t;
        temp_ptr = intptr_t(temp_int);

        std::cout<< "Value of temp_int: " << std::hex<< temp_int << std::endl;
        std::cout<< "Value returned from t6.get<rd>(): " << std::hex <<
            temp_ptr << std::endl;

        BOOST_TEST( *temp_ptr == 70 );
    }

    return boost::report_errors();
}
