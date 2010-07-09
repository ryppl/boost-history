//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/endian.hpp>
#include <boost/integer/endian_binary_stream.hpp>
#include <boost/binary_stream.hpp>

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>

// used for working with the special conversion tools for creating 
// specific endianed storage.
#include <boost/type_traits/is_convertible.hpp>

#include <boost/mpl/bitwise.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <climits>

#include <boost/assert.hpp>
#include <iostream>
#include <typeinfo>
#include <cstdio>

struct red;
struct green;
struct blue;
using namespace boost;
using namespace boost::integer;
using namespace boost::bitfields;
using namespace std;

// I have a little endian for my desktop.


namespace boost { namespace bitfields { namespace endian {

struct little_endian_tag {};
struct big_endian_tag {};
struct pdp_endian_tag {};


struct native_tag
    :
#ifdef BOOST_LITTLE_ENDIAN
    little_endian_tag
#elif BOOST_BIG_ENDIAN
    big_endian_tag
#elif BOOST_PDP_ENDIAN
    pdp_endian_tag
#else
#error "We are in Big trouble here as we have encountered an unknown endian macro" \
" definition this may be a result of a change inside of <boost/detail/endain.hpp>" \
" or your processor isn't registered in the <boost/detail/endian.hpp> header file." 
#endif
{ };

typedef 
#ifdef BOOST_LITTLE_ENDIAN
    little_endian_tag
#elif BOOST_BIG_ENDIAN
    big_endian_tag
#elif BOOST_PDP_ENDIAN
    pdp_endian_tag
#else
#error "We are in Big trouble here as we have encountered an unknown endian macro" \
" definition this may be a result of a change inside of <boost/detail/endain.hpp>" \
" or your processor isn't registered in the <boost/detail/endian.hpp> header file." 
#endif
native;

template <std::size_t Order>
struct _byte_order_from_macro
    :integral_constant<std::size_t, Order>
{ };

// this is a typedef that defines the byte order statically instead of
// via macro that way it can be reasoned about.
typedef _byte_order_from_macro<BOOST_BYTE_ORDER>    byte_order;

typedef integral_constant <std::size_t, CHAR_BIT> char_bit;

/** dumb stuff to remember about endianness
 *  all single bytes stored left to right in bits ALWAYS 0 - 7
 *  little endian is 1234
 *  bit endian is 4321
 *
 *  1234
 *  means that the 0 bit of the integer is first.
 *  
 *  4321
 *  means that 31st bit is first.
 *  
 *  little endian short  -- to -- big endian short
 *  0x12     --->   0x21
 *  
 *  
 *  
 *  
 *  
 *  
 */
namespace details {

template <
    std::size_t ByteIndex,
    typename InIntegralC,
    typename OutIntegralC
>
struct calculate_out_integral_c {

    // create mask for getting the next byte.
    typedef bits_mask<
        typename InIntegralC::value_type,
        ByteIndex * char_bit::value,
        char_bit::value
    >                               mask;

    // gettting next byte's value
    typedef typename mpl::bitand_<
        InIntegralC,
        mask
    >::type                         current_byte_value;

    // shift back to the starting point. 
    typedef typename mpl::shift_right<
        current_byte_value,
        mpl::size_t<ByteIndex * char_bit::value>
    >::type                         right_shifted_byte;
    
    // shift to correct position.
    typedef typename mpl::shift_left<
        right_shifted_byte,
        mpl::size_t<
        ((sizeof(typename InIntegralC::value_type) - 1 - ByteIndex)
          *
        char_bit::value )>
    >::type                         positioned_byte;

    typedef typename mpl::bitor_<
        OutIntegralC,
        positioned_byte
    >::type                     type;
};


template <
    std::size_t ByteIndex,
    typename InIntegralC,
    typename OutIntegralC,
    bool Continue = true
>
struct conversion_loop
    :conversion_loop<
        ByteIndex + 1, // increment ByteIndex so I move to the next byte.
        InIntegralC,
        typename calculate_out_integral_c<
            ByteIndex,
            InIntegralC,
            OutIntegralC
        >::type,
        // now evaluating the boolean condition for continuation of loop.
        (ByteIndex < (sizeof(typename InIntegralC::value_type)-1) )
    >
{ };

// this kills the loop.
template <
    std::size_t ByteIndex,
    typename InIntegralC,
    typename OutIntegralC
>
struct conversion_loop<ByteIndex,InIntegralC,OutIntegralC,false> {
    typedef OutIntegralC type;
};


// calls the loop that converts the endianness.
template <typename IntegralConstType, typename EndiannessTag>
struct convertion_helper {
    typedef typename conversion_loop<
        0,
        IntegralConstType,
        integral_constant<
            typename IntegralConstType::value_type,
            0
        >
    >::type                 type;
};

// trival case where native endianness is being used or something convertible to
// the native_tag is being used.
template <typename IntegralConstType>
struct convertion_helper <IntegralConstType,native_tag> {
    typedef IntegralConstType type;
};


}

/** Yes this will take an integral constant and change the encoded endianness
 *  of it to be which ever you choose accept for PDP because I don't know
 *  just how to deal with that one just yet.
 */
template <typename IntegralConstantType, typename ConvertTo = native_tag >
struct set_endianness {
    typedef typename mpl::if_<
        is_convertible<ConvertTo,native_tag>,
        native_tag,
        ConvertTo
    >::type                     endianness_tag;
    typedef typename details::convertion_helper<
        IntegralConstantType,
        endianness_tag
    >::type         type;
};

}}} // end boost::bitfields::endian

using namespace boost::bitfields::endian;

typedef bitfield_tuple<
    storage< big32_t >,
    member<unsigned char, red, 5>,
    member<unsigned char, green, 6>,
    member<unsigned char, blue, 5>
>                               rgb565_t;
typedef set_endianness<
    integral_constant<unsigned int, 0xDEADBEEF>,
    big_endian_tag
>::type             converted_type;

typedef integral_constant<unsigned int, 0xDEADBEEF> beefy;
typedef integral_constant<unsigned int, 0> start_of_the_end;

int main() {
    {
        // cout << typeid(native).name() << endl;
        // cout << byte_order() << endl;
        // typedef integer::bitfield<ubig32_t,0,3> a_bitfield;
        // cout << is_same<unative32_t,ulittle32_t>::value << endl;
        // cout << is_same<unative32_t,ubig32_t>::value << endl;
        // cout << typeid(converted_type).name() << endl;
        // cout << converted_type() << endl;

        printf("%0x\n",0xdeadbeef);
        printf("%0x\n", converted_type::value );

        typedef boost::bitfields::endian::details::calculate_out_integral_c<
            0,
            beefy,
            start_of_the_end
        >::type                     step_one;
        printf("Step 1: %0x\n", step_one::value );

        typedef boost::bitfields::endian::details::calculate_out_integral_c<
            1,
            beefy,
            step_one
        >::type                     step_two;
        printf("Step 2: %0x\n", step_two::value );



        typedef boost::bitfields::endian::details::calculate_out_integral_c<
            2,
            beefy,
            step_two
        >::type                     step_three;
        printf("Step 3: %0x\n", step_three::value );


        typedef boost::bitfields::endian::details::calculate_out_integral_c<
            3,
            beefy,
            step_three
        >::type                     step_four;
        printf("Step 4: %0x\n", step_four::value );

        // BOOST_TEST(false);

    }
    {
    #if 0
    rgb565_t rgb565;
    rgb565.get<red>() = 3;
    BOOST_TEST(rgb565.get<red>() == 3);
    rgb565.get<0>() = 4;
    BOOST_TEST(rgb565.get<0>() == 4);
    #endif
    }
    return boost::report_errors();
}
