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
typedef proxy_reference_type<char, 7>                   test_type_7;
typedef proxy_reference_type<long, 9>                   test_type_8;
typedef proxy_reference_type<long long, 17>             test_type_9;
typedef proxy_reference_type<long long, 50>             test_type_10;
typedef proxy_reference_type<short, 13>                 test_type_11;

bool display_debug = false;
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
        BOOST_TEST(t##NUMBER.m_mask.m_offset == 2); \
        BOOST_TEST(t##NUMBER.m_ptr == temp);

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
        BOOST_TEST(u##NUMBER.m_mask.m_offset == 2); \
        BOOST_TEST(u##NUMBER.m_ptr == temp);

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
        BOOST_TEST(u##NUMBER.m_mask.m_offset == 4);         \
        BOOST_TEST(u##NUMBER.m_ptr == temp2);        \
        u##NUMBER = t##NUMBER;                      \
        BOOST_TEST(u##NUMBER.m_mask.m_offset == 2);         \
        BOOST_TEST(u##NUMBER.m_ptr == temp);

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
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "single byte mask" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        storage_type storage[20];
        storage_ptr ptr = storage;
        *ptr = storage_type(0x7) << 5;
        if(display_debug) {
        std::cout << "Test type 1. First byte value in hex: " << std::endl;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        }
        // first within the first index.
        test_type_1 t1(ptr,0);
        if(display_debug) {
        std::cout << "fist use of t1" << std::endl;
        }
        test_type_1::value_type x = t1;
        BOOST_TEST(x == 0x7);
       
        // now trying second index. Still single byte mask testing
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "middle of byte mask single bit" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        test_type_1 t2(ptr,3);
        if(display_debug) {
        std::cout << "fist use of t2" << std::endl;
        }
        BOOST_TEST(t2 == 0);
        *ptr |= 0x1c;
        if(display_debug) {
        std::cout << "Test type 1. First byte value in hex for test 2: " << std::endl;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;
        std::cout << "second use of t2" << std::endl;
        std::cout << "Value returned by t2: "<< std::hex << t2 << std::endl;
        std::cout << "third use of t2" << std::endl;
        }
        BOOST_TEST(t2 == 7);
        
        // 2 byte mask testing.
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "2 byte mask" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        test_type_1 t3(ptr,6);
        *ptr = 0x03;
        ++ptr;
        *ptr = 0x80;
        --ptr;
        print_type_and_value(t3);
        print_mask_details(t3);
        BOOST_TEST( t3 == 7 );

        // two byte with second byte = to 8.
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "2 byte mask. 0x1 and 0xFF" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        ptr = storage;
        proxy_reference_type<unsigned int, 9> t4(ptr,7);
        std::memset(ptr,0,5);
        *ptr = 0x01;
        ++ptr;
        *ptr = 0xFF;
        ptr = storage;
        if(display_debug) {
        print_from_to(ptr,5);
        print_type_and_value(t4);
        print_mask_details(t4);
        }
        BOOST_PRINT_ON_TEST_FAILURE(t4,0x1FF);

        // testing multi byte > 2 
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Multi byte mask > 2 bytes" << std::endl;
        std::cout << "0 offset" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        test_type_4 t5(ptr,0);
        if(display_debug) {
        print_type_and_value(t5);
        print_mask_details(t5);
        }
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
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Multi byte mask > 2 bytes and ending with 0xFF" << std::endl;
        std::cout << "0 offset" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
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
        if(display_debug) {
        print_type_and_value(t6);
        print_mask_details(t6);
        }
        typedef unsigned long long ullt;
        ullt temp(0);
        temp = ~temp;
        temp <<=16;
        temp >>=16;
        BOOST_PRINT_ON_TEST_FAILURE(t6,temp);

        // testing multi byte > 2 and ending with 0xFF
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Multi byte mask > 2 63 bit long long" << std::endl;
        std::cout << "0 offset" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        proxy_reference_type<unsigned long long, 63> t7(ptr,0);
        std::memset(ptr,0,20);

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
        if(display_debug) {
        print_type_and_value(t7);
        print_mask_details(t7);
        }
        BOOST_TEST(t7 == 0x7fffffffffffffffll );
    }

    // testing value_type assignment operator.
    {
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "Testing assignment value_type assignement operator" << std::endl;
        std::cout << "-----------------------------------------" << std::endl
            << std::endl<< std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "1 byte offset 0" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);
        test_type_1 t1(ptr, 0);
        t1 = 0x7;
        // std::cout << std::hex << std::size_t(*ptr) << std::endl;
        print_mask_details(t1);
        BOOST_TEST(*ptr == 0xE0);

        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "1 byte offset 2" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        std::memset(ptr,0,1);
        test_type_1 t2(ptr, 2);
        t2 = 0x7;
        // std::cout << std::hex << std::size_t(*ptr) << std::endl;
        print_mask_details(t2);
        BOOST_TEST(*ptr == 0x38);
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "multi byte extraction" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        // std::memset(ptr,0,1);
        test_type_1 t3(ptr, 6);
        
        t3 = 0x7;
        // std::cout << std::hex << std::size_t(*ptr) << std::endl;
        BOOST_TEST(t2 == 7);
        ++ptr;
        // std::cout << std::hex << std::size_t(*ptr) << std::endl;
        BOOST_TEST(*ptr == 0x80);
        ptr = storage;
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "multi byte storage > 2" << std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
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
    }
    {

#define ASSIGN_MONITOR(P1,P2) \
    if(display_debug) {\
    print_mask_details(P1);\
    std::cout << #P2 << " Binary_value: " << to_binary(P2) << std::endl;\
    std::cout << #P1 << " = " << #P2 << std::endl;\
    std::cout << "before: "; \
    print_storage_for_reference(P1);\
    }\
    P1 = P2;\
    if(display_debug){\
    std::cout << "after: "; \
    print_storage_for_reference(P1);\
    std::cout << std::endl << std::endl;\
    }
        if(display_debug) {

        std::cout << "-----------------------------------------" << std::endl;
        std::cout << "memory corruption testing." << std::endl;
        std::cout << "-----------------------------------------" << std::endl
            << std::endl << std::endl;
        }

        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);
        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << " Testing type: " << typestr<test_type_1>() <<std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        test_type_1 t1(ptr,0);
        test_type_1 t2(ptr,3);
        test_type_1 t3(ptr,6);
        ++ptr;
        test_type_1 t4(ptr,1);
        test_type_1 t5(ptr,4);
        test_type_1 t6(ptr,7);
        ++ptr;
        test_type_1 t7(ptr,2);
        test_type_1 t8(ptr,5);


        ASSIGN_MONITOR(t1,0)
        ASSIGN_MONITOR(t2,1);
        ASSIGN_MONITOR(t3,2);
        ASSIGN_MONITOR(t4,3);
        ASSIGN_MONITOR(t5,4);
        if(display_debug) {
            std::cout << "=================================="<< std::endl;
            print_from_to(storage,4);
        }        
        ASSIGN_MONITOR(t6,5);
        if(display_debug) {
            print_from_to(storage,4);
            std::cout << "=================================="<< std::endl;
            std::cout << std::endl<< std::endl;
        }
        ASSIGN_MONITOR(t7,6);
        ASSIGN_MONITOR(t8,7);      

        BOOST_PRINT_ON_TEST_FAILURE_2(t1, 0);
        BOOST_PRINT_ON_TEST_FAILURE_2(t2, 1);
        BOOST_PRINT_ON_TEST_FAILURE_2(t3, 2);
        BOOST_PRINT_ON_TEST_FAILURE_2(t4, 3);
        BOOST_PRINT_ON_TEST_FAILURE_2(t5, 4);
        BOOST_PRINT_ON_TEST_FAILURE_2(t6, 5);
        BOOST_PRINT_ON_TEST_FAILURE_2(t7, 6);
        BOOST_PRINT_ON_TEST_FAILURE_2(t8, 7);

    }
    {

        if(display_debug) {
        std::cout << "-----------------------------------------" << std::endl;
        std::cout << " Testing type: " << typestr<test_type_2>() <<std::endl;
        std::cout << "-----------------------------------------" << std::endl;
        }
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);

        test_type_2 t1(ptr,0);
        test_type_2 t2(ptr,7);
        ++ptr;
        test_type_2 t3(ptr,6);
        ++ptr;
        test_type_2 t4(ptr,5);
        ++ptr;
        test_type_2 t5(ptr,4);
        ++ptr;
        test_type_2 t6(ptr,3);
        ++ptr;
        test_type_2 t7(ptr,2);
        ++ptr;
        test_type_2 t8(ptr,1);


        ASSIGN_MONITOR(t1,7)
        ASSIGN_MONITOR(t2,47);
        ASSIGN_MONITOR(t3,2);
        ASSIGN_MONITOR(t4,3);
        ASSIGN_MONITOR(t5,4);
        ASSIGN_MONITOR(t6,5);
        ASSIGN_MONITOR(t7,6);
        ASSIGN_MONITOR(t8,7);

        BOOST_PRINT_ON_TEST_FAILURE_2(t1, 7);
        BOOST_PRINT_ON_TEST_FAILURE_2(t2, 47);
        BOOST_PRINT_ON_TEST_FAILURE_2(t3, 2);
        BOOST_PRINT_ON_TEST_FAILURE_2(t4, 3);
        BOOST_PRINT_ON_TEST_FAILURE_2(t5, 4);
        BOOST_PRINT_ON_TEST_FAILURE_2(t6, 5);
        BOOST_PRINT_ON_TEST_FAILURE_2(t7, 6);
        BOOST_PRINT_ON_TEST_FAILURE_2(t8, 7);
        if(display_debug){
            print_from_to(storage,10);
        }
    }
#undef ASSIGN_MONITOR
    {
        if(display_debug) {
            std::cout << "-----------------------------------------" << std::endl;
            std::cout << " Testing type: " << typestr<test_type_2>() <<std::endl;
            std::cout << "-----------------------------------------" << std::endl;
        }
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);
// index | multiple of 17  | byte index | offset
//     0     17      0      0
//     1     34      2      1
//     2     51      4      2
//     3     68      6      3
//     4     85      8      4
//     5    102     10      5
//     6    119     12      6
//     7    136     14      7



        
        test_type_4 t1(ptr,0);
        ptr += 2;
        test_type_4 t2(ptr,1);
        ptr += 2;
        test_type_4 t3(ptr,2);
        ptr += 2; 
        test_type_4 t4(ptr,3);
        ptr += 2;
        test_type_4 t5(ptr,4);
        ptr += 2; 
        test_type_4 t6(ptr,5);
        ptr += 2; 
        test_type_4 t7(ptr,6);
        ptr += 2; 
        test_type_4 t8(ptr,7);


        t1 = 30204;
        t2 = 14140;
        t3 = 59713;
        t4 = 60100;
        t5 = 99999;
        t6 = 10210;
        t7 = 41740;
        t8 = 10129;
        BOOST_PRINT_ON_TEST_FAILURE_3(t1, 30204);
        BOOST_PRINT_ON_TEST_FAILURE_3(t2, 14140);
        BOOST_PRINT_ON_TEST_FAILURE_3(t3, 59713);
        BOOST_PRINT_ON_TEST_FAILURE_3(t4, 60100);
        BOOST_PRINT_ON_TEST_FAILURE_3(t5, 99999);
        BOOST_PRINT_ON_TEST_FAILURE_3(t6, 10210);
        BOOST_PRINT_ON_TEST_FAILURE_3(t7, 41740);
        BOOST_PRINT_ON_TEST_FAILURE_3(t8, 10129);
        // unsigned long long temp = t8;
        // std::cout << to_binary(temp) << std::endl;
        // 0  0000 10011  1100 10001
        if(display_debug) {
            print_storage_for_reference(t8);
            print_from_to(storage,17);
        }
    }
    // begining testing of the proxy reference type specialization for
    // signed numbers.
    {
        display_debug = false;
        if(display_debug) {
            std::cout << "=========================================" << std::endl;
            std::cout << " Signed type tests" << std::endl;
            std::cout << "=========================================" << std::endl;
        }

        typedef long long ll_type;
        int     int_sign_bit    = ~int(0) << (boost::bit_width<int>::value-1);
        char    char_sign_bit   = ~char(0) << (boost::bit_width<char>::value-1);
        short   short_sign_bit  = ~long(0) <<
            (boost::bit_width<short>::value-1);

        long    long_sign_bit   = ~long(0) << (boost::bit_width<long>::value-1);
        ll_type ll_sign_bit     = ~ll_type(0) <<
            (boost::bit_width<ll_type>::value-1);



       
#define PRINT_ON_SIGNBIT_FAILURE(P1, P2) \
    if(display_debug) {     \
        if(P1 != P2) {\
            std::cout << #P1 << " = " << to_binary(P1) << std::endl;\
            std::cout << #P2 << " = " << to_binary(P2) << std::endl;\
            \
        }\
    }\
    BOOST_TEST(P1 == P2);

        // testing sign bits.
        PRINT_ON_SIGNBIT_FAILURE(test_type_6::sign_bit, int_sign_bit);
        PRINT_ON_SIGNBIT_FAILURE(test_type_7::sign_bit,char_sign_bit);
        PRINT_ON_SIGNBIT_FAILURE(test_type_8::sign_bit,long_sign_bit);
        PRINT_ON_SIGNBIT_FAILURE(test_type_9::sign_bit,ll_sign_bit);
        PRINT_ON_SIGNBIT_FAILURE(test_type_10::sign_bit,ll_sign_bit);
        PRINT_ON_SIGNBIT_FAILURE(test_type_11::sign_bit,short_sign_bit);

        // testing stored sign bit retrieval.
        PRINT_ON_SIGNBIT_FAILURE(test_type_6::stored_sign_bit,
            int(1)<<(test_type_6::width-1) );
        PRINT_ON_SIGNBIT_FAILURE(test_type_7::stored_sign_bit,
            test_type_7::value_type(1)<<(test_type_7::width-1) );
        PRINT_ON_SIGNBIT_FAILURE(test_type_8::stored_sign_bit,
            test_type_8::value_type(1)<<(test_type_8::width-1) );
        PRINT_ON_SIGNBIT_FAILURE(test_type_9::stored_sign_bit,
            test_type_9::value_type(1)<<(test_type_9::width-1) );
        PRINT_ON_SIGNBIT_FAILURE(test_type_10::stored_sign_bit,
            test_type_10::value_type(1)<<(test_type_10::width-1) );
        PRINT_ON_SIGNBIT_FAILURE(test_type_11::stored_sign_bit,
            test_type_11::value_type(1)<<(test_type_11::width-1) );
#undef PRINT_ON_SIGNBIT_FAILURE

    }

    // testing signed field retrieval.
    {
        // int t = -15;
        // int value_bits = (~(~int(0) << 6)) & t;
        // std::cout << "t: " << to_binary(t) << std::endl;
        // std::cout << "value_bits: "<< to_binary(value_bits) << std::endl;

        // int int_min_val = ~int(0) << 6;
        // std::cout << "int_min_val: "<< to_binary(int_min_val) << std::endl;
        // t = value_bits ^ int_min_val;
        // std::cout << "resulting value: " << to_binary(t) << std::endl;
        // std::cout << "resulting value: " << std::dec << t << std::endl;
        // BOOST_TEST(false);
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);

        *ptr = 0xA0;
        
        test_type_6 t1(ptr, 0);
        BOOST_TEST( t1 == -3);
        *ptr = 0x20;
        BOOST_TEST( t1 == 1 );
        *ptr = 0x5E;
        test_type_7 t2(ptr, 0);
        BOOST_TEST(t2 == 47);
        *ptr = 0x51 << 1;

        BOOST_TEST(t2 == -47);
        // std::cout << to_binary(-47) <<std::endl;
        *ptr = 0x50 >> 1;
        ++ptr;
        *ptr = 0x80;
        ptr = storage;
        test_type_7 t3(ptr,2);
//         std::cout << to_binary(*ptr) << " ";
//         ++ptr;
//         std::cout << to_binary(*ptr) << std::endl;

        BOOST_TEST( t3 == -47);


        ptr = storage;
        std::memset(ptr,0,2);
        test_type_10 t4(ptr, 4);
        // 2710
        ++ptr;
        ++ptr; // 16
        ++ptr;
        ++ptr; // 32
        ++ptr;
        *ptr = 0x9c;
        ++ptr;
        *ptr = 0x40;
        ++ptr;
        BOOST_TEST( t4 == 10000);

        ptr = storage;
        *ptr = 0x01 << 3;
        BOOST_TEST(t4 == -562949953411312LL);        
    }

    // testing storage of signed types with proxy_reference_type
    {
        display_debug = true;
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;        
        storage_type storage[20];
        storage_ptr ptr = storage;
        std::memset(ptr,0,20);
        test_type_6 t1(ptr,0);
        t1 = -2;
        BOOST_TEST(t1 == -2);
    
    }


    return boost::report_errors();
}



