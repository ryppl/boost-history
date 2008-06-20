// Boost md5.hpp header file  ------------------------------------------------//

// (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for documentation.

/** \file
    \brief  Declarations of MD5 computation components

    Contains the declaration of types and functions used for computing MD5
    message digests of given data blocks and granting I/O capability to any
    applicable types.
 */

#ifndef BOOST_CODING_MD5_HPP
#define BOOST_CODING_MD5_HPP

#include <boost/coding_fwd.hpp>

#include <boost/array.hpp>                 // for boost::array
#include <boost/assert.hpp>                // for BOOST_ASSERT
#include <boost/cstdint.hpp>               // for boost::uint_least8_t
#include <boost/foreach.hpp>               // for BOOST_FOREACH
#include <boost/integer.hpp>               // for boost::uint_t
#include <boost/serialization/access.hpp>  // for boost::serialization::access
#include <boost/static_assert.hpp>         // for BOOST_STATIC_ASSERT
#include <boost/typeof/typeof.hpp>         // for BOOST_AUTO

#include <algorithm>  // for std::equal, swap, copy
#include <climits>    // for CHAR_BIT
#include <cstddef>    // for std::size_t, NULL
#include <cstring>    // for std::strlen
#include <ios>        // for std::ios
#include <istream>    // for std::basic_istream
#include <locale>     // for std::use_facet, ctype
#include <ostream>    // for std::basic_ostream
#include <utility>    // for std::make_pair


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

    \see  boost::coding::md5_computer
    \see  boost::coding::compute_md5(void const*,std::size_t)
 */
class md5_digest
{
public:
    /** \brief  Number of bits for word-sized quantities

        Represents the number of bits per word as given in RFC 1321, section 2.
     */
    static  int const  bits_per_word = 32;

    /** \brief  Type of MD register

        Represents the type of each register of the MD buffer.
     */
    typedef uint_t<bits_per_word>::least  word_type;

    /** \brief  Length of MD buffer

        Represents the number of registers in a MD buffer.
     */
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
 */
class md5_computer
{
    /** \brief  A class for submitting bit-oriented data for a MD5 message
                digest in a function-object interface.

        This class represents objects that are proxy views to their owning MD5
        computing object.  The proxy interface appears as a function object that
        takes a \c bool value, which passes it on to the owning computer for
        processing.  It should be suitable STL-like algorithms that use such a
        function object.  It supports assignment to work with \c std::for_each
        and other algorithms that can return a post-use function object.

        \see  boost::coding::md5_computer
        \see  #byte_applicator
        \see  #bits

        \todo  The current store-by-reference semantics for the owning computer
               means that said owner is always changed during an application of
               \c *this.  Maybe write-back to the owner should only be explicit
               with the assignment operator.
     */
    class bit_applicator
    {
        friend class md5_computer;
        md5_computer  *parent_;
        explicit  bit_applicator( md5_computer &p )  : parent_( &p )  {};
    public:
        //! Application
        void              operator ()( bool v );
        //! Copy-assignment
        bit_applicator &  operator  =( bit_applicator const &c );

    };  // bit_applicator

    /** \brief  A class for submitting byte-oriented data for a MD5 message
                digest in a function-object interface.

        This class represents objects that are proxy views to their owning MD5
        computing object.  The proxy interface appears as a function object that
        takes an <code>unsigned char</code> value, which passes it on to the
        owning computer for processing.  It should be suitable STL-like
        algorithms that use such a function object.  It supports assignment to
        work with \c std::for_each and other algorithms that can return a
        post-use function object.

        \see  boost::coding::md5_computer
        \see  #bit_applicator
        \see  #bytes

        \todo  The current store-by-reference semantics for the owning computer
               means that said owner is always changed during an application of
               \c *this.  Maybe write-back to the owner should only be explicit
               with the assignment operator.
     */
    class byte_applicator
    {
        friend class md5_computer;
        md5_computer  *parent_;
        explicit  byte_applicator( md5_computer &p )  : parent_( &p )  {};
    public:
        //! Application
        void               operator ()( unsigned char v );
        //! Copy-assignment
        byte_applicator &  operator  =( byte_applicator const &c );

    };  // byte_applicator

    // Implementation constants
    static  std::size_t const  words_per_block = 16u;  // RFC 1321, section 3.4

public:
    // Special application interface
    /** \brief  Proxy for bit-oriented application interface

        Accesses an interface where <code>*this</code> can be used as a function
        object take can take a single \c bool value as input.  It cannot be
        reseated from <code>*this</code>, but assignment to it will copy the
        other's owner's state to <code>*this</code>, enabling algorithms that
        return updated function objects to work.

        \attention  Since #bit_applicator stores a non-constant reference to its
                    owner, the owner will be change through any application
                    through \c bits, even if \c bits is passed by value.

        \see  #bit_applicator
     */
    bit_applicator   bits;
    /** \brief  Proxy for byte-oriented application interface

        Accesses an interface where <code>*this</code> can be used as a function
        object take can take a single <code>unsigned char</code> value as input.
        It cannot be reseated from <code>*this</code>, but assignment to it will
        copy the other's owner's state to <code>*this</code>, enabling
        algorithms that return updated function objects to work.

        \attention  Since #byte_applicator stores a non-constant reference to
                    its owner, the owner will be change through any application
                    through \c bytes, even if \c bytes is passed by value.

        \see  #byte_applicator
     */
    byte_applicator  bytes;

    // Constants
    /** \brief  Number of bits for length quantities

        Represents the number of significant (low-order) bits kept for the
        message length, which can also be processed as two words, all as given
        in RFC 1321, section 3.2, paragraph 1.
     */
    static  int const          significant_bits_per_length = 2 *
     md5_digest::bits_per_word;
    /** \brief  Number of bits in hash queue

        Represents the number of submitted bits that are queued until that queue
        is emptied as a single block to be hashed, implied from RFC 1321,
        section 3.4.  The count of unhashed bits is always less than this value.
        (The input processing member functions trigger a hash right after a bit
        fills the queue.)
     */
    static  std::size_t const  bits_per_block = words_per_block *
     md5_digest::bits_per_word;

    /** \brief  Hashing sine table

        Sample of the table described in RFC 1321, section 3.4, paragraph 4.
        Its values are taken directly from the "MD5Transform" function in the
        RFC's section A.3, and are not computed.  Of course, the index is
        zero-based (C++) instead of one-based (RFC).

        \see  #generate_hashing_table
     */
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
    typedef std::size_t  size_type;

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
    md5_computer();
    //! Copy construction
    md5_computer( md5_computer const &c );

    /*! \name Assignment */ //@{
    // Assignment
    //! Sets state back to initial conditions
    void  reset();
    //! Changes the current state to a copy of another object's
    void  assign( md5_computer const &c );

    //! Exchanges state with another object
    void  swap( md5_computer &other );//@}

    /*! \name Inspection */ //@{
    // Inspectors
    //! Returns the count of bits read so far
    length_type  bits_read() const;
    //! Returns the count of bits not hashed into the buffer yet
    length_type  bits_unbuffered() const;
    //! Returns the checksum buffer of hashed bits
    buffer_type  last_buffer() const;

    //! Copies out the unhashed bits
    template < typename OutputIterator >
     OutputIterator  copy_unbuffered( OutputIterator o ) const;//@}

    /*! \name Bit-stream reading */ //@{
    // Input processing
    //! Enters one bit for hashing
    void  process_bit( bool bit );
    //! Enters part of a byte for hashing
    void  process_bits( unsigned char bits, size_type bit_count );
    //! Enters several bits, all of the same value, for hashing
    void  process_bit_copies( bool value, size_type bit_count );

    //! Enters a whole byte for hashing
    void  process_byte( unsigned char byte );
    //! Enters several bytes, all of the same value, for hashing
    void  process_byte_copies( unsigned char value, size_type byte_count );

    //! Enters a range of bytes in memory for hashing
    void  process_block( void const *bytes_begin, void const *bytes_end );
    //! Enters a byte buffer in memory for hashing
    void  process_bytes( void const *buffer, size_type byte_count );

    //! Enters an octet for hashing
    void  process_octet( uint_least8_t octet );
    //! Enters a word for hashing
    void  process_word( md5_digest::word_type word );
    //! Enters a double-word for hashing
    void  process_double_word( length_type dword );//@}

    /*! \name Message-digest writing */ //@{
    // Output processing
    //! Returns the message digest, assuming all bits have been hashed
    /** Provides the computed check-sum of all the submitted bits.  (The queued
        bits are temporarily hashed with a special finishing procedure.)

        \return  The check-sum (i.e. message digest).
     */
    value_type  checksum() const;//@}

    /*! \name Operators */ //@{
    // Operators
    //! Copy-assignment
    md5_computer &  operator =( md5_computer const &c );

    //! Equals
    bool  operator ==( md5_computer const &c ) const;
    //! Not-equals
    bool  operator !=( md5_computer const &c ) const;

    //! Application
    value_type  operator ()() const;//@}

    // Extras
    //! Creates a copy of #hashing_table using calculated, not static, values
    /** Constructs the hashing sine table based on the directions given in RFC
        1321, section 3.4, paragraph 4.  It should give the same values as
        #hashing_table, but it's dependent on the quality of the environment's
        math library, thereby giving a test of the environment.

        \return  The computed hashing sine table

        \relates  #hashing_table
     */
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
    void  update_hash();

    // Implementation types
    typedef md5_computer  self_type;

    typedef uint_fast64_t  ilength_type;
      // replace w/ uint_t<significant_bits_per_length>::fast
    typedef uint_t<md5_digest::bits_per_word>::fast  iword_type;
    typedef array<iword_type, md5_digest::words_per_digest>  ibuffer_type;

    typedef array<bool, bits_per_block>  block_type;

    // (Computation) member data
    ilength_type  length_;
    ibuffer_type  buffer_;
    block_type    unbuffered_;

    static  ibuffer_type const  initial_buffer_;

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


//  MD5 computation special applicator operator definitions  -----------------//

/** Calls <code><var>o</var>.process_bit( <var>v</var> )</code>, where
    \p o is the owning \c md5_computer of <code>*this</code>.

    \param v  The bit to process.

    \see  boost::coding::md5_computer::process_bit(bool)
 */
inline
void
md5_computer::bit_applicator::operator ()( bool v )
{
    this->parent_->process_bit( v );
}

/** Calls <code>this-&gt;<var>p</var> = <var>c.p</var></code>, where
    \p p holds the owning \c md5_computer of a particular proxy.

    \return  <code>*this</code>

    \see  boost::coding::md5_computer::operator=(md5_computer const&)
 */
inline
md5_computer::bit_applicator &
md5_computer::bit_applicator::operator  =
(
    md5_computer::bit_applicator const &  c
)
{
    *this->parent_ = *c.parent_;
    return *this;
}

/** Calls <code><var>o</var>.process_byte( <var>v</var> )</code>, where
    \p o is the owning \c md5_computer of <code>*this</code>.

    \param v  The byte to process.

    \see  boost::coding::md5_computer::process_byte(unsigned char)
 */
inline
void
md5_computer::byte_applicator::operator ()( unsigned char v )
{
    this->parent_->process_byte( v );
}

/** Calls <code>this-&gt;<var>p</var> = <var>c.p</var></code>, where
    \p p holds the owning \c md5_computer of a particular proxy.

    \return  <code>*this</code>

    \see  boost::coding::md5_computer::operator=(md5_computer const&)
 */
inline
md5_computer::byte_applicator &
md5_computer::byte_applicator::operator  =
(
    md5_computer::byte_applicator const &  c
)
{
    *this->parent_ = *c.parent_;
    return *this;
}


//  MD5 message-digest computation constructor definitions  ------------------//

/** Constructs a \c md5_computer set to initial conditions.  That is, with the
    buffer initialized as in RFC 1321, section 3.3, and no bits counted as read
    or currently left to be hashed.

    \post  <code>#bits_read() == 0</code>
    \post  <code>#last_buffer() == { 0x67452301, 0xEFCDAB89, 0x98BACDFE,
           0x10325476 }</code>
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused
    \post  \c #bits and \c #bytes point to \c *this
 */
inline
md5_computer::md5_computer()
    : bits( *this ), bytes( *this )
    , length_(), buffer_( self_type::initial_buffer_ ), unbuffered_()
{
    BOOST_ASSERT( this->test_invariant() );
}

/** Constructs a \c md5_computer to the same computation state as <var>c</var>.

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
md5_computer::md5_computer( md5_computer const &c )
    : bits( *this ), bytes( *this )
    , length_( c.length_ ), buffer_( c.buffer_ ), unbuffered_( c.unbuffered_ )
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
md5_computer::length_type
md5_computer::bits_read() const
{
    return this->length_;
}

/** Returns the number of bits that have not been hashed.  Hashing occurs only
    after every \c #bits_per_block bit entries, so this member function can
    confirm queued stragglers. (The identities of hashed bits are not
    retrievable.)

    \return  How many bits are queued to be hashed.

    \see  #bits_read()
    \see  #bits_per_block
 */
inline
md5_computer::length_type
md5_computer::bits_unbuffered() const
{
    return this->length_ % self_type::bits_per_block;
}

/** Returns the checksum of all the bits that have been hashed so far.  Hashing
    occurs only after every \c #bits_per_block bit entries, so check
    \c #bits_unbuffered() for any queued stragglers.

    \return  The current state of the MD buffer, not counting any unhashed bits.
 */
inline
md5_computer::buffer_type
md5_computer::last_buffer() const
{
    // boost::array has no constructors (since it's POD), that means that if
    // buffer_type and ibuffer_type differ, we need to convert via assignment.
    buffer_type  r;

    r = this->buffer_;
    return r;
}

/** Copies the last submitted bits that have not yet hashed into the running
    checksum, starting from the oldest submission.  Use \c #bits_unbuffered()
    for advance notice of how many iterations are done.  (Always less than
    \c #bits_per_block elements are copied.)

    \pre  At least \c #bits_unbuffered() more elements are free to be created
          and/or assigned through \p o

    \tparam OutputIterator  The type of the iterator submitted.  It should match
                            the requirements of either an output or a forward
                            (or above) mutable iterator over something that can
                            receive \c bool values via dereferenced assignment.

    \param o  The iterator starting the destination range.

    \return  \p o after copying.

    \see  #bits_unbuffered()
    \see  #bits_per_block
 */
template < typename OutputIterator >
inline
OutputIterator
md5_computer::copy_unbuffered( OutputIterator o ) const
{
    return std::copy( this->unbuffered_.begin(), this->unbuffered_.begin() +
     this->bits_unbuffered(), o );
}


//  MD5 message-digest computation assignment member function definitions  ---//

/** Changes an object to be like it was default-constructed.

    \post  <code>#bits_read() == 0</code>
    \post  <code>#last_buffer() == { 0x67452301, 0xEFCDAB89, 0x98BACDFE,
           0x10325476 }</code>
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused

    \see  #md5_computer()
 */
inline
void
md5_computer::reset()
{
    // The "bits" and "bytes" members don't/can't reset.  The "unbuffered_"
    // member doesn't need it either since "length_" reset will cause existing
    // elements of "unbuffered_" to be ignored.
    this->length_ = 0u;
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

    \see  #md5_computer(md5_computer const&)
 */
inline
void
md5_computer::assign( md5_computer const &c )
{
    // Don't/can't reseat the function object proxies; only the elements of
    // "unbuffered_" that will remain significant need to be copied.
    this->length_ = c.length_;
    this->buffer_ = c.buffer_;
    std::copy( c.unbuffered_.begin(), c.unbuffered_.begin() +
     c.bits_unbuffered(), this->unbuffered_.begin() );

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
md5_computer::swap( md5_computer &other )
{
    // Use the appropriate swap via Koeing look-up
    using std::swap;

    // Swap the computation members (don't reseat the function object proxies)
    swap( this->length_, other.length_ );
    swap( this->buffer_, other.buffer_ );
    swap( this->unbuffered_, other.unbuffered_ );

    BOOST_ASSERT( this->test_invariant() );
    BOOST_ASSERT( other.test_invariant() );
}


//  MD5 message-digest computation bit-input member function definitions  ----//

/** Submits a single bit for computation.  The bit is queued; if this bit fills
    the queue, the queue's bits are hashed and the queue is emptied.

    \param bit  The bit value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 1</code>
    \post  <code>#bits_unbuffered() == bits_read() % #bits_per_block ? 0 :
           <var>old_this</var>.bits_unbuffered() + 1</code> 
    \post  <code>#last_buffer() == bits_read() % bits_per_block ?
           <var>new_value</var> : <var>old_this</var>.last_buffer()</code>  (The
           new value is computed with the algorithm described in RFC 1321,
           section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 1</code>
           (assuming that \p o2 is, at least, a forward iterator)
 */
inline
void
md5_computer::process_bit( bool bit )
{
    this->unbuffered_[ this->bits_unbuffered() ] = bit;
    if ( 0u == (++this->length_ % self_type::bits_per_block) )
        this->update_hash();
    BOOST_ASSERT( this->test_invariant() );
}

/** Submits part of a byte for computation.  Bits are submitted starting from
    the highest-order bit to the lowest.

    \pre  <code>0 &lt; <var>bit_count</var> &lt;= CHAR_BIT</code>

    \param bits       The byte from which the values are submitted.
    \param bit_count  The number of bits to submit, starting from the
                      2<sup><var>bit_count</var> - 1</sup> place down to the
                      ones-place.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() +
           <var>bit_count</var></code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           <var>bit_count</var>) % bits_per_block</code> 
    \post  <code>#last_buffer() == <var>old_this</var>.bits_unbuffered() +
           <var>bit_count</var> &gt;= bits_per_block ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() +
           <var>bit_count</var> &lt; bits_per_block,</code> then
           <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) ==
           <var>bit_count</var></code> (assuming that \p o2 is, at least, a
           forward iterator), otherwise the range \p o3 to
           <code>copy_unbuffered(<var>o3</var>)</code> contains the
           <code><var>old_this</var>.bits_unbuffered() + <var>bit_count</var> -
           bits_per_block</code> lowest-order bits of \p bits

    \see  #process_bit(bool)
 */
inline
void
md5_computer::process_bits
(
    unsigned char            bits,
    md5_computer::size_type  bit_count
)
{
    BOOST_ASSERT( (0u < bit_count) && (bit_count <= CHAR_BIT) );
    for ( unsigned char  m = 0x01u << (bit_count - 1u) ; bit_count-- ; m >>= 1 )
        this->process_bit( bits & m );
}

/** Submits multiple copies of a single bit value for computation.

    \param value      The bit value to be submitted.
    \param bit_count  The number of bits to submit.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() +
           <var>bit_count</var></code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           <var>bit_count</var>) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() +
           <var>bit_count</var> &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() +
           <var>bit_count</var> &lt; bits_per_block,</code> then
           <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) ==
           <var>bit_count</var></code> (assuming that \p o2 is, at least, a
           forward iterator), otherwise the range \p o3 to
           <code>copy_unbuffered(<var>o3</var>)</code> contains
           <code>(<var>old_this</var>.bits_unbuffered() + <var>bit_count</var>)
           % bits_per_block</code> copies of \p value

    \see  #process_bit(bool)
 */
inline
void
md5_computer::process_bit_copies
(
    bool                     value,
    md5_computer::size_type  bit_count
)
{
    while ( bit_count-- )
        this->process_bit( value );
}

/** Submits a byte for computation.  The bits are submitted starting from the
    highest-order bit to the lowest.

    \param byte  The byte value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() +
           CHAR_BIT</code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           CHAR_BIT) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() +
           CHAR_BIT &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + CHAR_BIT
           &lt; bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>)
           - <var>old_this</var>.copy_unbuffered(<var>o2</var>) ==
           CHAR_BIT</code> (assuming that \p o2 is, at least, a forward
           iterator), otherwise the range \p o3 to
           <code>copy_unbuffered(<var>o3</var>)</code> contains the
           <code><var>old_this</var>.bits_unbuffered() + CHAR_BIT -
           bits_per_block</code> lowest-order bits of \p byte

    \see  #process_bits(unsigned char,#size_type)
 */
inline
void
md5_computer::process_byte( unsigned char byte )
{
    this->process_bits( byte, CHAR_BIT );
}

/** Submits multiple copies of a single byte value for computation.

    \param value       The byte value to be submitted.
    \param byte_count  The number of bytes to submit.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() +
           <var>byte_count</var> * CHAR_BIT</code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           <var>byte_count</var> * CHAR_BIT) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() +
           <var>byte_count</var> * CHAR_BIT &gt;= bits_per_block) ?
           <var>new_value</var> : <var>old_this</var>.last_buffer()</code>  (The
           new value is computed with the algorithm described in RFC 1321,
           section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() +
           <var>byte_count</var> * CHAR_BIT &lt; bits_per_block,</code> then
           <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) ==
           <var>bit_count</var> * CHAR_BIT</code> (assuming that \p o2 is, at
           least, a forward iterator), otherwise the range \p o3 to
           <code>copy_unbuffered(<var>o3</var>)</code> contains
           <code>(<var>old_this</var>.bits_unbuffered() + <var>byte_count</var>
           * CHAR_BIT) % bits_per_block</code> bits from copies of \p value

    \see  #process_byte(unsigned char)
 */
inline
void
md5_computer::process_byte_copies
(
    unsigned char            value,
    md5_computer::size_type  byte_count
)
{
    while ( byte_count-- )
        this->process_byte( value );
}

/** Submits bytes, delimited by a pointer range, for computation.

    \pre  If \p bytes_begin is not \c NULL, then \p bytes_end has to be
          reachable from \p bytes_begin via forward iterations of their
          equivalent <code>unsigned char const *</code> values, otherwise
          \p bytes_end has to be \c NULL too

    \param bytes_begin  The start of the byte range to be submitted.
    \param bytes_end    One-past-the-end of the byte range in \p bytes_begin.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() +
           <var>L</var> * CHAR_BIT</code>, where \p L is number of bytes in the
           range from \p bytes_begin to just before \p bytes_end
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           <var>L</var> * CHAR_BIT) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() +
           <var>L</var> * CHAR_BIT &gt;= bits_per_block) ? <var>new_value</var>
           : <var>old_this</var>.last_buffer()</code>  (The new value is
           computed with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() +
           <var>L</var> * CHAR_BIT &lt; bits_per_block,</code> then
           <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) ==
           <var>L</var> * CHAR_BIT</code> (assuming that \p o2 is, at least, a
           forward iterator), otherwise the range \p o3 to
           <code>copy_unbuffered(<var>o3</var>)</code> contains
           <code>(<var>old_this</var>.bits_unbuffered() + <var>L</var> *
           CHAR_BIT) % bits_per_block</code> trailing bits from the byte range

    \see  #process_byte(unsigned char)
 */
inline
void
md5_computer::process_block( void const *bytes_begin, void const *bytes_end )
{
    BOOST_ASSERT( (bytes_begin == NULL) == (bytes_end == NULL) );
    BOOST_FOREACH( unsigned char b, std::make_pair(static_cast<unsigned char
     const *>( bytes_begin ), static_cast<unsigned char const *>( bytes_end )) )
        this->process_byte( b );
}

/** Submits bytes, bounded by a pointer and length, for computation.

    \pre  If \p buffer is \c NULL, then \p byte_count must be zero

    \param buffer      The start of the byte range to be submitted.
    \param byte_count  Number of (leading) bytes to use from the range.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() +
           <var>byte_count</var> * CHAR_BIT</code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           <var>byte_count</var> * CHAR_BIT) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() +
           <var>byte_count</var> * CHAR_BIT &gt;= bits_per_block) ?
           <var>new_value</var> : <var>old_this</var>.last_buffer()</code>  (The
           new value is computed with the algorithm described in RFC 1321,
           section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() +
           <var>byte_count</var> * CHAR_BIT &lt; bits_per_block,</code> then
           <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) ==
           <var>byte_count</var> * CHAR_BIT</code> (assuming that \p o2 is, at
           least, a forward iterator), otherwise the range \p o3 to
           <code>copy_unbuffered(<var>o3</var>)</code> contains
           <code>(<var>old_this</var>.bits_unbuffered() + <var>byte_count</var>
           * CHAR_BIT) % bits_per_block</code> trailing bits from the buffer

    \see  #process_block(void const*,void const*)
 */
inline
void
md5_computer::process_bytes
(
    void const *             buffer,
    md5_computer::size_type  byte_count
)
{
    BOOST_ASSERT( buffer || !byte_count );
    this->process_block( buffer, static_cast<unsigned char const *>(buffer) +
     byte_count );
}

/** Submits an octet for computation.  The bits are submitted starting from the
    highest-order bit to the lowest.

    \param octet  The octet value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 8</code>
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           8) % bits_per_block</code> 
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() + 8
           &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + 8 &lt;
           bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 8</code>
           (assuming that \p o2 is, at least, a forward iterator), otherwise the
           range \p o3 to <code>copy_unbuffered(<var>o3</var>)</code> contains
           the <code><var>old_this</var>.bits_unbuffered() + 8 -
           bits_per_block</code> lowest-order bits of \p octet

    \see  #process_bits(unsigned char,#size_type)
 */
inline
void
md5_computer::process_octet( uint_least8_t octet )
{
    this->process_bits( octet, 8u );
}

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
md5_computer::process_word( md5_digest::word_type word )
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
md5_computer::process_double_word( md5_computer::length_type dword )
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

    \see  #assign(md5_computer const&)
 */
inline
md5_computer &
md5_computer::operator =( md5_computer const &c )
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
md5_computer::operator ==( md5_computer const &c ) const
{
    // Don't compare the function object proxies since they don't carry
    // significant state.  (Furthermore, they can't change once initalized and
    // don't have any comparison operators.)
    return ( this->length_ == c.length_ ) && ( this->buffer_ == c.buffer_ ) &&
     std::equal( this->unbuffered_.begin(), this->unbuffered_.begin() +
     this->bits_unbuffered(), c.unbuffered_.begin() );
}

/** Compares MD5 message computers for non-equivalence.  Such computers are
    unequal if at least one set of corresponding parts of their significant
    state (data length, running hash, and bit queue) are unequal.

    \param c  The right-side operand to be compared.

    \retval true   \c *this and \p c are not equivalent.
    \retval false  \c *this and \p c are equivalent.

    \see  #operator==(md5_computer const&)const
 */
inline
bool
md5_computer::operator !=( md5_computer const &c ) const
{
    return !this->operator ==( c );
}

/** Computes the check-sum of the submitted data, through a standard generator
    interface.

    \return  The generated check-sum.

    \see  #checksum()const
 */
inline
md5_computer::value_type
md5_computer::operator ()() const
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
