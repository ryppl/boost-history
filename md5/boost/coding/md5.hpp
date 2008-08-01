// Boost coding/md5.hpp header file  -----------------------------------------//
/** \file
    \brief  Declarations of MD5 computation components.

    \author  Daryle Walker

    Contains the declaration of types and functions used for computing MD5
    message digests of given data blocks and granting I/O capability to any
    applicable types.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_MD5_HPP
#define BOOST_CODING_MD5_HPP

#include <boost/coding_fwd.hpp>

#include <boost/array.hpp>                 // for boost::array
#include <boost/assert.hpp>                // for BOOST_ASSERT
#include <boost/coding/coding_shell.hpp>   // for b:c:bit_coding_shell
#include <boost/coding/operations.hpp>     // for b:c:queued_bit_processing_base
#include <boost/integer.hpp>               // for boost::sized_integral
#include <boost/serialization/access.hpp>  // for boost::serialization::access
#include <boost/static_assert.hpp>         // for BOOST_STATIC_ASSERT
#include <boost/typeof/typeof.hpp>         // for BOOST_AUTO

#include <algorithm>  // for std::equal, swap
#include <climits>    // for CHAR_BIT
#include <cstddef>    // for std::size_t
#include <ios>        // for std::ios_base
#include <istream>    // for std::basic_istream
#include <locale>     // for std::use_facet, ctype
#include <ostream>    // for std::basic_ostream


namespace boost
{
namespace coding
{


//  Forward declarations  ----------------------------------------------------//

// None right now


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
class md5_digest
{
public:
    //! Number of bits for word-sized quantities
    static  int const  bits_per_word = 32;

    /** \brief  Type of MD register

        Represents the type of each register of the MD buffer.
     */
    typedef boost::sized_integral<bits_per_word, unsigned>::type  word_type;

    //! Length of MD buffer
    static  std::size_t const  words_per_digest = 4u;

    /** \brief  The MD5 message digest checksum

        Represents the checksum from a MD5 hashing, mirroring for format of the
        MD buffer (see RFC 1321, section 3.3).  The zero-index corresponds to
        the "A" register, up to index 3 representing the "D" register.
     */
    word_type  hash[ words_per_digest ];

};  // md5_digest


//  MD5 message-digest computation class declaration  ------------------------//

/** \brief  A class for generating a MD5 message digest from submitted data.

    This class can accept data in several runs and produce a hash based on that
    data from the MD5 message-digest algorithm described in RFC 1321.  It should
    have a similar interface to Boost.CRC, plus specialized function object
    interfaces for bit- and byte-level processing (inspired by Boost.Bimap).
    Comparisons are supported for check-summing purposes, but not ordering.
    Persistence is supported though Boost.Serialization.

    \see  boost::coding::md5_digest
    \see  boost::coding::compute_md5(void const*,std::size_t)

    \todo  Replace "uint_fast64_t" with "uint_t&lt;significant_bits_per_length&gt;::
           fast", where "significant_bits_per_length" is 64.  (Need to tweak
           Boost.Integer to support 64-bit types.)  Also need to make a
           convienent constant for the "16" in the base class declaration.
           (It's the number of words per block, from RFC 1321, section 3.4.)
    \todo  The implementation base class was originally inherited privately, but
           the core member functions would give access errors when needed.  The
           solution, for now, was either make \e all the base classes (direct
           and indirect) friends of this class, or change the inheritance to
           either public or protected.  I haven't figured out why yet, or how to
           switch back to private inheritance without needing to add friendship.
           (Furthermore, I couldn't use \c base_type to establish friendship;
           i.e., both "friend base_type" and "friend class base_type" didn't
           work.  I don't know why either.)
 */
class md5_computerX
    : protected queued_bit_processing_base<md5_computerX, uint_fast64_t, 16u *
       md5_digest::bits_per_word>
{
    typedef queued_bit_processing_base<md5_computerX, uint_fast64_t, 16u *
     md5_digest::bits_per_word>  base_type;

    friend  void base_type::process_bit( bool );  // needs "update_hash" access

    // Implementation constants, followed by sanity checks
    static  std::size_t const  words_per_block = base_type::queue_length /
     md5_digest::bits_per_word;

    BOOST_STATIC_ASSERT( (base_type::queue_length % md5_digest::bits_per_word)
     == 0u );
    BOOST_STATIC_ASSERT( words_per_block == 16u );  // RFC 1321, section 3.4

public:
    // Special application interface
    using base_type::bits;
    using base_type::bytes;

    // Constants
    //! Number of bits for length quantities
    static  int const          significant_bits_per_length = 2 *
     md5_digest::bits_per_word;
    //! Number of bits in hash queue
    static  std::size_t const  bits_per_block = base_type::queue_length;

    //! Hashing sine table
    static  array<md5_digest::word_type, 64> const  hashing_table;

    // Types
    /** \brief  Type of checksums

        Represents the type of hashes generated by this type.
     */
    typedef md5_digest  value_type;
    /** \brief  Type of size values

        Represents the type used for sizing parameters and returns.  It should
        be an unsigned integer.
     */
    typedef base_type::size_type  size_type;

    /** \brief  Type of MD message lengths

        Represents the type needed to store the significant count of bits read.
     */
    typedef uint_least64_t  length_type;
      // replace w/ uint_t<significant_bits_per_length>::least
    /** \brief  Type of MD buffers

        Represents the intermediate MD buffer, holding the checksum for all
        prior \e completed hashed blocks.  The zero-index corresponds to the "A"
        register, up to index 3 representing the "D" register.
     */
    typedef array<md5_digest::word_type, md5_digest::words_per_digest>
      buffer_type;

    // Lifetime management (use automatic destructor)
    //! Default construction
    md5_computerX();
    //! Copy construction
    md5_computerX( md5_computerX const &c );

    /*! \name Assignment */ //@{
    // Assignment
    //! Sets state back to initial conditions
    void  reset();
    //! Changes the current state to a copy of another object's
    void  assign( md5_computerX const &c );

    //! Exchanges state with another object
    void  swap( md5_computerX &other );//@}

    /*! \name Inspection */ //@{
    // Inspectors
    //! Returns the count of bits read so far
    length_type  bits_read() const;
    //! Returns the checksum buffer of hashed bits
    buffer_type  last_buffer() const;

    using base_type::bits_unbuffered;
    using base_type::copy_unbuffered;//@}

    /*! \name Bit-stream reading */ //@{
    // Input processing
    using base_type::process_bit;
    using base_type::process_bits;
    using base_type::process_bit_copies;
    using base_type::process_byte;
    using base_type::process_byte_copies;
    using base_type::process_block;
    using base_type::process_bytes;
    using base_type::process_octet;

    //! Enters a word for hashing
    void  process_word( md5_digest::word_type word );
    //! Enters a double-word for hashing
    void  process_double_word( length_type dword );//@}

    /*! \name Message-digest writing */ //@{
    // Output processing
    //! Returns the message digest, assuming all bits have been hashed
    value_type  checksum() const;//@}

    /*! \name Operators */ //@{
    // Operators
    //! Copy-assignment
    md5_computerX &  operator =( md5_computerX const &c );

    //! Equals
    bool  operator ==( md5_computerX const &c ) const;
    //! Not-equals
    bool  operator !=( md5_computerX const &c ) const;

    //! Application
    value_type  operator ()() const;//@}

    // Extras
    //! Creates a copy of #hashing_table using calculated, not static, values
    static  array<md5_digest::word_type, 64>  generate_hashing_table();

private:
    // Bad if computers someday get so big that a byte overflows the queue!
    BOOST_STATIC_ASSERT( bits_per_block > CHAR_BIT );

    // State maintainence
    bool  test_invariant() const  { return true; }  // Nothing special right now

    // Serialization
    friend class serialization::access;

    /*! \name Persistence */ //@{
    //! Enables persistence with Boost.Serialization-compatible archives
    template < class Archive >
     void  serialize( Archive &ar, const unsigned int version );//@}
       // may have to do save/load split; support XML archives

    // Implementation functions
    void  update_hash( bool const *queue_b, bool const *queue_e );

    // Implementation types
    typedef md5_computerX  self_type;

    typedef uint_t<md5_digest::bits_per_word>::fast  iword_type;
    typedef array<iword_type, md5_digest::words_per_digest>  ibuffer_type;

    // (Computation) member data
    ibuffer_type  buffer_;

    static  ibuffer_type const  initial_buffer_;

};  // md5_computerX

/** \brief  A computer that produces MD5 message digests from consuming bits.

    This class is the bare-bones engine for the MD5 message-digest algorithm
    described in RFC 1321.  Besides computation, it also supports comparisons
    (equivalence only, not ordering) and serialization.

    \see  boost::coding::md5_digest
 */
class md5_context
{
    typedef md5_context  self_type;

    friend class md5_computer;

public:
    typedef md5_digest  product_type;
    typedef bool        consumed_type;

    void  operator ()( consumed_type bit )  { this->worker.process_bit(bit); }
    bool  operator ==( self_type const &o ) const
      { return this->worker == o.worker; }
    bool  operator !=( self_type const &o ) const
      { return !this->operator ==( o ); }
    product_type  operator ()() const  { return this->worker.checksum(); }

private:
    md5_computerX  worker;

    // Serialization
    friend class boost::serialization::access;

    template < class Archive >
    void  serialize( Archive &ar, const unsigned int version );  // not defined yet

};  // md5_context

/** \brief  A class for generating a MD5 message digest from submitted data.

    This class can accept data in several runs and produce a hash based on that
    data from the MD5 message-digest algorithm described in RFC 1321.  It should
    have a similar interface to Boost.CRC, plus specialized function object
    interfaces for bit- and byte-level processing (inspired by Boost.Bimap).
    Comparisons are supported for check-summing purposes, but not ordering.
    Persistence is supported though Boost.Serialization.

    \see  boost::coding::md5_digest
    \see  boost::coding::md5_context
    \see  boost::coding::compute_md5(void const*,std::size_t)

    \todo  Replace "uint_fast64_t" with "uint_t&lt;significant_bits_per_length&gt;::
           fast", where "significant_bits_per_length" is 64.  (Need to tweak
           Boost.Integer to support 64-bit types.)  Also need to make a
           convienent constant for the "16" in the base class declaration.
           (It's the number of words per block, from RFC 1321, section 3.4.)
 */
class md5_computer
    : public bit_coding_shell<md5_context, true>
{
    typedef bit_coding_shell<md5_context, true>  base_type;
    typedef md5_computer                         self_type;

public:
    // Constants
    static  int const            significant_bits_per_length =
     md5_computerX::significant_bits_per_length;
    static  std::size_t const    bits_per_block = md5_computerX::bits_per_block;
    static  array<md5_digest::word_type, 64> const  hashing_table;

    // Types
    typedef uint_least64_t  length_type;
    typedef array<md5_digest::word_type, md5_digest::words_per_digest>
      buffer_type;

    // Assignment
    //! Sets state back to initial conditions
    void  reset()  { *this = self_type(); }
    //! Changes the current state to a copy of another object's
    void  assign( self_type const &c )  { *this = c; }

    //! Exchanges state with another object
    void  swap( self_type &other )  { std::swap( *this, other ); }

    // Inspectors
    //! Returns the count of bits read so far
    length_type  bits_read() const
      { return this->context().worker.bits_read(); }
    //! Returns the checksum buffer of hashed bits
    buffer_type  last_buffer() const
      { return this->context().worker.last_buffer(); }
    //! Returns the count of the queued bits
    length_type  bits_unbuffered() const
      { return this->context().worker.bits_unbuffered(); }
    //! Copies out the queued bits
    template < typename OutputIterator >
    OutputIterator  copy_unbuffered( OutputIterator o ) const
      { return this->context().worker.copy_unbuffered( o ); }

    // Input processing
    //! Enters a word for hashing
    void  process_word( md5_digest::word_type word )
      { this->context().worker.process_word( word ); }
    //! Enters a double-word for hashing
    void  process_double_word( length_type dword )
      { this->context().worker.process_double_word( dword ); }

    // Operators
    bool  operator ==( self_type const &o ) const
      { return this->base_type::operator ==( o ); }
    bool  operator !=( self_type const &o ) const
      { return !this->operator ==( o ); }

    // Extras
    static  array<md5_digest::word_type, 64>  generate_hashing_table()
      { return md5_computerX::generate_hashing_table(); }

private:
    // Serialization
    friend class boost::serialization::access;

    template < class Archive >
    void  serialize( Archive &ar, const unsigned int version );  // not defined yet

};  // md5_computer


//! \cond
//  Implementation details  --------------------------------------------------//

namespace detail
{

// MD5 message digest constants, especially for I/O
struct md5_constants
{
    // Nybbles and hexadecimal digits
    static  std::size_t const  nybbles_per_hexadecimal_digit = 1u;
    static  std::size_t const  bits_per_nybble = 4u;

    static  std::size_t const  number_of_hexadecimal_digits = 16u;  // duh!

    static  char const  hex_digits_lc[ number_of_hexadecimal_digits + 1 ];
    static  char const  hex_digits_uc[ number_of_hexadecimal_digits + 1 ];

    // MD words
    static  std::size_t const  nybbles_per_word = md5_digest::bits_per_word /
     bits_per_nybble;

    // MD strings
    static  std::size_t const  characters_per_digest = md5_digest::bits_per_word
     * md5_digest::words_per_digest / ( nybbles_per_hexadecimal_digit *
     bits_per_nybble );

};  // md5_constants

}  // namespace detail
//! \endcond


//  MD5 message-digest structure non-member operator function definitions  ---//

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
    return std::equal( l.hash, l.hash + md5_digest::words_per_digest, r.hash );
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
    \param n  The \c md5_digest object to store the read.

    \return  \p i

    \see  boost::coding::operator<<(std::basic_ostream<Ch,Tr>&,md5_digest const&)

    \relates  boost::coding::md5_digest
 */
template < typename Ch, class Tr >
std::basic_istream<Ch, Tr> &
operator >>( std::basic_istream<Ch, Tr> &i, md5_digest &n )
{
    typename std::basic_istream<Ch, Tr>::sentry  is( i );

    if ( is )
    {
        // Set up
        BOOST_AUTO( const &  f, std::use_facet< std::ctype<Ch> >(i.getloc()) );
        std::size_t  nybble_index = 0u;
        md5_digest   temp = { {0} };

        // Read the exact number of characters
        for ( std::istreambuf_iterator<Ch, Tr>  ii(i), ie ; (ie != ii) &&
         (nybble_index < detail::md5_constants::characters_per_digest) ; ++ii,
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
            temp.hash[ nybble_index / detail::md5_constants::nybbles_per_word ]
             |= nybble << ( detail::md5_constants::bits_per_nybble * ((
             nybble_index % detail::md5_constants::nybbles_per_word ) ^ 0x01u)
             );
        }

abort_read:
        // Finish up
        if ( nybble_index < detail::md5_constants::characters_per_digest )
        {
            // Incomplete read
            i.setstate( std::ios_base::failbit );
        }
        else
        {
            // Successful read
            n = temp;
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
    \param n  The \c md5_digest object to be written.

    \return  \p o

    \see  boost::coding::operator>>(std::basic_istream<Ch,Tr>&,md5_digest&)

    \relates  boost::coding::md5_digest
 */
template < typename Ch, class Tr >
std::basic_ostream<Ch, Tr> &
operator <<( std::basic_ostream<Ch, Tr> &o, md5_digest const &n )
{
    // The message always has an exact number of characters; plot it out.
    // (Leave an extra character for the NUL terminator.)
    char    hex_string[ detail::md5_constants::characters_per_digest + 1u ];
    char *  p = hex_string;

    // Each nybble will be printed as a hexadecimal digit.
    char const  (&digits)[ detail::md5_constants::number_of_hexadecimal_digits +
     1 ] = ( o.flags() & std::ios_base::uppercase )
     ? detail::md5_constants::hex_digits_uc
     : detail::md5_constants::hex_digits_lc;

    // Print each nybble.  Since the nybble progression within an octet is the
    // reverse of the octet and word progressions, stick in a reversal flag
    // while indexing.
    for ( std::size_t  nybble_index = 0u ; nybble_index <
     detail::md5_constants::characters_per_digest ; ++nybble_index )
    {
        *p++ = digits[ 0x0Fu & (n.hash[ nybble_index /
         detail::md5_constants::nybbles_per_word ] >> (
         detail::md5_constants::bits_per_nybble * (( nybble_index %
         detail::md5_constants::nybbles_per_word ) ^ 0x01u) )) ];
    }
    *p = '\0';

    // Print the message, taking stream settings into account
    return o << hex_string;
}


//  MD5 message-digest computation constructor definitions  ------------------//

/** Constructs a \c md5_computerX set to initial conditions.  That is, with the
    buffer initialized as in RFC 1321, section 3.3, and no bits counted as read
    or currently left to be hashed.

    \post  <code>#bits_read() == 0</code>
    \post  <code>#last_buffer() == { 0x67452301, 0xEFCDAB89, 0x98BACDFE,
           0x10325476 }</code>
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused
    \post  \c #bits and \c #bytes point to \c *this
 */
inline
md5_computerX::md5_computerX()
    : base_type()
    , buffer_( self_type::initial_buffer_ )
{
    BOOST_ASSERT( this->test_invariant() );
}

/** Constructs a \c md5_computerX to the same computation state as <var>c</var>.

    \param c  The original object to be copied.

    \post  <code>#bits_read() == <var>c</var>.bits_read()</code>
    \post  <code>#last_buffer() == <var>c</var>.last_buffer()</code>
    \post  Given <code>#copy_unbuffered(<var>o1</var>)</code> and
           <code><var>c</var>.copy_unbuffered(<var>o2</var>)</code>, where both
           \p o1 and \p o2 are random access iterators to different container
           segments that each have at least <code>#bits_unbuffered()</code>
           elements available, <code>std::equal( o1, o1 + bits_unbuffered(), o2
           ) == true</code>
    \post  \c #bits and \c #bytes point to \c *this
 */
inline
md5_computerX::md5_computerX( md5_computerX const &c )
    : base_type( c )
    , buffer_( c.buffer_ )
{
    BOOST_ASSERT( this->test_invariant() );
}


//  MD5 message-digest computation inspector member function definitions  ----//

/** Returns the number of bits that have been processed, both those that have
    been hashed and those that are on queue.  Only the low-order 64 bits of
    count are significant and kept/reliable.

    \return  How many bits have been submitted, hashed and queued.
 */
inline
md5_computerX::length_type
md5_computerX::bits_read() const
{
    // Don't count any wrap-around past 2**64
    // (Use mask value once Boost.Integer is upped to 64-bit support)
    return this->base_type::bits_read() & 0xFFFFFFFFFFFFFFFFull;
}

/** Returns the checksum of all the bits that have been hashed so far.  Hashing
    occurs only after every \c #bits_per_block bit entries, so check
    \c #bits_unbuffered() for any queued stragglers.

    \return  The current state of the MD buffer, not counting any unhashed bits.
 */
inline
md5_computerX::buffer_type
md5_computerX::last_buffer() const
{
    // boost::array has no constructors (since it's POD), that means that if
    // buffer_type and ibuffer_type differ, we need to convert via assignment.
    buffer_type  r;

    r = this->buffer_;
    return r;
}


//  MD5 message-digest computation assignment member function definitions  ---//

/** Changes an object to be like it was default-constructed.

    \post  <code>#bits_read() == 0</code>
    \post  <code>#last_buffer() == { 0x67452301, 0xEFCDAB89, 0x98BACDFE,
           0x10325476 }</code>
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused

    \see  #md5_computerX()
 */
inline
void
md5_computerX::reset()
{
    this->base_type::reset();
    this->buffer_ = self_type::initial_buffer_;

    BOOST_ASSERT( this->test_invariant() );
}

/** Changes an object to be like the given object.  Only the computation
    elements are copied; no function object proxies are reseated.

    \param c  The object with the new state to be copied.

    \post  <code>#bits_read() == <var>c</var>.bits_read()</code>
    \post  <code>#last_buffer() == <var>c</var>.last_buffer()</code>
    \post  Given <code>#copy_unbuffered(<var>o1</var>)</code> and
           <code><var>c</var>.copy_unbuffered(<var>o2</var>)</code>, where both
           \p o1 and \p o2 are random access iterators to different container
           segments that each have at least <code>#bits_unbuffered()</code>
           elements available, <code>std::equal( o1, o1 + bits_unbuffered(), o2
           ) == true</code>
    \post  \c #bits and \c #bytes \e still point to \c *this

    \see  #md5_computerX(md5_computerX const&)
 */
inline
void
md5_computerX::assign( md5_computerX const &c )
{
    this->base_type::assign( c );
    this->buffer_ = c.buffer_;

    BOOST_ASSERT( this->test_invariant() );
}

/** Swaps the content of this object with another.  Only the computation
    elements are changed; no function object proxies are reseated.

    \param other  The other object to trade state with this object.

    \post  <code>*this == <var>old_other</var> &amp;&amp; <var>old_this</var> ==
           <var>other</var></code>
 */
inline
void
md5_computerX::swap( md5_computerX &other )
{
    // Use the appropriate swap via Koeing look-up
    using std::swap;

    // Swap the computation members (don't reseat the function object proxies)
    this->base_type::swap( other );
    swap( this->buffer_, other.buffer_ );

    BOOST_ASSERT( this->test_invariant() );
    BOOST_ASSERT( other.test_invariant() );
}


//  MD5 message-digest computation bit-input member function definitions  ----//

/** Submits 32-bit MD-word for computation.  The word is submitted an octet at a
    time, from the lowest-order octet to the highest.

    \param word  The word value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 32</code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           32) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() + 32
           &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + 32 &lt;
           bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 32</code>
           (assuming that \p o2 is, at least, a forward iterator), otherwise the
           range \p o3 to <code>copy_unbuffered(<var>o3</var>)</code> contains
           the <code><var>old_this</var>.bits_unbuffered() + 32 -
           bits_per_block</code> lowest-order bits of \p word

    \see  #process_octet(boost::uint_least8_t)
 */
inline
void
md5_computerX::process_word( md5_digest::word_type word )
{
    this->process_octet( word & 0xFFul );
    this->process_octet( (word >> 8) & 0xFFul );
    this->process_octet( (word >> 16) & 0xFFul );
    this->process_octet( (word >> 24) & 0xFFul );
}

/** Submits 64-bit MD-length for computation.  The double-word is submitted a
    word at a time, first the low-order word then the high-order word.

    \param dword  The double-word value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 64</code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           64) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() + 64
           &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + 64 &lt;
           bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 64</code>
           (assuming that \p o2 is, at least, a forward iterator), otherwise the
           range \p o3 to <code>copy_unbuffered(<var>o3</var>)</code> contains
           the <code><var>old_this</var>.bits_unbuffered() + 64 -
           bits_per_block</code> lowest-order bits of \p word

    \see  #process_word(boost::coding::md5_digest::word_type)
 */
inline
void
md5_computerX::process_double_word( md5_computerX::length_type dword )
{
    this->process_word( dword & 0xFFFFFFFFull );
    this->process_word( (dword >> 32) & 0xFFFFFFFFull );
}


//  MD5 message-digest structure member operator function definitions  -------//

/** Changes a MD5 message computer to have the same observable state as a given
    computer.  (No function object proxies are reseated, however.)

    \param c  The source object with the new state.

    \return  \c *this

    \post  <code>#bits_read() == <var>c</var>.bits_read()</code>
    \post  <code>#last_buffer() == <var>c</var>.last_buffer()</code>
    \post  Given <code>#copy_unbuffered(<var>o1</var>)</code> and
           <code><var>c</var>.copy_unbuffered(<var>o2</var>)</code>, where both
           \p o1 and \p o2 are random access iterators to different container
           segments that each have at least <code>#bits_unbuffered()</code>
           elements available, <code>std::equal( o1, o1 + bits_unbuffered(), o2
           ) == true</code>
    \post  \c #bits and \c #bytes \e still point to \c *this

    \see  #assign(md5_computerX const&)
 */
inline
md5_computerX &
md5_computerX::operator =( md5_computerX const &c )
{
    this->assign( c );
    return *this;
}

/** Compares MD5 message computers for equivalence.  Such computers are equal if
    all of the corresponding parts of their significant state (data length,
    running hash, and bit queue) are equal.

    \param c  The right-side operand to be compared.

    \retval true   \c *this and \p c are equivalent.
    \retval false  \c *this and \p c are not equivalent.

    \see  #bits_read()
    \see  #last_buffer()
    \see  #copy_unbuffered(OutputIterator)
 */
inline
bool
md5_computerX::operator ==( md5_computerX const &c ) const
{
    // Don't compare the function object proxies since they don't carry
    // significant state.  (Furthermore, they can't change once initalized and
    // don't have any comparison operators.)
    return ( this->buffer_ == c.buffer_ ) && this->base_type::operator ==( c );
}

/** Compares MD5 message computers for non-equivalence.  Such computers are
    unequal if at least one set of corresponding parts of their significant
    state (data length, running hash, and bit queue) are unequal.

    \param c  The right-side operand to be compared.

    \retval true   \c *this and \p c are not equivalent.
    \retval false  \c *this and \p c are equivalent.

    \see  #operator==(md5_computerX const&)const
 */
inline
bool
md5_computerX::operator !=( md5_computerX const &c ) const
{
    return !this->operator ==( c );
}

/** Computes the check-sum of the submitted data, through a standard generator
    interface.

    \return  The generated check-sum.

    \see  #checksum()const
 */
inline
md5_computerX::value_type
md5_computerX::operator ()() const
{
    return this->checksum();
}


//  MD5 message-digest computation miscellaneous function definitions  -------//

/** \brief  Non-member swapping function for \c md5_computer

    Exchanges the states of two \c md5_computer objects.  This specialization of
    the algorithm can be called by generic code that uses free-function
    (template) swap assisted with Koenig lookup.

    \param a  The first object involved in the swap.
    \param b  The second object involved in the swap.

    \post  <code><var>a</var> == <var>old_b</var> &amp;&amp; <var>old_a</var> ==
           <var>b</var></code>

    \see  boost::coding::md5_computer::swap

    \relates  boost::coding::md5_computer
 */
inline
void
swap
(
    md5_computer &  a,
    md5_computer &  b
)
{
    a.swap( b );
}


//  MD5 message-digest computation function definition  ----------------------//

/** \brief  Immediate MD5 message-digest computation

    Determines the MD5 message-digest of a given block of data, without
    requiring the setup of a computation object.

    \pre  \p buffer must point to a valid region of memory that contains at
          least \p byte_count bytes past the given pointer.

    \param buffer      Points to the beginning of the data block to be
                       processed.
    \param byte_count  The length of the data block to be processed, in bytes.

    \return  The MD5 message digest of the data block.

    \see  boost::coding::md5_digest
    \see  boost::coding::md5_computer
 */
inline
md5_digest
compute_md5
(
    void const *  buffer,
    std::size_t   byte_count
)
{
    md5_computer  c;

    c.process_bytes( buffer, byte_count );
    return c.checksum();
}


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_MD5_HPP
