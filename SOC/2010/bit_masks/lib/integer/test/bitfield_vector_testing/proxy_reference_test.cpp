//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <iostream>
#include <iomanip>

using namespace boost::detail;

/** Proxy Reference Type.
 *  Suite for proxy reference type.
 */

// unsigned type testing.
typedef proxy_reference_type<unsigned int, 3>           test_type_1;
typedef proxy_reference_type<unsigned char, 9>          test_type_2;
typedef proxy_reference_type<unsigned long, 7>          test_type_3;
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
        BOOST_TEST(t##NUMBER._offset == 2); \
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
        BOOST_TEST(u##NUMBER._offset == 2); \
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
        BOOST_TEST(u##NUMBER._offset == 4);         \
        BOOST_TEST(u##NUMBER._ptr == temp2);        \
        u##NUMBER = t##NUMBER;                      \
        BOOST_TEST(u##NUMBER._offset == 2);         \
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
        typedef unsigned char storage_type;
        typedef storage_type* storage_ptr;
        storage_type storage[20];
        storage_ptr ptr = storage;
        *ptr = storage_type(0x7) << 5;
        std::cout << "Test type 1. First byte value in hex: " << std::endl;
        std::cout << std::hex << std::size_t(*ptr) << std::endl;

        // this will be access to the first element within the array.
        test_type_1 t1(ptr,0);
        test_type_1::value_type x = t1;
        std::cout << "Value returned by t1: "<< std::hex << x << std::endl;
        BOOST_TEST(x == 0x7);
        test_type_1 t1(ptr,0);
    }
    return boost::report_errors();
}


