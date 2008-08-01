// Boost md5.cpp implementation file  ----------------------------------------//
/** \file
    \brief  Definitions of MD5 computation components.

    \author  Daryle Walker

    Contains the definitions of constants and functions used for computing MD5
    message digests of given data blocks and granting I/O capability to any
    applicable types.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#include <boost/coding/md5.hpp>

#include <boost/array.hpp>                  // for boost::array
#include <boost/assert.hpp>                 // for BOOST_ASSERT
#include <boost/integer/integer_mask.hpp>   // for boost::low_bits_mask_t
#include <boost/math/common_factor_rt.hpp>  // for boost::math::gcd
#include <boost/static_assert.hpp>          // for BOOST_STATIC_ASSERT

#include <algorithm>  // for std::copy
#include <cmath>      // for std::sin, abs, ldexp, modf
#include <cstddef>    // for std::size_t
#include <limits>     // for std::numeric_limits
#include <numeric>    // for std::inner_product, partial_sum
#include <valarray>   // for std::valarray, etc.


//  Custom types/functions/templates  ----------------------------------------//

// Put custom types/templates and helper functions here
namespace
{

// Auxillary functions from RFC 1321, section 3.4, paragraphs 1-3
template < typename BitwiseType >
class md5_f
{
public:
    typedef BitwiseType  result_type, first_argument_type, second_argument_type,
     third_argument_type;

    result_type  operator ()( first_argument_type x, second_argument_type y,
     third_argument_type z ) const
    {
        // First MD5 auxillary function F: selection (if x, then y, else z)
        return ( x & y ) | ( ~x & z );
    }
};

template < typename BitwiseType >
class md5_g
{
public:
    typedef BitwiseType  result_type, first_argument_type, second_argument_type,
     third_argument_type;

    result_type  operator ()( first_argument_type x, second_argument_type y,
     third_argument_type z ) const
    {
        // Second MD5 auxillary function G: selection (if z, then x, else y)
        return ( x & z ) | ( y & ~z );
    }
};

template < typename BitwiseType >
class md5_h
{
public:
    typedef BitwiseType  result_type, first_argument_type, second_argument_type,
     third_argument_type;

    result_type  operator ()( first_argument_type x, second_argument_type y,
     third_argument_type z ) const
    {
        // Third MD5 auxillary function H: parity (# of TRUE values is odd)
        return x ^ y ^ z;
    }
};

template < typename BitwiseType >
class md5_i
{
public:
    typedef BitwiseType  result_type, first_argument_type, second_argument_type,
     third_argument_type;

    result_type  operator ()( first_argument_type x, second_argument_type y,
     third_argument_type z ) const
    {
        // Fourth MD5 auxillary function I: ? (???)
        return y ^ ( x | ~z );
    }
};

// Transfoming rotation constants
int const  md5_s[4][4] = { {7, 12, 17, 22}, {5, 9, 14, 20}, {4, 11, 16, 23}, {6,
 10, 15, 21} };

// Rotating left-shift for bits
template < typename Unsigned, int TotalRotated >
class left_rotator
{
    typedef std::numeric_limits<Unsigned>  limits_type;

    BOOST_STATIC_ASSERT( limits_type::is_specialized && limits_type::is_integer
     && (limits_type::radix == 2) && limits_type::is_bounded &&
     !limits_type::is_signed && (limits_type::digits >= TotalRotated) );

    static  Unsigned const  mask_;

public:
    typedef Unsigned  result_type, first_argument_type;
    typedef int       second_argument_type;

    result_type  operator ()( first_argument_type x, second_argument_type n )
     const
    {
        return ( (x << n) | (( x & mask_ ) >> ( TotalRotated - n )) ) & mask_;
    }
};

template < typename Unsigned, int TotalRotated >
Unsigned const  left_rotator<Unsigned, TotalRotated>::mask_ =
 boost::low_bits_mask_t<TotalRotated>::sig_bits;

// Order of listed bits within a 32-bit word: octets are listed lowest-order
// first, but the bits within octets are listed highest-order first!  The order
// is given in the appropriate power-of-two for that bit.
boost::array<unsigned long, 32> const  order_in_word = { {1ul << 7, 1ul << 6,
 1ul << 5, 1ul << 4, 1ul << 3, 1ul << 2, 1ul << 1, 1ul << 0, 1ul << 15, 1ul <<
 14, 1ul << 13, 1ul << 12, 1ul << 11, 1ul << 10, 1ul << 9, 1ul << 8, 1ul << 23,
 1ul << 22, 1ul << 21, 1ul << 20, 1ul << 19, 1ul << 18, 1ul << 17, 1ul << 16,
 1ul << 31, 1ul << 30, 1ul << 29, 1ul << 28, 1ul << 27, 1ul << 26, 1ul << 25,
 1ul << 24} };

// The special operation repeatedly used for hashing
template < template <typename> class TernaryFuncTmpl, typename Word, int
 WordLength >
class md5_special_op
{
    TernaryFuncTmpl<Word>           f_;
    left_rotator<Word, WordLength>  lr_;

public:
    void  operator ()(Word &a, Word b, Word c, Word d, Word xk, int s, Word ti)
    {
        a = b + this->lr_( a + this->f_(b, c, d) + xk + ti, s );
    }
};

// Fill valarray with spaced indices
std::valarray<std::size_t>
skipped_indices( std::size_t total_size, std::size_t first_index, std::size_t
 spacing )
{
    BOOST_ASSERT( (total_size > 0u) && (first_index < total_size) &&
     boost::math::gcd(spacing, total_size) == 1u );

    std::valarray<std::size_t>  r( spacing, total_size );

    r[ 0 ] = first_index;
    std::partial_sum( &r[0], &r[0] + total_size, &r[0] );
    r %= total_size;

    return r;
}

// The exact amount of bits needed to be in the queue so an appendage of the
// length will exactly fill the queue.
std::size_t const  padding_remainder =
 boost::coding::md5_computerX::bits_per_block -
 boost::coding::md5_computerX::significant_bits_per_length;

}  // unnamed namespace


namespace boost
{
namespace coding
{


//  MD5 message-digest class-static member definitions  ----------------------//

/** Represents the number of bits per word as given in RFC 1321, section 2.
 */
int const          md5_digest::bits_per_word;
/** Represents the number of registers in a MD buffer.
 */
std::size_t const  md5_digest::words_per_digest;


//! \cond
//  Implementation detail object definitions  --------------------------------//

namespace detail
{

// Nybbles and hexadecimal digits
std::size_t const  md5_constants::nybbles_per_hexadecimal_digit;
std::size_t const  md5_constants::bits_per_nybble;

std::size_t const  md5_constants::number_of_hexadecimal_digits;

char const  md5_constants::hex_digits_lc[] = "0123456789abcdef";
char const  md5_constants::hex_digits_uc[] = "0123456789ABCDEF";

// MD words
std::size_t const  md5_constants::nybbles_per_word;

// MD strings
std::size_t const  md5_constants::characters_per_digest;

}  // namespace detail
//! \endcond


//  MD5 message-digest computer class-static member definitions  -------------//

std::size_t const  md5_computerX::words_per_block;

/** Represents the number of significant (low-order) bits kept for the
    message length, which can also be processed as two words, all as given
    in RFC 1321, section 3.2, paragraph 1.
 */
int const          md5_computerX::significant_bits_per_length;
/** Represents the number of submitted bits that are queued until that queue
    is emptied as a single block to be hashed, implied from RFC 1321,
    section 3.4.  The count of unhashed bits is always less than this value.
    (The input processing member functions trigger a hash right after a bit
    fills the queue.)
 */
std::size_t const  md5_computerX::bits_per_block;

/** Sample of the table described in RFC 1321, section 3.4, paragraph 4.
    Its values are taken directly from the "MD5Transform" function in the
    RFC's section A.3, and are not computed.  Of course, the index is
    zero-based (C++) instead of one-based (RFC).

    \see  #generate_hashing_table
 */
array<md5_digest::word_type, 64> const  md5_computerX::hashing_table = { {
    0xD76AA478ul,
    0xE8C7B756ul,
    0x242070DBul,
    0xC1BDCEEEul,
    0xF57C0FAFul,
    0x4787C62Aul,
    0xA8304613ul,
    0xFD469501ul,
    0x698098D8ul,
    0x8B44F7AFul,
    0xFFFF5BB1ul,
    0x895CD7BEul,
    0x6B901122ul,
    0xFD987193ul,
    0xA679438Eul,
    0x49B40821ul,
    0xF61E2562ul,
    0xC040B340ul,
    0x265E5A51ul,
    0xE9B6C7AAul,
    0xD62F105Dul,
     0x2441453ul,
    0xD8A1E681ul,
    0xE7D3FBC8ul,
    0x21E1CDE6ul,
    0xC33707D6ul,
    0xF4D50D87ul,
    0x455A14EDul,
    0xA9E3E905ul,
    0xFCEFA3F8ul,
    0x676F02D9ul,
    0x8D2A4C8Aul,
    0xFFFA3942ul,
    0x8771F681ul,
    0x6D9D6122ul,
    0xFDE5380Cul,
    0xA4BEEA44ul,
    0x4BDECFA9ul,
    0xF6BB4B60ul,
    0xBEBFBC70ul,
    0x289B7EC6ul,
    0xEAA127FAul,
    0xD4EF3085ul,
     0x4881D05ul,
    0xD9D4D039ul,
    0xE6DB99E5ul,
    0x1FA27CF8ul,
    0xC4AC5665ul,
    0xF4292244ul,
    0x432AFF97ul,
    0xAB9423A7ul,
    0xFC93A039ul,
    0x655B59C3ul,
    0x8F0CCC92ul,
    0xFFEFF47Dul,
    0x85845DD1ul,
    0x6FA87E4Ful,
    0xFE2CE6E0ul,
    0xA3014314ul,
    0x4E0811A1ul,
    0xF7537E82ul,
    0xBD3AF235ul,
    0x2AD7D2BBul,
    0xEB86D391ul
} };

// Initial values of the MD buffer, taken from RFC 1321, section 3.3.  (Note
// that the RFC lists each number low-order byte first, while numbers need to be
// written high-order byte first in C++.)
md5_computerX::ibuffer_type const  md5_computerX::initial_buffer_ = {
 {0x67452301ul, 0xEFCDAB89ul, 0x98BADCFEul, 0x10325476ul} };


//  MD5 message-digest computer class-static member function definitions  ----//

/** Constructs the hashing sine table based on the directions given in RFC
    1321, section 3.4, paragraph 4.  It should give the same values as
    #hashing_table, but it's dependent on the quality of the environment's
    math library, thereby giving a test of the environment.

    \return  The computed hashing sine table

    \see  #hashing_table
 */
array<md5_digest::word_type, 64>
md5_computerX::generate_hashing_table()
{
    array<md5_digest::word_type, 64>  r;

    for ( int  i = 0 ; i < 64 ; ++i )
    {
        double  x = std::ldexp( std::abs(std::sin( static_cast<double>(i + 1)
         )), +32 );  // 2**32 * abs(sin(I)), where I = i + 1

        std::modf( x, &x );  // x -> x rounded towards zero
        r[ i ] = static_cast<unsigned long>( x );
    }
    return r;
}


//  MD5 message-digest computer implementation member function definitions  --//

// Hash an entire block into the running checksum, using RFC 1321, section 3.4
void
md5_computerX::update_hash( bool const *queue_b, bool const *queue_e )
{
    using std::size_t;

    // Convert the queued bit block to a word block
    std::valarray<iword_type>  words( words_per_block ), scratch( queue_e -
     queue_b );

    BOOST_ASSERT( scratch.size() == bits_per_block );
    std::copy( queue_b, queue_e, &scratch[0] );
    for ( size_t  i = 0u ; i < words_per_block ; ++i )
    {
        // Use the default inner-product; since "unbuffered_" has "bool"
        // elements, which convert to 0 or 1, multiplication acts as AND; since
        // "order_in_word" has distinct single-bit values, addition acts as OR
        words[ i ] = std::inner_product( order_in_word.begin(),
         order_in_word.end(), &scratch[ i * md5_digest::bits_per_word ],
         iword_type(0u) );
    }

    // Set up rounds
    ibuffer_type  buffer = this->buffer_;

    scratch.resize( words.size() );  // repurposed!

    // Round 1
    {
        md5_special_op<md5_f, iword_type, md5_digest::bits_per_word>  ff;

        scratch = words[ skipped_indices(words_per_block, 0, 1) ];
        for ( size_t  i = 0u ; i < words_per_block ; ++i )
        {
            ff( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[0][i % 4u], hashing_table[i] );
        }
    }

    // Round 2
    {
        md5_special_op<md5_g, iword_type, md5_digest::bits_per_word>  gg;

        scratch = words[ skipped_indices(words_per_block, 1, 5) ];
        for ( size_t  i = 0u ; i < words_per_block ; ++i )
        {
            gg( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[1][i % 4u], hashing_table[16 + i] );
        }
    }

    // Round 3
    {
        md5_special_op<md5_h, iword_type, md5_digest::bits_per_word>  hh;

        scratch = words[ skipped_indices(words_per_block, 5, 3) ];
        for ( size_t  i = 0u ; i < words_per_block ; ++i )
        {
            hh( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[2][i % 4u], hashing_table[32 + i] );
        }
    }

    // Round 4
    {
        md5_special_op<md5_i, iword_type, md5_digest::bits_per_word>  ii;

        scratch = words[ skipped_indices(words_per_block, 0, 7) ];
        for ( size_t  i = 0u ; i < words_per_block ; ++i )
        {
            ii( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[3][i % 4u], hashing_table[48 + i] );
        }
    }

    // Update buffer
    for ( size_t  i = 0u ; i < md5_digest::words_per_digest ; ++i )
    {
        this->buffer_[ i ] += buffer[ i ];
        this->buffer_[ i ] &= low_bits_mask_t< md5_digest :: bits_per_word > ::
         sig_bits;
    }
}


//  MD5 message-digest computation digest-output member function definition  -//

/** Provides the computed check-sum of all the submitted bits.  (The queued
    bits are temporarily hashed with a special finishing procedure.)

    \return  The check-sum (i.e. message digest).
 */
md5_computerX::value_type
md5_computerX::checksum() const
{
    // As explained in RFC 1321, section 3, the final check-sum is the state of
    // the hash after padding and the original length are appended to the
    // message data.  (The padding is sized such that the length's bits always
    // exactly finishes a block.)  The padding is one mandatory TRUE value
    // followed by the appropriate amount of FALSE values.
    //
    // Technically, the modulo-bits_per_block is needed only if bits_unbuffered
    // exceeds padding_remainder (when it doesn't, just do the subtraction
    // directly), but this way always works and a conditional is avoided.  (The
    // corresponding test added cases to make sure the old conditional was hit.)
    self_type  cache( *this );

    cache.process_bit( true );
    cache.process_bit_copies( false, (padding_remainder +
     self_type::bits_per_block - cache.bits_unbuffered()) %
     self_type::bits_per_block );
    cache.process_double_word( this->bits_read() );
    BOOST_ASSERT( cache.bits_unbuffered() == 0u );

    self_type::buffer_type const  rb = cache.last_buffer();
    self_type::value_type         r;

    std::copy( rb.begin(), rb.end(), r.hash );
    return r;
}

int const  md5_computer::significant_bits_per_length;
std::size_t const  md5_computer::bits_per_block;
array<md5_digest::word_type, 64> const  md5_computer::hashing_table =
 md5_computerX::hashing_table;


}  // namespace coding
}  // namespace boost
