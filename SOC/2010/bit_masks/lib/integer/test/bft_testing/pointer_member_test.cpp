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
struct i1;



typedef int* my_intptr_t;

int main() {
    {
        typedef bitfield_tuple< pointer<int, rd> > test_type_1;
        
        BOOST_TEST((
            is_same<
                test_type_1::storage_type,
                detail::ptr_mask_type
            >::value
        ));

        test_type_1 t1;
        int i=0;

        t1.get<rd>() = &i;

        BOOST_TEST(*t1.get<rd>() == 0);

        i = 5;

        BOOST_TEST(*t1.get<rd>() == 5);

    }

    // double boolean test.
    {
        typedef bitfield_tuple< pointer<int, rd>, flag<b1>, flag<b2> > test_type_2;

        BOOST_TEST((
            is_same<
                test_type_2::storage_type,
                detail::ptr_mask_type
            >::value
        ));

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
        typedef bitfield_tuple<
            storage<long long>,
            pointer<int, rd>,
            flag<b1>,
            flag<b2>
        >                   test_type_3;        
        BOOST_TEST((
            is_same<
                test_type_3::storage_type,
                long long
            >::value
        ));

        test_type_3 t3;
        int i = 30;

        t3.get<b1>() = true;
        t3.get<b2>() = true;
        t3.get<rd>() = &i;

        BOOST_TEST(*t3.get<rd>() == 30 );
        BOOST_TEST( t3.get<b1>() == true);
        BOOST_TEST( t3.get<b2>() == true);
    }

    // this test will also fail on 64 bit machines.
    {
    // This is second half long long
    // note this only works on 32 bit systems Need to remove this from the test
    // in the event that its being test on a 64 bit architecture
    typedef bitfield_tuple<
        storage<long long>,
        member<unsigned int, i1, 32>,
        pointer<int, rd>,
        flag<b1>,
        flag<b2>
    >                   test_type_4;
        BOOST_TEST((
            is_same<
                test_type_4::storage_type,
                long long
            >::value
        ));

        test_type_4 t4;
        int i = 70;

        t4.get<i1>() = 0xffffffff;
        t4.get<b1>() = true;
        t4.get<b2>() = true;
        t4.get<rd>() = &i;

        BOOST_TEST( *t4.get<rd>() == 70 );
        BOOST_TEST( t4.get<i1>() == 0xffffffff);
        std::cout << "Value of i1: " << std::hex << t4.get<i1>() << std::endl;
        BOOST_TEST( t4.get<b1>() == true);
        BOOST_TEST( t4.get<b2>() == true);
    }

    // this test will also fail on 64 bit machines.
    {
        typedef bitfield_tuple<
            storage<unsigned long long>,
            member<unsigned int, i1, 32>,
            pointer<int, rd>,
            flag<b1>,
            flag<b2>
        >                   test_type_5;

        BOOST_TEST((
            is_same<
                test_type_5::storage_type,
                unsigned long long
            >::value
        ));

        test_type_5 t5;
        int i = 70;

        t5.get<i1>() = 0xffffffff;
        t5.get<b1>() = true;
        t5.get<b2>() = true;
        t5.get<rd>() = &i;

        BOOST_TEST( *t5.get<rd>() == 70 );
        BOOST_TEST( t5.get<i1>() == 0xffffffff);
        std::cout << "Value of i1: " << std::hex << t5.get<i1>() << std::endl;
        BOOST_TEST( t5.get<b1>() == true);
        BOOST_TEST( t5.get<b2>() == true);
    }
#endif

    {
        typedef bitfield_tuple<
            storage<unsigned long long>,
            member<unsigned int,i1,2>,
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

        BOOST_TEST((
            is_same<
                test_type_6::storage_type,
                unsigned long long
            >::value
        ));

        test_type_6 t6;
        int i = 70;
        
        // getting first 2 btis of pointer.
        detail::ptr_mask_type ptr = high_bits_mask<
            detail::ptr_mask_type,2>::value & detail::ptr_mask_type(&i);

        std::cout<< "value of &i: " << std::hex << &i << std::endl;
        std::cout<< "2 bits of &i: " << std::hex << ptr << std::endl;

        t6.get<i1>() = 0xf;
        t6.get<b1>() = true;
        t6.get<b2>() = true;
        t6.get<rd>() = &i;

        int* temp_ptr = t6.get<rd>();

        std::cout<< "Value of temp_ptr: " << std::hex << temp_ptr << std::endl;

        // rebuilding pointer.
        detail::ptr_mask_type temp_int = detail::ptr_mask_type(temp_ptr); 
        temp_int |= detail::ptr_mask_type( ptr );
        temp_ptr = my_intptr_t(temp_int);


        std::cout<< "Value of temp_int: " << std::hex<< temp_int << std::endl;
        std::cout<< "Value returned from t6.get<rd>(): " << std::hex <<
            temp_ptr << std::endl;

        BOOST_TEST( * temp_ptr == 70 );
        BOOST_TEST( t6.get<i1>() == 3u );
        BOOST_TEST( t6.get<b1>() == true );
        BOOST_TEST( t6.get<b2>() == true );
    }

    {
        typedef bitfield_tuple<
            storage<unsigned long long>,
            member<unsigned int,i1,3>,
            pointer<int, rd,
                bits_mask<
                    detail::ptr_mask_type,
                    2,
                    bit_width<
                        detail::ptr_mask_type
                    >::value - 4
                >
            >,
            flag<b1>
        >                   test_type_7;
        BOOST_TEST((
            is_same<
                test_type_7::storage_type,
                unsigned long long
            >::value
        ));

        test_type_7 t7;
        int i = 70;
        
        // getting first 2 bits of pointer.
        detail::ptr_mask_type ptr = high_bits_mask<
            detail::ptr_mask_type,2>::value & detail::ptr_mask_type(&i);

        std::cout<< "value of &i: " << std::hex << &i << std::endl;
        std::cout<< "2 bits of &i: " << std::hex << ptr << std::endl;
        t7.get<i1>() = 0x7;
        t7.get<b1>() = true;
        t7.get<rd>() = &i;

        int* temp_ptr = t7.get<rd>();

        std::cout<< "Value of temp_ptr: " << std::hex << temp_ptr << std::endl;

        detail::ptr_mask_type temp_int = detail::ptr_mask_type(temp_ptr); 
        temp_int |= detail::ptr_mask_type( ptr );
        temp_ptr = my_intptr_t(temp_int);

        std::cout<< "Value of temp_int: " << std::hex<< temp_int << std::endl;
        std::cout<< "Value returned from t7.get<rd>(): " << std::hex <<
            temp_ptr << std::endl;

        BOOST_TEST( *temp_ptr == 70 );
        BOOST_TEST( t7.get<i1>() == 7u );
        BOOST_TEST( t7.get<b1>() == true );
    }

    {
    typedef bitfield_tuple<
        pointer<int, rd,
            bits_mask<
                detail::ptr_mask_type,
                2,
                bit_width<
                    detail::ptr_mask_type
                >::value - 4
            >
        >,
        member<unsigned int, i1, 4>
    >                   test_type_8;
        BOOST_TEST((
            is_same<
                test_type_8::storage_type,
                detail::ptr_mask_type
            >::value
        ));

        test_type_8 t8;
        int i = 70;
        
        // getting first 2 bits of pointer.
        detail::ptr_mask_type ptr = high_bits_mask<
            detail::ptr_mask_type,2>::value & detail::ptr_mask_type(&i);

        std::cout<< "value of &i: " << std::hex << &i << std::endl;
        std::cout<< "2 bits of &i: " << std::hex << ptr << std::endl;

        t8.get<i1>() = 0xf;
        t8.get<rd>() = &i;

        int* temp_ptr = t8.get<rd>();

        std::cout<< "Value of temp_ptr: " << std::hex << temp_ptr << std::endl;

        detail::ptr_mask_type temp_int = detail::ptr_mask_type(temp_ptr); 
        temp_int |= detail::ptr_mask_type( ptr );
        temp_ptr = my_intptr_t(temp_int);

        std::cout<< "Value of temp_int: " << std::hex<< temp_int << std::endl;
        std::cout<< "Value returned from t8.get<rd>(): " << std::hex <<
            temp_ptr << std::endl;

        BOOST_TEST( *temp_ptr == 70 );
        BOOST_TEST( t8.get<i1>() == 0xf);
    }

    return boost::report_errors();
}
