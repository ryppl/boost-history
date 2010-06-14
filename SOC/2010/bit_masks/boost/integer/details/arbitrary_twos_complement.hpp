//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ARBITRARY_TWOS_COMPLEMENT_HPP
#define BOOST_ARBITRARY_TWOS_COMPLEMENT_HPP

#include <boost/static_assert.hpp>
#include <boost/integer/bits_mask.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/not.hpp>

// std includes.
#include <cstddef>
#include <stdexcept>

namespace boost { namespace details {

template <  typename Encoded_Type,
            std::size_t Width,
            typename Return_Type,
            bool Is_Signed
>
struct twos_complement_decoding_impl;


template <typename Decoding_From_Type, std::size_t Width, typename Return_Type>
struct twos_complement_decoding_impl<Decoding_From_Type,Width,Return_Type,true> {
    Return_Type operator() (Decoding_From_Type const& encoded_value) {
        // sign bit
        Return_Type sign_bit = static_cast<Return_Type>(
            encoded_value
              &
            bits_mask<
                Decoding_From_Type,
                Width - 1
            >::value
        );
        // Value Bits.
        Return_Type value_bits = 
            encoded_value
              &
            low_bits_mask<Decoding_From_Type,Width-1>::value;

        return value_bits + (Return_Type(0) - sign_bit);
    }
};

template <typename Decoding_Type, std::size_t Width, typename Return_Type >
struct twos_complement_decoding_impl<Decoding_Type,Width,Return_Type,false> {
    Return_Type operator() (Decoding_Type const& non_encoded_value) {
        return non_encoded_value;
    }
};



template <  typename Encoding_Type,
            std::size_t Width,
            typename Return_Type,
            bool Is_Signed
>
struct twos_complement_encoding_impl;


template <typename Encoding_Type, std::size_t Width, typename Return_Type>
struct twos_complement_encoding_impl<Encoding_Type, Width, Return_Type, true> {
    Return_Type operator() (Encoding_Type const& value_to_encode) {
        return static_cast<
            typename make_unsigned<Encoding_Type>::type
        >(
            value_to_encode & low_bits_mask<Return_Type, Width - 1>::value
        )
          ^
        (static_cast<
            typename make_unsigned<Encoding_Type>::type
        >(
            value_to_encode & bits_mask<
                                Encoding_Type, 
                                bit_width<Encoding_Type>::value - 1 
                            >::value 
        ) >> (bit_width<Encoding_Type>::value - Width));
    }
};

template <typename Encoding_Type, std::size_t Width, typename Return_Type>
struct twos_complement_encoding_impl<Encoding_Type, Width, Return_Type, false> {
    Return_Type operator() (Encoding_Type const& value_to_encode) {
        return Return_Type(value_to_encode);
    }
};



} // end details.

template <typename Encoded_Type, std::size_t Width, typename Return_Type>
Return_Type twos_complement_encoding(Encoded_Type  x) {
    BOOST_STATIC_ASSERT( Width != 0 );
    return details::twos_complement_encoding_impl<
        Encoded_Type,
        Width,
        Return_Type,
        is_signed< Encoded_Type >::value
          &&
        bit_width< Encoded_Type >::value != Width
          &&
        mpl::not_< is_same< Encoded_Type, bool > >::value
          &&
        Width != 1
    >()(x);
}

template <typename Decoding_Type, std::size_t Width, typename Return_Type>
Return_Type twos_complement_decoding(Decoding_Type x) {
    BOOST_STATIC_ASSERT( Width != 0 );
    return details::twos_complement_decoding_impl<
        Decoding_Type,
        Width,
        Return_Type,
        is_signed< Return_Type >::value
          &&
        bit_width< Return_Type >::value != Width
          &&
        mpl::not_< is_same< Return_Type, bool > >::value
          &&
        Width != 1
    >()( x );
}

}



#endif
