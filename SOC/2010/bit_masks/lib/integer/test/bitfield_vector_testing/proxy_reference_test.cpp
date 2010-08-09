//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost::detail;

/** Proxy Reference Type.
 *  Suite for proxy reference type.
 */

// unsigned type testing.
typedef proxy_reference_type<unsigned int, 3>           test_type_1;
typedef proxy_reference_type<unsigned char, 7>          test_type_2;
typedef proxy_reference_type<unsigned long, 9>          test_type_3;
typedef proxy_reference_type<unsigned long long, 17>    test_type_4;
typedef proxy_reference_type<unsigned long long, 50>    test_type_5;

// signed type testing.
typedef proxy_reference_type<int, 3>                    test_type_6;
typedef proxy_reference_type<char, 9>                   test_type_7;
typedef proxy_reference_type<long, 7>                   test_type_8;
typedef proxy_reference_type<long long, 17>             test_type_9;
typedef proxy_reference_type<long long, 50>             test_type_10;


/*
test_type_1;
test_type_2;
test_type_3;
test_type_4;
test_type_5;
test_type_6;
test_type_7;
test_type_8;
test_type_9;
test_type_10;
*/

int main() {
    // Constructor tests.
    {
        unsigned char temp[20];
#define TEST_MACRO_PTR_OFFSET_CTOR(NUMBER) \
        test_type_##NUMBER t##NUMBER(temp,2); \
        BOOST_TEST(t##NUMBER._mask._offset == 2); \
        BOOST_TEST(t##NUMBER._ptr == temp);

        TEST_MACRO_PTR_OFFSET_CTOR(1);
        TEST_MACRO_PTR_OFFSET_CTOR(2);
        TEST_MACRO_PTR_OFFSET_CTOR(3);
        TEST_MACRO_PTR_OFFSET_CTOR(4);
        TEST_MACRO_PTR_OFFSET_CTOR(5);
        TEST_MACRO_PTR_OFFSET_CTOR(6);
        TEST_MACRO_PTR_OFFSET_CTOR(7);
        TEST_MACRO_PTR_OFFSET_CTOR(8);
        TEST_MACRO_PTR_OFFSET_CTOR(9);
        TEST_MACRO_PTR_OFFSET_CTOR(10);

#undef TEST_MACRO_PTR_OFFSET_CTOR
    }
    // testing copy constructor.
    {
        unsigned char temp[20];
#define TEST_MACRO_COPY_CTOR(NUMBER) \
        test_type_##NUMBER t##NUMBER(temp,2); \
        test_type_##NUMBER u##NUMBER(t##NUMBER); \
        BOOST_TEST(u##NUMBER._mask._offset == 2); \
        BOOST_TEST(u##NUMBER._ptr == temp);

        TEST_MACRO_COPY_CTOR(1);
        TEST_MACRO_COPY_CTOR(2);
        TEST_MACRO_COPY_CTOR(3);
        TEST_MACRO_COPY_CTOR(4);
        TEST_MACRO_COPY_CTOR(5);
        TEST_MACRO_COPY_CTOR(6);
        TEST_MACRO_COPY_CTOR(7);
        TEST_MACRO_COPY_CTOR(8);
        TEST_MACRO_COPY_CTOR(9);
        TEST_MACRO_COPY_CTOR(10);

#undef  TEST_MACRO_COPY_CTOR
    }

    // copy assignment operator.
    {
        unsigned char temp[20];
        unsigned char temp2[30];
#define TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(NUMBER) \
        test_type_##NUMBER t##NUMBER(temp,2);       \
        test_type_##NUMBER u##NUMBER(temp2,4);      \
        BOOST_TEST(u##NUMBER._mask._offset == 4);         \
        BOOST_TEST(u##NUMBER._ptr == temp2);        \
        u##NUMBER = t##NUMBER;                      \
        BOOST_TEST(u##NUMBER._mask._offset == 2);         \
        BOOST_TEST(u##NUMBER._ptr == temp);

        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(1);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(2);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(3);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(4);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(5);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(6);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(7);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(8);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(9);
        TEST_MACRO_COPY_ASSIGNMENT_OPERATOR(10);

#undef TEST_MACRO_COPY_ASSIGNMENT_OPERATOR
    }
    // encoding and decoding tests.
    {

        // single byte mask testing
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "single byte mask" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        storage_type storage[20];
        storage_ptr ptr = storage;
        *ptr = storage_type(0x7) << 5;
        std::cout << "Test type 1. First byte value in hex: " << std::endl;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        // first within the first index.
        test_type_1 t1(ptr,0);
        std::cout << "fist use of t1" << std::endl;
        test_type_1::value_type x = t1;
        BOOST_TEST(x == 0x7);
       
        // now trying second index. Still single byte mask testing
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "middle of byte mask single bit" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        test_type_1 t2(ptr,3);
        std::cout << "fist use of t2" << std::endl;
        BOOST_TEST(t2 == 0);
        *ptr |= 0x1c;
        std::cout << "Test type 1. First byte value in hex for test 2: " << std::endl;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        std::cout << "second use of t2" << std::endl;
        std::cout << "Value returned by t2: "<< std::hex << t2 << std::endl;
        std::cout << "third use of t2" << std::endl;
        BOOST_TEST(t2 == 7);
        
        // 2 byte mask testing.
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "2 byte mask" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        test_type_1 t3(ptr,6);
        *ptr = 0x03;
        ++ptr;
        *ptr = 0x80;
        --ptr;
        print_type_and_value(t3);
        print_mask_details(t3);
        BOOST_TEST( t3 == 7 );

        // two byte with second byte = to 8.
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "2 byte mask. 0x1 and 0xFF" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        proxy_reference_type<unsigned int, 9> t4(ptr,7);
        std::memset(ptr,0,3);
        *ptr = 0x01;
        ++ptr;
        *ptr = 0xFF;
        --ptr;
        print_type_and_value(t4);
        print_mask_details(t4);
        BOOST_TEST(t4 == 0x1FF);

        // testing multi byte > 2 
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Multi byte mask > 2 bytes" << std::endl;
        std::cout << "0 offset" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        test_type_4 t5(ptr,0);
        print_type_and_value(t5);
        print_mask_details(t5);
        std::memset(ptr,0,4);
        *ptr = 0xFF;
        ++ptr;
        *ptr = 0xFF;
        ++ptr;
        *ptr = 0x80;
        ptr = storage;

        // expected mask 
        // 0xFF 0xFF 0x80

        BOOST_TEST(t5 == 0x1FFFF);

        // testing multi byte > 2 and ending with 0xFF
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Multi byte mask > 2 bytes and ending with 0xFF" << std::endl;
        std::cout << "0 offset" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        proxy_reference_type<unsigned long long, 48> t6(ptr,0);
        std::memset(ptr,0,4);

        *ptr = 0xFF;
        ++ptr;
        *ptr = 0xFF;
        ++ptr;
        *ptr = 0xFF;
        ++ptr;
        *ptr = 0xFF;
        ++ptr;
        *ptr = 0xFF;
        ++ptr;
        *ptr = 0xFF;

        ptr = storage;
        print_type_and_value(t6);
        print_mask_details(t6);
        typedef unsigned long long ullt;
        BOOST_TEST(t6 == ullt(0xFFFFFFFFFF));


        // testing multi byte > 2 and ending with 0xFF
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Multi byte mask > 2 63 bit long long" << std::endl;
        std::cout << "0 offset" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        proxy_reference_type<unsigned long long, 63> t7(ptr,0);
        std::memset(ptr,0,4);
        // 1
        *ptr = 0xFF;
        ++ptr;
        // 2
        *ptr = 0xFF;
        ++ptr;
        // 3
        *ptr = 0xFF;
        ++ptr;
        // 4
        *ptr = 0xFF;
        ++ptr;
        // 5
        *ptr = 0xFF;
        ++ptr;
        // 6
        *ptr = 0xFF;
        ++ptr;
        // 7
        *ptr = 0xFF;
        ++ptr;
        // 8
        *ptr = 0xFE;
        ++ptr;
        ptr = storage;
        print_type_and_value(t7);
        print_mask_details(t7);
        BOOST_TEST(t7 == 0x7fffffffffffffff );
    }

    // testing value_type assignment operator.
    {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Testing assignment value_type assignement operator" << std::endl;
        std::cout << "-----------------------------------------" << std::endl
            << std::endl<< std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "1 byte offset 0" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);
        test_type_1 t1(ptr, 0);
        t1 = 0x7;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        print_mask_details(t1);
        BOOST_TEST(*ptr == 0xE0);


        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "1 byte offset 2" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::memset(ptr,0,1);
        test_type_1 t2(ptr, 2);
        t2 = 0x7;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        print_mask_details(t2);
        BOOST_TEST(*ptr == 0x38);

        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "multi byte extraction" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        // std::memset(ptr,0,1);
        test_type_1 t3(ptr, 6);
        
        t3 = 0x7;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        BOOST_TEST(t2 == 7);
        ++ptr;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        BOOST_TEST(*ptr == 0x80);
        ptr = storage;

        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "multi byte storage > 2" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::memset(ptr,0,2);
        test_type_4 t4(ptr,0);
        t4 = 0x1FFFFu;
        BOOST_TEST(*ptr == 0xFF);
        ++ptr;
        BOOST_TEST(*ptr == 0xFF);
        ++ptr;
        BOOST_TEST(*ptr == 0x80);
        ptr = storage;
        BOOST_TEST(t4 == 0x1FFFF);
        t4 = 17;
        BOOST_TEST(t4 == 17);
        
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "memory corruption testing." << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        // test_type_4 t4(ptr,0);
        // BOOST_TEST(false);

/*
test_type_1;
test_type_2;
test_type_3;
test_type_4;
test_type_5;
test_type_6;
test_type_7;
test_type_8;
test_type_9;
test_type_10;
*/
    }
    return boost::report_errors();
}



