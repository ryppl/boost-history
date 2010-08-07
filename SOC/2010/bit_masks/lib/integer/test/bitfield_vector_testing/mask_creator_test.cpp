//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/detail/bitfield_vector/mask_creator.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/type_traits/is_same.hpp>

#include <string>

#include <typeinfo>

#include <iostream>
#include <iomanip>

#if defined(__GNUC__)
#include <cstring>
#include <cxxabi.h>
#endif

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





struct print_set {
    template<typename T>
    void operator()(T x) {
        std::cout<< x << " ";
    }
};

using namespace boost::detail;
int main() {
    // testing mask size meta function
    {
        typedef mask_size<
            boost::mpl::size_t<0>,
            boost::mpl::size_t<4>
        >::type test_1;
        BOOST_TEST( test_1::value == 1 );

        typedef mask_size<
            boost::mpl::size_t<6>,
            boost::mpl::size_t<3>
        >::type test_2;
        BOOST_TEST( test_2::value == 2 );

        typedef mask_size<
            boost::mpl::size_t<0>,
            boost::mpl::size_t<50>
        >::type test_3;
        BOOST_TEST( test_3::value == 7 );
    }

    // Testing for generation of all valid offsets for a perticular width
    {
        using namespace boost;
        typedef determine_vaild_offsets<3>::type t1;
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<0> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<1> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<2> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<3> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<4> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<5> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<6> >::type::value ));
        BOOST_TEST(( mpl::has_key<t1,mpl::size_t<7> >::type::value ));

        typedef determine_vaild_offsets<4>::type t2;
        BOOST_TEST(( mpl::has_key<t2,mpl::size_t<0> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t2,mpl::size_t<1> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t2,mpl::size_t<2> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t2,mpl::size_t<3> >::type::value ));
        BOOST_TEST(( mpl::has_key<t2,mpl::size_t<4> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t2,mpl::size_t<5> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t2,mpl::size_t<6> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t2,mpl::size_t<7> >::type::value ));


        typedef determine_vaild_offsets<50>::type t3;
        BOOST_TEST(( mpl::has_key<t3,mpl::size_t<0> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t3,mpl::size_t<1> >::type::value ));
        BOOST_TEST(( mpl::has_key<t3,mpl::size_t<2> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t3,mpl::size_t<3> >::type::value ));
        BOOST_TEST(( mpl::has_key<t3,mpl::size_t<4> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t3,mpl::size_t<5> >::type::value ));
        BOOST_TEST(( mpl::has_key<t3,mpl::size_t<6> >::type::value ));
        BOOST_TEST(( !mpl::has_key<t3,mpl::size_t<7> >::type::value ));

        typedef determine_vaild_offsets<11>::type t4;
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<0> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<1> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<2> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<3> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<4> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<5> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<6> >::type::value ));
        BOOST_TEST(( mpl::has_key<t4,mpl::size_t<7> >::type::value ));

    }

    // testing calc_first_byte
    {
        BOOST_PRINT_ON_TEST_FAILURE((calc_first_byte<7,3>::type::value),  0x1 );
        BOOST_PRINT_ON_TEST_FAILURE((calc_first_byte<2,3>::type::value),  0x38);
        BOOST_PRINT_ON_TEST_FAILURE((calc_first_byte<6,3>::type::value),  0x3 );
        BOOST_PRINT_ON_TEST_FAILURE((calc_first_byte<0,3>::type::value),  0xE0);
        BOOST_PRINT_ON_TEST_FAILURE((calc_first_byte<6,50>::type::value), 0x3 );
        BOOST_PRINT_ON_TEST_FAILURE((calc_first_byte<0,50>::type::value), 0xFF);
    }
    
    // testing calc_last_byte
    {

        BOOST_PRINT_ON_TEST_FAILURE((calc_last_byte<7,3>::type::value), 0xC0 );
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_byte<2,3>::type::value), 0x0);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_byte<6,3>::type::value),  0x80);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_byte<0,3>::type::value),  0x0);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_byte<6,50>::type::value), 0xFF );
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_byte<0,50>::type::value), 0xC0);
    }

    // testing calc_last_shift
    {
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_shift<7,3>::type::value),  6);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_shift<2,3>::type::value),  3);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_shift<6,3>::type::value),  7);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_shift<0,3>::type::value),  5);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_shift<6,50>::type::value), 8);
        BOOST_PRINT_ON_TEST_FAILURE((calc_last_shift<0,50>::type::value), 6);
    }

    // testing mask_info
    {
        
        BOOST_PRINT_ON_TEST_FAILURE((mask_info<7,3>::offset),          7);
        BOOST_PRINT_ON_TEST_FAILURE((mask_info<7,3>::width),           3);
        BOOST_PRINT_ON_TEST_FAILURE((mask_info<7,3>::size),            2);
        BOOST_PRINT_ON_TEST_FAILURE((mask_info<7,3>::first_value),   0x1);
        BOOST_PRINT_ON_TEST_FAILURE((mask_info<7,3>::last_value),   0xC0);
        BOOST_PRINT_ON_TEST_FAILURE((mask_info<7,3>::last_shift),      6);

    }
    // testing create_masks
    {

        using namespace boost;
        typedef create_masks<3>::type   mask_vector;

        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<0> >::type,
            mask_info<0,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<1> >::type,
            mask_info<1,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<2> >::type,
            mask_info<2,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<3> >::type,
            mask_info<3,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<4> >::type,
            mask_info<4,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<5> >::type,
            mask_info<5,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<6> >::type,
            mask_info<6,3> >::value ));
        BOOST_TEST(( is_same<
            mpl::at< mask_vector, mpl::size_t<7> >::type,
            mask_info<7,3> >::value ));       
    }
    return boost::report_errors();
}
