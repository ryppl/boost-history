// Boost coding/md5_digest_core.hpp header file  -----------------------------//
/** \file
    \brief  Definition of MD5 message-digest (core).

    \author  Daryle Walker

    Contains the declaration of the type representing MD5 message digests and
    the definitions of basic operations.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_MD5_DIGEST_CORE_HPP
#define BOOST_CODING_MD5_DIGEST_CORE_HPP

#include <boost/coding_fwd.hpp>

#include <boost/integer.hpp>     // for boost::sized_integral
#include <boost/mpl/int.hpp>     // for boost::mpl::int_
#include <boost/mpl/size_t.hpp>  // for boost::mpl::size_t

#include <algorithm>  // for std::equal
#include <iosfwd>     // for std::basic_istream, basic_ostream (declarations)


namespace boost
{
namespace coding
{


//  Forward declarations  ----------------------------------------------------//

// I/O streaming operator functions
template < typename Ch, class Tr >
  std::basic_istream<Ch, Tr> &  operator >>( std::basic_istream<Ch, Tr> &i,
  md5_digest &n );
template < typename Ch, class Tr >
  std::basic_ostream<Ch, Tr> &  operator <<( std::basic_ostream<Ch, Tr> &o,
  md5_digest const &n );


//  MD5 message-digest class declaration  ------------------------------------//

/** \brief  A class for storing a MD5 message digest.

    This type is as basic as possible, meant to be the return type for MD5
    hashing operations.  It is supposed to mirror the buffer described in RFC
    1321, sections 3.3&ndash;3.5.  Comparisons are supported for check-summing
    purposes, but not ordering.  Persistence is supported through the standard
    text stream I/O system.

    \see  boost::coding::md5_context
    \see  boost::coding::md5_computer
    \see  boost::coding::compute_md5(void const*,std::size_t)
 */
struct md5_digest
{
    // Types
    /** \brief  Number of bits for word-sized quantities

        Represents the number of bits per word as given in RFC 1321, section 2.
     */
    typedef mpl::int_<32>                                     bits_per_word;
    /** \brief  Type of MD register

        Represents the type of each register of the MD buffer.
     */
    typedef sized_integral<bits_per_word::value, unsigned>::type  word_type;
    /** \brief  Length of MD buffer

        Represents the number of registers in a MD buffer.
     */
    typedef mpl::size_t<4u>                                words_per_digest;

    // Member data
    /** \brief  The MD5 message digest checksum

        Represents the checksum from a MD5 hashing, mirroring for format of the
        MD buffer (see RFC 1321, section 3.3).  The zero-index corresponds to
        the "A" register, up to index 3 representing the "D" register.
     */
    word_type  hash[ words_per_digest::value ];

};  // md5_digest


//  MD5 message-digest structure equality operator function definitions  -----//

/** \brief  Equals

    Compares MD5 message digests for equivalence.  Such digests are equal if all
    of the corresponding parts of their hashes are equal.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l and \p r are equivalent.
    \retval false  \p l and \p r are not equivalent.

    \relates  boost::coding::md5_digest
 */
inline
bool
operator ==( md5_digest const &l, md5_digest const &r )
{
    return std::equal( l.hash, l.hash + md5_digest::words_per_digest::value,
     r.hash );
}

/** \brief  Not-equals

    Compares MD5 message digests for non-equivalence.  Such digests are unequal
    if at least one set of corresponding parts in their hashes are unequal.

    \param l  The left-side operand to be compared.
    \param r  The right-side operand to be compared.

    \retval true   \p l and \p r are not equivalent.
    \retval false  \p l and \p r are equivalent.

    \see  boost::coding::operator==(md5_digest const&,md5_digest const&)

    \relates  boost::coding::md5_digest
 */
inline
bool
operator !=( md5_digest const &l, md5_digest const &r )
{
    return !( l == r );
}


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_MD5_DIGEST_CORE_HPP
