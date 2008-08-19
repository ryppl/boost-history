// Boost coding/md5_digest.hpp header file  ----------------------------------//
/** \file
    \brief  Definition of MD5 message-digest (main, I/O).

    \author  Daryle Walker

    Extends the declaration of MD5 message digests with the definitions of the
    streaming-I/O operations.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_MD5_DIGEST_HPP
#define BOOST_CODING_MD5_DIGEST_HPP

#include <boost/coding_fwd.hpp>
#include <boost/coding/md5_digest_core.hpp>  // for boost::coding::md5_digest

#include <boost/config.hpp>             // for BOOST_USE_FACET
#include <boost/mpl/assert.hpp>         // for BOOST_MPL_ASSERT_RELATION
#include <boost/mpl/arithmetic.hpp>     // for boost::mpl::divides,modulus,times
#include <boost/mpl/size_t.hpp>         // for boost::mpl::size_t
#include <boost/serialization/nvp.hpp>  // for boost::serialization::make_nvp
#include <boost/typeof/typeof.hpp>      // for BOOST_AUTO

#include <cstdlib>   // for std::div, div_t
#include <ios>       // for std::ios_base
#include <istream>   // for std::basic_istream
#include <iterator>  // for std::istreambuf_iterator
#include <locale>    // for std::use_facet, ctype
#include <ostream>   // for std::basic_ostream


namespace boost
{
namespace coding
{


//! \cond
//  Implementation details  --------------------------------------------------//

namespace detail
{

// MD5 message digest constants, especially for I/O
struct md5_constants
{
    // Nybbles and hexadecimal digits
    typedef mpl::size_t<4u>  bits_per_nybble;
    typedef mpl::size_t<1u>  nybbles_per_digit16, chars_per_digit16;

    // MD words
    typedef md5_digest::bits_per_word  bits_per_word;

    typedef mpl::divides<bits_per_word, bits_per_nybble>  nybbles_per_word;

    // MD strings
    typedef md5_digest::words_per_digest                      words_per_digest;
    typedef mpl::times<bits_per_word, words_per_digest>        bits_per_digest;
    typedef mpl::times<bits_per_nybble, nybbles_per_digit16>   bits_per_digit16;
    typedef mpl::divides<bits_per_digit16, chars_per_digit16>  bits_per_char;

    typedef mpl::divides<bits_per_digest, bits_per_char>  chars_per_digest;

    // Make sure those divisions really are even
    struct leftover_checks
    {
        typedef mpl::modulus<bits_per_word, bits_per_nybble>     leftover_bits1;
        typedef mpl::modulus<bits_per_digit16,chars_per_digit16> leftover_bits2;
        typedef mpl::modulus<bits_per_digest, bits_per_char>     leftover_bits3;

        BOOST_MPL_ASSERT_RELATION( leftover_bits1::value, ==, 0 );
        BOOST_MPL_ASSERT_RELATION( leftover_bits2::value, ==, 0 );
        BOOST_MPL_ASSERT_RELATION( leftover_bits3::value, ==, 0 );
    };

    // Hexadecimal digit identities
    typedef mpl::size_t<16u>  number_of_digit16_digits;  // duh!

    typedef char  digits16_str[ number_of_digit16_digits::value + 1u ];

    static  digits16_str const &  digit16_list( bool use_uppercase )
    {
        static  char const  digits16_lc[] = "0123456789abcdef";
        static  char const  digits16_uc[] = "0123456789ABCDEF";

        BOOST_MPL_ASSERT_RELATION( sizeof digits16_lc, ==, sizeof digits16_uc );
        BOOST_MPL_ASSERT_RELATION(sizeof digits16_lc, ==, sizeof(digits16_str));

        return use_uppercase ? digits16_uc : digits16_lc;
    }

};  // md5_constants

}  // namespace detail
//! \endcond


//  MD5 message-digest structure streaming operator function definitions  ----//

/** \brief  Reads a \c md5_digest from an input stream

    Receives a \c md5_digest object from an input stream.  The format is as
    given in RFC 1321, section 3.5, i.e. go from the least-significant octet of
    the first hash component to the most-significant octet of the last hash
    component.  The format for each octet read is two hexadecimal digits
    (0&ndash;9 and either a&ndash;f or A&ndash;F), with the digit for the
    most-significant nybble read first.  The letter-case of the higher-order
    hexadecimal digits does not matter.  (Note that exactly 32 characters are
    read, not counting how \c std::ios_base::skipws for \p i is set.)

    \param i  The input stream to perform the reading.
    \param d  The \c md5_digest object to store the read.

    \return  \p i

    \see  boost::coding::operator<<(std::basic_ostream<Ch,Tr>&,md5_digest const&)

    \relates  boost::coding::md5_digest
 */
template < typename Ch, class Tr >
std::basic_istream<Ch, Tr> &
operator >>( std::basic_istream<Ch, Tr> &i, md5_digest &d )
{
    typename std::basic_istream<Ch, Tr>::sentry  is( i );

    if ( is )
    {
        // Set up
        BOOST_AUTO( const &  f, BOOST_USE_FACET(std::ctype<Ch>, i.getloc()) );
        int         nybble_index = 0;
        md5_digest  temp = { {0} };

        // Read the exact number of characters
        for ( std::istreambuf_iterator<Ch, Tr>  ii(i), ie ; (ie != ii) &&
         (nybble_index < detail::md5_constants::chars_per_digest::value) ; ++ii,
         ++nybble_index )
        {
            // Read a character, which represents one nybble
            md5_digest::word_type  nybble = 0u;

            switch ( f.narrow(*ii, '\0') )
            {
            case 'F': case 'f':  ++nybble;  // FALL THROUGH
            case 'E': case 'e':  ++nybble;  // FALL THROUGH
            case 'D': case 'd':  ++nybble;  // FALL THROUGH
            case 'C': case 'c':  ++nybble;  // FALL THROUGH
            case 'B': case 'b':  ++nybble;  // FALL THROUGH
            case 'A': case 'a':  ++nybble;  // FALL THROUGH
            case '9':  ++nybble;  // FALL THROUGH
            case '8':  ++nybble;  // FALL THROUGH
            case '7':  ++nybble;  // FALL THROUGH
            case '6':  ++nybble;  // FALL THROUGH
            case '5':  ++nybble;  // FALL THROUGH
            case '4':  ++nybble;  // FALL THROUGH
            case '3':  ++nybble;  // FALL THROUGH
            case '2':  ++nybble;  // FALL THROUGH
            case '1':  ++nybble;  // FALL THROUGH
            case '0':  break;
            default:   goto abort_read;
            }

            // Place the nybble within its word.  (Octets are read lowest to
            // highest, but the nybbles are read in the reverse order, so swap
            // the positions of the high and low nybbles when putting them in
            // the appropriate octet.)
            std::div_t const  nybble_index_parts = std::div( nybble_index,
             detail::md5_constants::nybbles_per_word::value );

            temp.hash[ nybble_index_parts.quot ] |= nybble <<
             ( detail::md5_constants::bits_per_nybble::value * ((
             nybble_index_parts.rem ) ^ 0x01)
             );
        }

abort_read:
        // Finish up
        if ( nybble_index < detail::md5_constants::chars_per_digest::value )
        {
            // Incomplete read
            i.setstate( std::ios_base::failbit );
        }
        else
        {
            // Successful read
            d = temp;
        }
    }

    return i;
}

/** \brief  Writes a \c md5_digest to an output stream

    Sends a \c md5_digest object to an output stream.  The format is as given
    in RFC 1321, section 3.5, i.e. go from the least-significant octet of the
    first hash component to the most-significant octet of the last hash
    component.  The format for each octet written is two hexadecimal digits
    (0&ndash;9 and either a&ndash;f or A&ndash;F), with the digit for the
    most-significant nybble written first.  The setting of
    \c std::ios_base::uppercase in \p o affects which characters are used for
    the higher-order hexadecimal digits.  (Note that exactly 32 characters are
    written, not counting how <code><var>o</var>.width()</code> is set.)

    \param o  The output stream to perform the writing.
    \param d  The \c md5_digest object to be written.

    \return  \p o

    \see  boost::coding::operator>>(std::basic_istream<Ch,Tr>&,md5_digest&)

    \relates  boost::coding::md5_digest
 */
template < typename Ch, class Tr >
std::basic_ostream<Ch, Tr> &
operator <<( std::basic_ostream<Ch, Tr> &o, md5_digest const &d )
{
    // The message always has an exact number of characters; plot it out.
    // (Leave an extra character for the NUL terminator.)
    char    hex_string[ detail::md5_constants::chars_per_digest::value + 1u ];
    char *  p = hex_string;

    // Each nybble will be printed as a hexadecimal digit.
    detail::md5_constants::digits16_str const &  digits =
     detail::md5_constants::digit16_list(o.flags() & std::ios_base::uppercase);

    // Print each nybble.  Since the nybble progression within an octet is the
    // reverse of the octet and word progressions, stick in a reversal flag
    // while indexing.
    for ( int  nybble_index = 0u ; nybble_index <
     detail::md5_constants::chars_per_digest::value ; ++nybble_index )
    {
        std::div_t const  nybble_index_parts = std::div( nybble_index,
         detail::md5_constants::nybbles_per_word::value );

        *p++ = digits[ 0x0F & (d.hash[ nybble_index_parts.quot ] >> (
         detail::md5_constants::bits_per_nybble::value * ((
         nybble_index_parts.rem ) ^ 0x01) )) ];
    }
    *p = '\0';

    // Print the message, taking stream settings into account
    return o << hex_string;
}


}  // namespace coding

namespace serialization
{


//  MD5 message-digest structure serialization template function definition  -//

/** \brief  Enables persistence with Boost.Serialization-compatible archives for
            \c boost::coding::md5_digest, non-member

    Streams a message digest to/from an archive using the Boost.Serialization
    protocols.  This function is meant to be called only by the
    Boost.Serialization system, as needed.

    \tparam Archive  The type of \p ar.  It must conform to the requirements
                     Boost.Serialization expects of archiving classes.

    \param ar       The archiving object that this object's representation will
                     be streamed to/from.
    \param d        The \c md5_digest object to be serialized or deserialized.
    \param version  The version of the persistence format for this object.  (It
                    should be zero, since this type just got created.)

    \relates  boost::coding::md5_digest
 */
template < class Archive >
inline void
serialize( Archive &ar, coding::md5_digest &d, const unsigned int version )
{
    switch ( version )
    {
    default:
    case 0u:
        ar & make_nvp( "word-A", d.hash[0] )
           & make_nvp( "word-B", d.hash[1] )
           & make_nvp( "word-C", d.hash[2] )
           & make_nvp( "word-D", d.hash[3] );
        break;
    }
}


}  // namespace serialization
}  // namespace boost


#endif  // BOOST_CODING_MD5_DIGEST_HPP
