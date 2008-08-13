// Boost coding/md5_computer.hpp header file  --------------------------------//
/** \file
    \brief  Declaration of MD5 computation object.

    \author  Daryle Walker

    Contains the declaration of the presentation type used for the computing of
    MD5 message digests.  Non-inline items declared here are defined in
    &quot;$BOOST_ROOT/libs/coding/src/md5.cpp&quot;.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_MD5_COMPUTER_HPP
#define BOOST_CODING_MD5_COMPUTER_HPP

#include <boost/coding_fwd.hpp>
#include <boost/coding/md5_digest.hpp>   // for boost::coding::md5_digest
#include <boost/coding/md5_context.hpp>  // for boost::coding::md5_context

#include <boost/array.hpp>                 // for boost::array
#include <boost/coding/coding_shell.hpp>   // for boost:coding:bit_coding_shell
#include <boost/concept/assert.hpp>        // for BOOST_CONCEPT_ASSERT
#include <boost/concept_check.hpp>         // for boost::OutputIterator
#include <boost/cstdint.hpp>               // for boost::uint_least8_t
#include <boost/integer/integer_mask.hpp>  // for boost::integer_lo_mask
#include <boost/serialization/access.hpp>  // for boost::serialization::access
#include <boost/serialization/nvp.hpp>     // for boost::serialization::make_nvp

#include <algorithm>  // for std::copy, swap


namespace boost
{
namespace coding
{


//  MD5 message-digest computation class declaration  ------------------------//

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
 */
class md5_computer
    : public bit_coding_shell<md5_context, true>
{
    typedef bit_coding_shell<md5_context, true>  base_type;
    typedef md5_computer                         self_type;

public:
    // Types
    /** \brief  Type of MD message lengths

        Represents the type needed to store the significant count of bits read.
     */
    typedef md5_context::length_type  length_type;
    /** \brief  Type of MD buffers

        Represents the intermediate MD buffer, holding the checksum for all
        prior \e completed hashed blocks.  The zero-index corresponds to the "A"
        register, up to index 3 representing the "D" register.
     */
    typedef array<md5_digest::word_type, md5_digest::words_per_digest::value>
      buffer_type;

    /** \brief  Type of hashing sine tables

        Represents the storage for the hashing constants, based off the sine
        function.
     */
    typedef array<md5_digest::word_type,
     md5_context::hash_table_type::static_size>  hash_table_type;

    /** \brief  Number of bits in hash queue, as an integer meta-constant

        Represents the number of submitted bits that are queued until that queue
        is emptied as a single block to be hashed, implied from RFC 1321,
        section 3.4.  The count of unhashed bits is always less than this value.
        (The input processing member functions trigger a hash right after a bit
        fills the queue.)
     */
    typedef md5_context::bits_per_block   bits_per_block;
    /** \brief  Number of bits for length quantities, as integer meta-constant

        Represents the number of significant (low-order) bits kept for the
        message length, which can also be processed as two words, all as given
        in RFC 1321, section 3.2, paragraph 1.
     */
    typedef md5_context::bits_per_length  significant_bits_per_length;

    // Constants
    //! Hashing sine table
    static  hash_table_type const  hashing_table;

    // Inspectors
    //! Returns the count of bits read so far
    length_type  bits_read() const;
    //! Returns the checksum buffer of hashed bits
    buffer_type  last_buffer() const;
    //! Returns the count of the queued bits
    length_type  bits_unbuffered() const;

    //! Copies out the queued bits
    template < typename OutputIterator >
    OutputIterator  copy_unbuffered( OutputIterator o ) const;

    // Assignment
    //! Sets state back to initial conditions
    void  reset();
    //! Changes the current state to a copy of another object's
    void  assign( self_type const &c );

    //! Exchanges state with another object
    void  swap( self_type &other );

    // Input processing
    //! Enters an octet
    void  process_octet( uint_least8_t octet );
    //! Enters a word for hashing
    void  process_word( md5_digest::word_type word );
    //! Enters a double-word for hashing
    void  process_double_word( length_type dword );

    // Extras
    //! Creates copy of \c #hashing_table using calculated, not static, values
    static  hash_table_type  generate_hashing_table();

private:
    // Serialization
    friend class boost::serialization::access;

    //! Enables persistence with Boost.Serialization-compatible archives
    template < class Archive >
    void  serialize( Archive &ar, const unsigned int version );

};  // md5_computer


//  MD5 message-digest computation inspector member function definitions  ----//

/** Returns the number of bits that have been processed, both those that have
    been hashed and those that are on queue.  Only the lowest-order 64 bits of
    count are significant and kept/reliable.

    \return  How many bits have been submitted, hashed and queued.
 */
inline md5_computer::length_type
md5_computer::bits_read() const
{
    return this->context().length &
     boost::integer_lo_mask<md5_context::bits_per_length::value>::value;
}

/** Returns the checksum of all the bits that have been hashed so far.  Hashing
    occurs only after every <code>#bits_per_block :: value</code> bit entries,
    so check \c #bits_unbuffered() for any queued stragglers.

    \return  The current state of the MD buffer, not counting any unhashed bits.
 */
inline md5_computer::buffer_type
md5_computer::last_buffer() const  { return this->context().buffer; }

/** Returns the number of bits that are still in the queue, unhashed.  Hashing
    occurs only after every <code>#bits_per_block :: value</code> bit
    submissions, so this member function can confirm queued stragglers. (The
    identities of hashed bits are not retrievable.)

    \return  How many bits are queued to be hashed.

    \see  #bits_read()
    \see  #bits_per_block
 */
inline md5_computer::length_type
md5_computer::bits_unbuffered() const
{ return this->bits_read() % md5_context::bits_per_block::value; }

/** Copies the last submitted bits that have not yet been hashed starting from
    the oldest submission.  Use \c #bits_unbuffered() for advance notice of how
    many iterations are done.  (Always less than <code>#bits_per_block ::
    value</code> elements are copied.)

    \pre  At least \c #bits_unbuffered() more elements are free to be created
          and/or assigned through \p o.

    \tparam OutputIterator  The type of the iterator submitted.  It should match
                            the requirements of either an output or a forward
                            (or above) mutable iterator over something that can
                            receive \c bool values via dereferenced assignment.

    \param o  The iterator starting the destination range.

    \return  \p o after the copying.

    \see  #bits_unbuffered()
    \see  #bits_per_block
 */
template < typename OutputIterator >
inline OutputIterator
md5_computer::copy_unbuffered( OutputIterator o ) const
{
    // Parameter check
    BOOST_CONCEPT_ASSERT( (boost::OutputIterator<OutputIterator, bool>) );

    return std::copy( this->context().queue.begin(),
     this->context().queue.begin() + this->bits_unbuffered(), o );
}


//  MD5 message-digest computation assignment member function definitions  ---//

/** Changes an object to be like it was default-constructed.

    \post  <code>#bits_read() == 0</code>.
    \post  <code>#last_buffer() == { 0x67452301, 0xEFCDAB89, 0x98BACDFE,
           0x10325476 }</code>.
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused.
 */
inline void  md5_computer::reset()  { *this = self_type(); }

/** Changes an object to be like the given object.  Only the computation
    elements are copied; no function object proxies are reseated.

    \param c  The object with the new state to be copied.

    \post  <code>#bits_read() == <var>c</var>.bits_read()</code>.
    \post  <code>#last_buffer() == <var>c</var>.last_buffer()</code>.
    \post  Given <code>#copy_unbuffered(<var>o1</var>)</code> and
           <code><var>c</var>.copy_unbuffered(<var>o2</var>)</code>, where both
           \p o1 and \p o2 are forward iterators to different container segments
           that each have at least <code>#bits_unbuffered()</code> elements
           available, <code>std::equal( o1, o1 + bits_unbuffered(), o2 ) ==
           true</code>.
    \post  \c #bits and \c #bytes \e still point to \c *this
 */
inline void  md5_computer::assign( self_type const &c )  { *this = c; }

/** Swaps the content of this object with another.  Only the computation
    elements are changed; no function object proxies are reseated.

    \param other  The other object to trade state with this object.

    \post  <code>*this == <var>old_other</var> &amp;&amp; <var>old_this</var> ==
           <var>other</var></code>.
 */
inline void  md5_computer::swap( self_type &other ) { std::swap(*this, other); }


//  MD5 message-digest computation bit-input member function definitions  ----//

/** Submits an octet for computation.  The bits are submitted starting from the
    highest-order bit to the lowest.  Note that a byte may be larger than eight
    bits, so the lowest-significant bits within the containing byte (or larger
    register) are chosen.  This way, you don't have to shift up your octet value
    by <code>sizeof(<var>octet</var>) * CHAR_BIT - 8</code> (or whatever) bits.

    \param octet  The octet value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 8</code>.
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           8) % bits_per_block</code>.
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() + 8
           &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>.  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + 8 &lt;
           bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 8</code>
           (assuming that \p o2 is, at least, a forward iterator), otherwise the
           range \p o3 to <code>copy_unbuffered(<var>o3</var>)</code> contains
           the <code><var>old_this</var>.bits_unbuffered() + 8 -
           bits_per_block</code> lowest-order bits of \p octet.

    \see  boost::coding::bit_coding_shell<>::process_bits(unsigned char,size_type)
 */
inline void
md5_computer::process_octet( uint_least8_t octet )
{ this->context().consume_octet( octet ); }

/** Submits 32-bit MD-word for computation.  The word is submitted an octet at a
    time, from the lowest-order octet to the highest.

    \param word  The word value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 32</code>.
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           32) % bits_per_block</code>.
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() + 32
           &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>.  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + 32 &lt;
           bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 32</code>
           (assuming that \p o2 is, at least, a forward iterator), otherwise the
           range \p o3 to <code>copy_unbuffered(<var>o3</var>)</code> contains
           the <code><var>old_this</var>.bits_unbuffered() + 32 -
           bits_per_block</code> lowest-order bits of \p word.

    \see  #process_octet(uint_least8_t)
 */
inline void
md5_computer::process_word( md5_digest::word_type word )
{ this->context().consume_word( word ); }

/** Submits 64-bit MD-length for computation.  The double-word is submitted a
    word at a time, first the low-order word then the high-order word.

    \param dword  The double-word value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 64</code>.
    \post  <code>#bits_unbuffered() == (<var>old_this</var>.bits_unbuffered() +
           64) % bits_per_block</code>.
    \post  <code>#last_buffer() == (<var>old_this</var>.bits_unbuffered() + 64
           &gt;= bits_per_block) ? <var>new_value</var> :
           <var>old_this</var>.last_buffer()</code>.  (The new value is computed
           with the algorithm described in RFC 1321, section 3.4.)
    \post  If <code>bits_read() % bits_per_block == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise if <code><var>old_this</var>.bits_unbuffered() + 64 &lt;
           bits_per_block,</code> then <code>copy_unbuffered(<var>o2</var>) -
           <var>old_this</var>.copy_unbuffered(<var>o2</var>) == 64</code>
           (assuming that \p o2 is, at least, a forward iterator), otherwise the
           range \p o3 to <code>copy_unbuffered(<var>o3</var>)</code> contains
           the <code><var>old_this</var>.bits_unbuffered() + 64 -
           bits_per_block</code> lowest-order bits of \p dword.

    \see  #process_word(boost::coding::md5_digest::word_type)
 */
inline void
md5_computer::process_double_word( length_type dword )
{ this->context().consume_dword( dword ); }


//  MD5 message-digest computation serialization function definition  --------//

/** Streams a computer to/from an archive using the Boost.Serialization
    protocols.  This member function is meant to be called only by the
    Boost.Serialization system, as needed.

    \tparam  Archive  The type of \p ar.  It must conform to the requirements
                      Boost.Serialization expects of archiving classes.

    \param  ar       The archiving object that this object's representation will
                     be streamed to/from.
    \param  version  The version of the persistence format for this object.  (It
                     should be zero, since this type just got created.)
 */
template < class Archive >
inline void
md5_computer::serialize( Archive &ar, const unsigned int version )
{ ar & boost::serialization::make_nvp( "context", this->context() ); }


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
inline void  swap( md5_computer &a, md5_computer &b )  { a.swap( b ); }


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_MD5_COMPUTER_HPP
