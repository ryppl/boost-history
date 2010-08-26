//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/detail/lightweight_test.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <typeinfo>
#include <cstring>
#include <bitset>
#include <boost/integer/bit_width.hpp>
#include <sstream>
#include <boost/type_traits/is_same.hpp>

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

#ifndef BOOST_BITFIELD_VECTOR_TESTING_UTILITYIES_HPP
#define BOOST_BITFIELD_VECTOR_TESTING_UTILITYIES_HPP



template <typename T>
std::string typestr() {
#if defined(__GNUC__)
    std::size_t const BUFSIZE = 8192;
    std::size_t n = BUFSIZE;
    char buf[BUFSIZE];
    abi::__cxa_demangle(typeid(T).name(), buf, &n, 0);
    return std::string(buf, ::strlen(buf));
#else
    return std::string(typeid(T).name());
#endif
}

template <typename T>
inline std::string typestr(T const&)
{ return typestr<T>(); }


#define BOOST_PRINT_ON_TEST_FAILURE(P1, P2) \
    if(P1 != P2 ) { \
        std::cout << #P1 << ": " << std::hex << std::size_t(P1) << std::endl; \
        std::cout << #P2 << ": " << std::hex << std::size_t(P2) << std::endl; \
    }\
    BOOST_TEST( P1 == P2);

#define BOOST_PRINT_ON_TEST_FAILURE_2(P1, P2) \
    if(P1 != P2 ) { \
        std::cout << #P1 << ": " << std::hex << std::size_t(P1) << std::endl; \
        std::cout << #P2 << ": " << std::hex << std::size_t(P2) << std::endl; \
        print_mask_details(P1);\
    }\
    BOOST_TEST( P1 == P2);


#define BOOST_PRINT_ON_TEST_FAILURE_3(P1, P2) \
    if(P1 != P2 ) { \
        std::cout << #P1 << ": " << std::dec << std::size_t(P1) << std::endl; \
        std::cout << #P2 << ": " << std::dec << std::size_t(P2) << std::endl; \
        print_mask_details(P1);\
    }\
    BOOST_TEST( P1 == P2);

struct print_mpl_values {
    template<typename T>
    void operator()(T x) {
        std::cout<< x << " ";
    }
};

template<typename T>
void print_type_and_value( T const& x) {
    typedef unsigned long long ulltype;
    std::cout << "Type: " << typestr<T>() << "  Value:"
        << std::hex << ulltype(x) << std::endl;
}

template <typename T>
std::string to_binary(T
#if 0
x) {
    typedef unsigned long long ullt;
    std::stringstream ss(std::stringstream::in|std::stringstream::out);
    ss << std::setfill('0') << std::setw(boost::bit_width<T>::value)
        << std::bitset<  boost::bit_width<T>::value >(ullt(x)).to_string();
    return std::string(ss.str());
#else
) {
    return std::string();
#endif
}

void print_from_to(unsigned char* ptr, std::size_t to) {
    for(std::size_t i =1;i< to + 1;++i) {
        std::cout << std::dec << std::setw(8) << i*8 << " ";
    }
    std::cout << std::endl;

    for(std::size_t i = 0; i < to; ++i) {
        std::cout << to_binary(ptr[i]) << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void print_mask_details( T const& x) {
    typedef unsigned long long ulltype;
    std::cout << "Mask detail for the following class" << std::endl;
    std::cout << "Type: " << typestr<T>() << std::endl;
    std::cout << "Mask Values " << std::endl;
    std::cout << "size: " <<  x.m_mask.m_size << std::endl;
    std::cout << "offset: " <<  x.m_mask.m_offset << std::endl;
    std::cout << "first_byte: " <<  std::hex << ulltype(x.m_mask.m_first_byte) << std::endl;
    std::cout << "first_byte in binary: " <<  to_binary(x.m_mask.m_first_byte) << std::endl;
    std::cout << "last_byte: " <<  std::hex << ulltype(x.m_mask.m_last_byte) << std::endl;
    std::cout << "last_byte in binary: " <<   to_binary(x.m_mask.m_last_byte) << std::endl;
    std::cout << "last_shift: " << x.m_mask.m_last_shift << std::endl << std::endl;
}
template<typename T>
void print_storage_for_reference(T const& x) {
    std::cout << "Storage For Reference Type" << std::endl;
    print_from_to(x.m_ptr, x.m_mask.m_size);
}

template <typename T>
void print_typestr() {
    std::cout << typestr<T>() << std::endl;
}
#endif
