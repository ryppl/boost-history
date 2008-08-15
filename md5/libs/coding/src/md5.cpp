// Boost md5.cpp implementation file  ----------------------------------------//
/** \file
    \brief  Definitions of MD5 computation components.

    \author  Daryle Walker

    Contains the definitions of constants and functions used for computing MD5
    message digests of given data blocks and granting I/O capability to any
    applicable types.  Non-inline items from
    &lt;boost/coding/md5_computer.hpp&gt; and
    &lt;boost/coding/md5_context.hpp&gt; are defined here.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#include <boost/coding/md5_context.hpp>
#include <boost/coding/md5_computer.hpp>

#include <boost/array.hpp>                  // for boost::array
#include <boost/assert.hpp>                 // for BOOST_ASSERT
#include <boost/bimap.hpp>                  // for boost::bimap
#include <boost/integer/integer_mask.hpp>   // for boost::integer_lo_mask
#include <boost/limits.hpp>                 // for std::numeric_limits
#include <boost/math/common_factor_rt.hpp>  // for boost::math::gcd
#include <boost/static_assert.hpp>          // for BOOST_STATIC_ASSERT

#include <algorithm>  // for std::copy, fill
#include <climits>    // for CHAR_BIT
#include <cmath>      // for std::sin, abs, ldexp, modf
#include <cstddef>    // for std::size_t
#include <cstdlib>    // for std::div_t, div
#include <numeric>    // for std::inner_product, partial_sum
#include <string>     // for std::string
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
 boost::integer_lo_mask<TotalRotated>::value;

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

// Convert an array, which maps an index to a value, to a bimap so the index can
// be found given a value.  This assumes that each array element is unique.
template < typename T, std::size_t N >
boost::bimap<std::size_t, T>  array_to_bimap( T const (&array)[N] )
{
    typedef boost::bimap<std::size_t, T>     result_type;
    typedef typename result_type::value_type  value_type;

    result_type  result;

    for ( std::size_t  i = 0u ; i < N ; ++i )
        result.insert( value_type(i, array[ i ]) );
    BOOST_ASSERT( result.size() == N );
    return result;
}

// This is the "base64url" encoding; see RFC 4648, section 5
char const  base64url_str[] =
 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
char const  (&base64url)[64] = *reinterpret_cast<char const (*)[64]>(
 &base64url_str );
char const  base64url_pad = '=';

// Now make that encoding searchable both ways
typedef boost::bimap<std::size_t, char>  base64url_map_t;

base64url_map_t const  base64url_map = array_to_bimap( base64url );

}  // unnamed namespace


namespace boost
{
namespace coding
{


//  MD5 message-digest core computation non-inline member definitions  -------//

/** Compares computation contexts for equivalence.  Such contexts are equal if
    their internal states are equal.  (This means that they should both return
    the same checksum, and continue to do so as long as the same bit sequence is
    submitted to both contexts.)

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are equivalent.
    \retval false  \c *this and \p o are not equivalent.
 */
bool  md5_context::operator ==( self_type const &o ) const
{
    queue_type const  tq = this->expand_queue(), oq = o.expand_queue();

    return ( this->length == o.length ) && ( this->buffer == o.buffer ) &&
     std::equal( tq.begin(), tq.begin() + this->length % bits_per_block::value,
     oq.begin() );
}

/** Provides the computed check-sum of all the submitted bits (as if the message
    is complete), through a standard generator interface.

    \return  The check-sum.
 */
md5_context::product_type  md5_context::operator ()() const
{
    self_type  copy( *this );
    copy.finish();

    product_type  result;
    std::copy( copy.buffer.begin(), copy.buffer.end(), result.hash );
    std::fill( copy.buffer.begin(), copy.buffer.end(), 0u );
    // The above line should keep anyone from peeking at a complete MD5 (i.e.
    // "copy") with a debugger or similar.  (The idea is from Kevin Sopp's 2007
    // MD5 code.)  Unfortunately, this cannot prevent spying via "result" if
    // either the return value isn't used or (named) return-value optimization
    // isn't applied toward the final destination object, since I can't zero-out
    // the (intermediate) temporary object(s).
    return result;
}

// Sample of the table described in RFC 1321, section 3.4, paragraph 4.  Its
// values are taken directly from the "MD5Transform" function in the RFC's
// section A.3, and are not computed.  Of course, the index is zero-based (C++)
// instead of one-based (RFC).
md5_context::hash_table_type const   md5_context::hashing_table = { {
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
md5_context::buffer_type const  md5_context::initial_buffer = { {0x67452301ul,
 0xEFCDAB89ul, 0x98BADCFEul, 0x10325476ul} };

// Input a single bit.
void  md5_context::consume_bit( bool bit )
{
    int const            index = this->length++ % bits_per_block::value;
    std::div_t const     sub_indices = std::div( index, CHAR_BIT );
    unsigned char &      byte = this->queue[ sub_indices.quot ];
    unsigned char const  mask = 1u << ( CHAR_BIT - 1 - sub_indices.rem );

    // Since moving representation of the Boolean queue from an regular "bool"
    // array to a packed "unsigned char" array, there has been the question of
    // how the packed bits are arranged in each byte.  We have the next read bit
    // go into the highest-order spot that isn't already used.  This is manually
    // kept in sync with "expand_queue", "contract_queue", and
    // "md5_computer::copy_unbuffered".  The high-order-first order is also how
    // the bit-oriented MD5 algorithm reads a byte.  There is a bonus that an
    // optimized byte-consumption routine can copy a byte directly into the
    // array for quick entry, at least if CHAR_BIT divides index.

    if ( bit )  byte |=  mask;
    else        byte &= ~mask;
    if ( index + 1 == bits_per_block::value )  this->update_hash();
}

// Hash an entire block into the running checksum, using RFC 1321, section 3.4
void  md5_context::update_hash()
{
    using std::size_t;

    // Convert the queued bit block to a word block
    std::valarray<word_ftype>  words( words_per_block::value ), scratch(words);
    queue_type const           q = this->expand_queue();

    for ( size_t  i = 0u ; i < words_per_block::value ; ++i )
    {
        // Use the default inner-product; since "queue" has "bool" elements,
        // which convert to 0 or 1, multiplication acts as AND; since
        // "order_in_word" has distinct single-bit values, addition acts as OR.
        words[ i ] = std::inner_product( order_in_word.begin(),
         order_in_word.end(), q.begin() + i * bits_per_word::value,
         word_ftype(0u) );
    }

    // Set up rounds
    buffer_type  buffer = this->buffer;

    // Round 1
    {
        md5_special_op<md5_f, word_ftype, bits_per_word::value>  ff;

        scratch = words[ skipped_indices(words_per_block::value, 0, 1) ];
        for ( size_t  i = 0u ; i < words_per_block::value ; ++i )
        {
            ff( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[0][i % 4u], hashing_table[i] );
        }
    }

    // Round 2
    {
        md5_special_op<md5_g, word_ftype, bits_per_word::value>  gg;

        scratch = words[ skipped_indices(words_per_block::value, 1, 5) ];
        for ( size_t  i = 0u ; i < words_per_block::value ; ++i )
        {
            gg( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[1][i % 4u], hashing_table[16 + i] );
        }
    }

    // Round 3
    {
        md5_special_op<md5_h, word_ftype, bits_per_word::value>  hh;

        scratch = words[ skipped_indices(words_per_block::value, 5, 3) ];
        for ( size_t  i = 0u ; i < words_per_block::value ; ++i )
        {
            hh( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[2][i % 4u], hashing_table[32 + i] );
        }
    }

    // Round 4
    {
        md5_special_op<md5_i, word_ftype, bits_per_word::value>  ii;

        scratch = words[ skipped_indices(words_per_block::value, 0, 7) ];
        for ( size_t  i = 0u ; i < words_per_block::value ; ++i )
        {
            ii( buffer[( 16u - i ) % 4u], buffer[( 17u - i ) % 4u],
             buffer[( 18u - i ) % 4u], buffer[( 19u - i ) % 4u], scratch[i],
             md5_s[3][i % 4u], hashing_table[48 + i] );
        }
    }

    // Update buffer
    for ( size_t  i = 0u ; i < words_per_digest::value ; ++i )
    {
        this->buffer[ i ] += buffer[ i ];
        this->buffer[ i ] &= integer_lo_mask<bits_per_word::value>::value;
    }
}

// Apply the finishing procedure by submitting padding bits then the
// (pre-padding) length.  The padding + length should exactly turn over the
// queue.
void  md5_context::finish()
{
    // Save the current length before we mutate it.
    length_ftype const  original_length = this->length;

    // Enter a One, then enough Zeros so the length would fill the queue.
    this->consume_bit( true );
    for ( int  i = bits_per_block::value - (this->length +
     bits_per_length::value) % bits_per_block::value ; i > 0 ; --i )
        this->consume_bit( false );

    this->consume_dword( original_length );
    BOOST_ASSERT( !(this->length % bits_per_block::value) );

    // Now a finished checksum in this->buffer is ready to read.
}

// Copy the stored packed array to a regular Boolean array
md5_context::queue_type  md5_context::expand_queue() const
{
    int         count = bits_per_block::value;
    queue_type  result;
    bool *      b = result.begin();

    for ( unsigned char const *  p = this->queue.begin() ; count > 0 ; ++p )
        for ( unsigned char  m = 1u << (CHAR_BIT - 1) ; m && count-- ; m >>= 1 )
            *b++ = *p & m;
    return result;
}

// Copy a regular Boolean array into the packed array
void  md5_context::contract_queue( queue_type const &bits )
{
    int       count = bits_per_block::value;
    bool const *  b = bits.begin();

    for ( unsigned char *  p = this->queue.begin() ; count > 0 ; ++p )
        for ( unsigned char  m = 1u << (CHAR_BIT - 1) ; m && count-- ; m >>= 1 )
            if ( *b++ )
                *p |= m;
            else
                *p &= ~m;
}

// Convert the current queue to a string to serialize
std::string  md5_context::queue_to_string() const
{
    using std::size_t;

    size_t const      unbuffered = this->length % bits_per_block::value,
                      bits_per_sextet = 6,
                      full_sextets = unbuffered / bits_per_sextet,
                      leftover_bits = unbuffered % bits_per_sextet,
                      partial_sextet = ( leftover_bits != 0u );
    queue_type const  q = this->expand_queue();
    bool const *      i = q.begin();
    size_t            index;
    std::string       result;

    result.reserve( full_sextets + partial_sextet );

    for ( size_t  j = 0u ; j < full_sextets ; ++j )
    {
        // leading Booleans get high-order spots
        index = 0u;
        for ( size_t  k = 0u ; k < bits_per_sextet ; ++k )
        {
            index <<= 1;
            if ( *i++ )  index |= 1u;
        }
        result.push_back( base64url[index] );
    }

    if ( partial_sextet )
    {
        index = 0u;
        for ( size_t  l = 0u ; l < leftover_bits ; ++l )
        {
            index <<= 1;
            if ( *i++ )  index |= 1u;
        }

        // Fill in remaining spots of this sextet
        index <<= bits_per_sextet - leftover_bits;
        result.push_back( base64url[index] );
    }

    return result;
}

// Convert an unserialized string back to a queue state
void  md5_context::string_to_queue( std::string const &s )
{
    using std::size_t;

    size_t const  bits_per_sextet = 6;
    queue_type    q = { {false} };
    bool *        i = q.begin();
    int           limit = bits_per_block::value;  // resolves partial sextets

    for ( std::string::const_iterator  si = s.begin(), se = s.end() ; (si != se)
     && (limit > 0) ; ++si )
    {
        // Skip over any illegal characters
        base64url_map_t::right_const_iterator const  p_index =
         base64url_map.right.find( *si );

        if ( p_index == base64url_map.right.end() )  continue;

        // Insert the bits expanded from the index 
        size_t const  mask = 1u << ( bits_per_sextet - 1u );

        for ( size_t  j = 0u, index = p_index->second ; (j < bits_per_sextet) &&
         (limit-- > 0) ; ++j, index <<= 1 )
            *i++ = index & mask;
    }
    this->contract_queue( q );
}


//  MD5 message-digest computation non-inline member definitions  ------------//

/** Sample of the table described in RFC 1321, section 3.4, paragraph 4.  Its
    values are taken directly from the "MD5Transform" function in the RFC's
    section A.3, and are not computed.  Of course, the index is zero-based (C++)
    instead of one-based (RFC).

    \see  #generate_hashing_table
 */
md5_computer::hash_table_type const  md5_computer::hashing_table =
 md5_context::hashing_table;

/** Constructs the hashing sine table based on the directions given in RFC 1321,
    section 3.4, paragraph 4.  It should give the same values as
    \c #hashing_table, but it is dependent on the quality of the platform's math
    library, thereby testing that environment.

    \return  The computed hashing sine table

    \see  #hashing_table
 */
md5_computer::hash_table_type
md5_computer::generate_hashing_table()
{
    hash_table_type  result;

    for ( int  i = 0 ; i < hash_table_type::static_size ; ++i )
    {
        double  x = std::ldexp( std::abs(std::sin( static_cast<double>(i + 1)
         )), +32 );  // 2**32 * abs(sin(I)), where I = i + 1

        std::modf( x, &x );  // x -> x rounded towards zero
        result[ i ] = static_cast<unsigned long>( x );
    }
    return result;
}


}  // namespace coding
}  // namespace boost
