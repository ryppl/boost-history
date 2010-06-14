//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



// boost includes.
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/integer/bits_mask.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>

// std includes.
#include <cstddef>
#include <stdexcept>
#include <iostream>

using namespace std;
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
        is_signed<Encoded_Type>::value &&
            bit_width<Return_Type>::value != Width
    >()(x);
}

template <typename Decoding_Type, std::size_t Width, typename Return_Type>
Return_Type twos_complement_decoding(Decoding_Type x) {
    BOOST_STATIC_ASSERT( Width != 0 );
    return details::twos_complement_decoding_impl<
        Decoding_Type,
        Width,
        Return_Type,
        is_signed<Return_Type>::value &&
            bit_width<Return_Type>::value != Width
    >()( x );
}

}




using namespace boost;
using namespace std;
int main() {
    {
    char encoded_value = -2;

    cout << bool( (encoded_value & bits_mask<char,7>::value) >> bits_mask<char,7>::offset );
    cout << bool( (encoded_value & bits_mask<char,6>::value) >> bits_mask<char,6>::offset );
    cout << bool( (encoded_value & bits_mask<char,5>::value) >> bits_mask<char,5>::offset );
    cout << bool( (encoded_value & bits_mask<char,4>::value) >> bits_mask<char,4>::offset );
    cout << bool( (encoded_value & bits_mask<char,3>::value) >> bits_mask<char,3>::offset );
    cout << bool( (encoded_value & bits_mask<char,2>::value) >> bits_mask<char,2>::offset );
    cout << bool( (encoded_value & bits_mask<char,1>::value) >> bits_mask<char,1>::offset );
    cout << bool( (encoded_value & bits_mask<char,0>::value) >> bits_mask<char,0>::offset );
    cout << endl;


    
    // BOOST_ASSERT((storage == 0xc));

    // 1111 1110  == -2 in a char.
    // Return_Type(encoded_value & low_bits_mask<Return_Type, Width - 1>::value)
    // - Return_Type(encoded_value & bits_mask<Decoding_Type, Width>::value);
    const unsigned int  Width = 4;
    typedef std::size_t Return_Type;
    typedef char        Encoding_Type;
    Return_Type eval_1(encoded_value & low_bits_mask<Return_Type, Width - 1>::value );
    BOOST_ASSERT(( encoded_value == static_cast<char>(0xfe)));
    BOOST_ASSERT(( eval_1 == bits_mask<Return_Type, 1,2>::value ));
    
    // get the sign bit
    Return_Type eval_2(
        static_cast<
            make_unsigned<Encoding_Type>::type
        >(
            encoded_value & bits_mask<
                                Encoding_Type, 
                                bit_width<Encoding_Type>::value - 1
                            >::value 
        ) >> (bit_width<Encoding_Type>::value - Width)
    );
    BOOST_ASSERT(( eval_2 == 0x8));

    // this is how to encode a type with two's complement.
    Return_Type eval_3 = static_cast<
            make_unsigned<Encoding_Type>::type
        >(
            encoded_value & low_bits_mask<Return_Type, Width - 1>::value
        )
        ^
        (static_cast<
            make_unsigned<Encoding_Type>::type
        >(
            encoded_value & bits_mask<
                                Encoding_Type, 
                                bit_width<Encoding_Type>::value - 1 
                            >::value 
        ) >> (bit_width<Encoding_Type>::value - Width));

    BOOST_ASSERT(( eval_3 == 0xe));
    }

    // decoding a type inside of a Return_Type
    {
    typedef char        Return_Type;
    typedef std::size_t Decoding_From_Type;
    const std::size_t   Width = 4;

    std::size_t storage = 0xe;


    // get sign bit.
    Return_Type eval_4 = static_cast<Return_Type>(storage & bits_mask<Decoding_From_Type,Width - 1>::value);
    BOOST_ASSERT(( eval_4 == 0x8 ));

    Return_Type eval_5 = static_cast<Return_Type>(storage & bits_mask<Decoding_From_Type,Width - 1>::value) << (bit_width<Return_Type>::value - Width);
    BOOST_ASSERT(( eval_5 == static_cast<Return_Type>(0x80)));
    
    // getting value bits
    Return_Type eval_6 = static_cast<Return_Type>(storage & low_bits_mask<Decoding_From_Type,Width-1>::value );
    BOOST_ASSERT(( eval_6 == bits_mask<Return_Type,1,2>::value));
    }
    {
        std::size_t storage = 0;
        char temp = -2;
        storage = twos_complement_encoding< char, 4, unsigned int >( temp );
        BOOST_ASSERT((storage == 0xe));

        temp = twos_complement_decoding< unsigned int, 4, char >( storage );
        cout << "Value of Temp == "<< (int)temp <<endl;
        BOOST_ASSERT((temp == -2));
    }
    return 0;
}

