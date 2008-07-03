// Boost coding/operations.hpp header file  ----------------------------------//

// (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for documentation.

/** \file
    \brief  Definitions of coding helper base class templates

    Contains the definition of templates that can be used for base classes of
    bit- and byte-encoding computer types.
 */

#ifndef BOOST_CODING_OPERATIONS_HPP
#define BOOST_CODING_OPERATIONS_HPP

#include <boost/coding_fwd.hpp>

#include <boost/array.hpp>           // for boost::array
#include <boost/assert.hpp>          // for BOOST_ASSERT
#include <boost/concept/assert.hpp>  // for BOOST_CONCEPT_ASSERT
#include <boost/concept_check.hpp>   // for boost::OutputIterator
#include <boost/cstdint.hpp>         // for boost::uint_least8_t
#include <boost/static_assert.hpp>   // for BOOST_STATIC_ASSERT
#include <boost/type_traits.hpp>     // for boost::is_class, is_integral, etc.

#include <algorithm>  // for std::copy, swap, equal
#include <climits>    // for CHAR_BIT


namespace boost
{
namespace coding
{


//  Byte-processing base class template declaration  -------------------------//

/** \brief  Base class template for byte-processing operations

    This template is intended to base a class using the curiously recurring
    template pattern to gain completed definitions of common byte-processing
    operations.

    \pre  \p Derived is a class or class template and it must have, either
          directly or through an intermediate base class, an unambiguous
          non-static member function named \c process_byte that returns \c void
          and takes one argument of type <code>unsigned char</code>.  (A
          different, yet compatible, signature is acceptable.)
    \pre  \p SizeType is a built-in unsigned integral type.

    \tparam Derived   The final operand type that this class template will form
                      a base.  Any intermediate helper class templates should
                      pass \p Derived on to \c byte_processing_base.
    \tparam SizeType  The type used for size parameters.  If not given,
                      \c std::size_t is the default.

    \todo  Using #bytes always changes \c *this.  Is there some way to hold off
           updates until explicit assignment?
 */
template < class Derived, typename SizeType >
class byte_processing_base
{
    // Parameter checks (can't check for the existence of "process_byte")
    BOOST_STATIC_ASSERT( is_class<Derived>::value );
    BOOST_STATIC_ASSERT( is_integral<SizeType>::value &&
     is_unsigned<SizeType>::value );

    // Byte-wise application proxy class
    class byte_applicator
    {
        friend class byte_processing_base;
        Derived &  source_;
        explicit  byte_applicator( Derived &source )
          : source_( source )  {}
    public:
        void  operator ()( unsigned char byte )
          { this->source_.process_byte(byte); }
        byte_applicator &  operator =( byte_applicator const &c )
          { this->source_ = c.source_; return *this; }
    };  // byte_applicator

public:
    // Special application interface
    /** \brief  Proxy for byte-oriented application interface

        Accesses an interface where <code>*this</code> can be used as a function
        object take can take a single <code>unsigned char</code> value as input.
        It cannot be reseated from <code>*this</code>, but assignment to it will
        copy the other's owner's state to <code>*this</code>, enabling
        algorithms that return updated function objects to work.

        \attention  Since #byte_applicator stores a non-constant reference to
                    its owner, the owner will be change through any application
                    through \c bytes, even if \c bytes is passed by value.
     */
    byte_applicator  bytes;

    /*! \name Byte-stream reading */ //@{
    // Types
    /** \brief  Type of size values

        Represents the type used for sizing parameters.
     */
    typedef SizeType  size_type;

    // Input processing
    //! Enters several bytes, all of the same value
    void  process_byte_copies( unsigned char value, size_type byte_count );

    //! Enters a range of bytes in memory
    void  process_block( void const *bytes_begin, void const *bytes_end );
    //! Enters a byte buffer in memory
    void  process_bytes( void const *buffer, size_type byte_count );//@}

    /*! \name Operators */ //@{
    // Operators
    //! Copy-assignment
    byte_processing_base &  operator =( byte_processing_base const & );//@}

protected:
    // Implementation types
    /** \brief  Final operand type

        Represents the type that should ultimately inherit from this template.
     */
    typedef Derived  derived_type;

    // Lifetime management (use automatic destructor)
    //! Default construction
    byte_processing_base();
    //! Copy construction
    byte_processing_base( byte_processing_base const & );

};  // byte_processing_base


//  Bit-processing base class template declarations  -------------------------//

/** \brief  Base class template for bit-processing operations, big endian

    This template is intended to base a class using the curiously recurring
    template pattern to gain completed definitions of common bit-processing
    operations.  Bits within a byte are processed in order starting from the
    most-significant places.

    \pre  \p Derived is a class or class template and it must have, either
          directly or through an intermediate base class, an unambiguous
          non-static member function named \c process_bit that returns \c void
          and takes a single \c bool argument.  (A different, yet compatible,
          signature is acceptable.)
    \pre  \p SizeType is a built-in unsigned integral type.

    \tparam Derived   The final operand type that this class template will form
                      a base.  Any intermediate helper class templates should
                      pass \p Derived on to \c bit_processing_base.
    \tparam SizeType  The type used for size parameters.  If not given,
                      \c std::size_t is the default.

    \see  boost::coding::byte_processing_base

    \todo  Using #bits always changes \c *this.  Is there some way to hold off
           updates until explicit assignment?
 */
template < class Derived, typename SizeType >
class bit_processing_b_base
    : public byte_processing_base<Derived, SizeType>
{
    typedef byte_processing_base<Derived, SizeType>  base_type;

    // The parameter checks that the base class uses are acceptable.  (Except
    // that we can't check for the existence of "process_bit".)

    // Bit-wise application proxy class
    class bit_applicator
    {
        friend class bit_processing_b_base;
        Derived &  source_;
        explicit  bit_applicator( Derived &source )
          : source_( source )  {}
    public:
        void  operator ()( bool bit )
          { this->source_.process_bit(bit); }
        bit_applicator &  operator =( bit_applicator const &c )
          { this->source_ = c.source_; return *this; }
    };  // bit_applicator

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
    bit_applicator  bits;

    /*! \name Bit-stream reading */ //@{
    // Types
    /** \brief  Type of size values

        Represents the type used for sizing parameters.
     */
    typedef typename base_type::size_type  size_type;

    // Input processing
    //! Enters lowest-signficant part of a byte, relatively high-order bit first
    void  process_bits( unsigned char bits, size_type bit_count );
    //! Enters several bits, all of the same value
    void  process_bit_copies( bool value, size_type bit_count );

    //! Enters a whole byte, high-order bit first
    void  process_byte( unsigned char byte );

    //! Enters an octet, highest bit first
    void  process_octet( uint_least8_t octet );//@}

    /*! \name Operators */ //@{
    // Operators
    //! Copy-assignment
    bit_processing_b_base &  operator =( bit_processing_b_base const &c );//@}

protected:
    // Implementation types
    /** \brief  Final operand type

        Represents the type that should ultimately inherit from this template.
     */
    typedef typename base_type::derived_type  derived_type;

    // Lifetime management (use automatic destructor)
    //! Default construction
    bit_processing_b_base();
    //! Copy construction
    bit_processing_b_base( bit_processing_b_base const &c );

};  // bit_processing_b_base


//  Queued bit-processing base class template declarations  ------------------//

/** \brief  Base class template for queued bit-processing operations

    This template is intended to base a class using the curiously recurring
    template pattern to gain a definition of a bit-queueing operation, plus some
    inspectors on the bit queue.  Bits, either individually or within a byte,
    are processed in submission order, but in fixed-size runs.  Bits are kept in
    a queue until enough bits have been saved, then a full bit pack is processed
    in one shot.

    \pre  \p Derived is a class or class template and it must have, either
          directly or through an intermediate base class, an unambiguous
          non-static member function named \c update_hash that returns nothing
          (i.e. \c void) but takes two <code>bool const *</code> bounding an
          array segment.  (A different, yet compatible, signature is
          acceptable.)
    \pre  \p LengthType is a built-in unsigned integral type.
    \pre  <code><var>QueueLength</var> &gt;= 2</code>.  (If \p QueueLength is
          less than two, then \c update_hash will be called after \e every
          submission, which would make having a queue pointless.)
    \pre  \p SizeType is a built-in unsigned integral type.

    \tparam Derived      The final operand type that this class template will
                         form a base.  Any intermediate helper class templates
                         should pass \p Derived on to
                         \c queued_bit_processing_b_base.
    \tparam LengthType   The type used for queue-size parameters.
    \tparam QueueLength  The number of processed bits to queue between hashings.
    \tparam SizeType     The type used for processing-size parameters.  If not
                         given, \c std::size_t is the default.

    \see  boost::coding::bit_processing_b_base
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
class queued_bit_processing_base
    : public bit_processing_b_base<Derived, SizeType>
{
    typedef bit_processing_b_base<Derived, SizeType>  base_type;

    // Parameter checks (some template arguments are checked by the base class
    // template; can't check for the existence of "update_hash".)
    BOOST_STATIC_ASSERT( is_integral<LengthType>::value &&
     is_unsigned<LengthType>::value );
    BOOST_STATIC_ASSERT( QueueLength >= 2u );

public:
    /*! \name Inspection */ //@{
    // Types
    /** \brief  Type of length values

        Represents the type used for processing- and queueing-size results.
     */
    typedef LengthType  length_type;

    // Constants
    //! Number of bits in the hash queue
    static  length_type const  queue_length = QueueLength;

    // Inspectors
    //! Returns the count of bits read so far, hashed and queued
    length_type  bits_read() const;
    //! Returns the count of the queued bits
    length_type  bits_unbuffered() const;

    //! Copies out the queued bits
    template < typename OutputIterator >
     OutputIterator  copy_unbuffered( OutputIterator o ) const;//@}

    /*! \name Bit-stream reading */ //@{
    // More types
    /** \brief  Type of size values

        Represents the type used for reading-size parameters.
     */
    typedef typename base_type::size_type  size_type;

    // Input processing
    //! Enters one bit for hashing
    void  process_bit( bool bit );//@}

    /*! \name Operators */ //@{
    // Operators (use automatic copy-assignment operator)
    //! Equals
    bool  operator ==( queued_bit_processing_base const &c ) const;
    //! Not-equals
    bool  operator !=( queued_bit_processing_base const &c ) const;//@}

protected:
    // Implementation types
    /** \brief  Final operand type

        Represents the type that should ultimately inherit from this template.
     */
    typedef typename base_type::derived_type  derived_type;

    // Lifetime management (use automatic copy constructor and destructor)
    //! Default construction
    queued_bit_processing_base();

    /*! \name Assignment */ //@{
    // Assignment
    //! Sets state back to initial conditions
    void  reset();
    //! Changes the current state to a copy of another object's
    void  assign( queued_bit_processing_base const &c );

    //! Exchanges state with another object
    void  swap( queued_bit_processing_base &other );//@}

private:
    // More implementation types
    typedef array<bool, queue_length>  queue_type;

    // Member data
    length_type  bit_count;
    queue_type   queued_bits;

};  // queued_bit_processing_base


//  Byte-processing base class template member definitions  ------------------//

/** Constructs a byte processor set to initial conditions.

    \post  \c #bytes points to \c *this
 */
template < class Derived, typename SizeType >
inline
byte_processing_base<Derived, SizeType>::byte_processing_base()
    : bytes( *static_cast<derived_type *>(this) )
{
}

/** Constructs a byte processor to the same significant state as another.

    \post  \c #bytes points to \c *this
 */
template < class Derived, typename SizeType >
inline
byte_processing_base<Derived, SizeType>::byte_processing_base
(
    byte_processing_base const &  // unused
)
    : bytes( *static_cast<derived_type *>(this) )
{
}

/** Submits multiple copies of a single byte value for processing.

    \param value       The byte value to be submitted.
    \param byte_count  The number of bytes to submit.

    \post  This object's \c process_byte member function has been called
           \p byte_count times with \p value as the parameter.
 */
template < class Derived, typename SizeType >
inline
void
byte_processing_base<Derived, SizeType>::process_byte_copies
(
    unsigned char  value,
    size_type      byte_count
)
{
    while ( byte_count-- )
        this->bytes( value );
}

/** Submits bytes, delimited by a pointer range, for processing.

    \pre  If \p bytes_begin is not \c NULL, then \p bytes_end has to be
          reachable from \p bytes_begin via forward iterations of their
          equivalent <code>unsigned char const *</code> values, otherwise
          \p bytes_end has to be \c NULL too.

    \param bytes_begin  The start of the byte range to be submitted.
    \param bytes_end    One-past-the-end of the byte range in \p bytes_begin.

    \post  This object's \c process_byte member function has been called for
           each byte in the given range, starting from the one at the address
           \p bytes_begin to the byte just before the \p bytes_end mark.
 */
template < class Derived, typename SizeType >
inline
void
byte_processing_base<Derived, SizeType>::process_block
(
    void const *  bytes_begin,
    void const *  bytes_end
)
{
    // Parameter check (can't check if bytes_end is reachable from bytes_begin)
    BOOST_ASSERT( !bytes_begin == !bytes_end );

    for ( unsigned char const *  p =
     static_cast<unsigned char const *>(bytes_begin) ; p != bytes_end ; ++p )
        this->bytes( *p );
}

/** Submits bytes, bounded by a pointer and length, for processing.

    \pre  If \p buffer is \c NULL, then \p byte_count must be zero.

    \param buffer      The start of the byte range to be submitted.
    \param byte_count  Number of (leading) bytes to use from the range.

    \post  This object's \c process_byte member function has been called for
           each byte in the given range, starting from the one at the address
           \p buffer, going forward for \p byte_count bytes total.
 */
template < class Derived, typename SizeType >
inline
void
byte_processing_base<Derived, SizeType>::process_bytes
(
    void const *  buffer,
    size_type     byte_count
)
{
    // Parameter check
    BOOST_ASSERT( buffer || !byte_count );

    for ( unsigned char const *  p = static_cast<unsigned char const *>(buffer)
     ; byte_count-- ; ++p )
        this->bytes( *p );
}

/** Changes a byte processor to have the same observable state as a given
    processor.  (The function object proxy isn't reseated, however.)

    \return  \c *this

    \post  \c #bytes \e still points to \c *this
 */
template < class Derived, typename SizeType >
inline
byte_processing_base<Derived, SizeType> &
byte_processing_base<Derived, SizeType>::operator =
(
    byte_processing_base const &  // unused
)
{
    // Do nothing, so "bytes" can't be reseated
    return *this;
}


//  Bit-processing base class template member definitions  -------------------//

/** Constructs a bit processor set to initial conditions.

    \post  \c #bits points to \c *this
 */
template < class Derived, typename SizeType >
inline
bit_processing_b_base<Derived, SizeType>::bit_processing_b_base()
    : base_type()
    , bits( *static_cast<derived_type *>(this) )
{
}

/** Constructs a bit processor to the same computation state as \p c.

    \param c  The original object to be copied.

    \post  \c #bits points to \c *this
 */
template < class Derived, typename SizeType >
inline
bit_processing_b_base<Derived, SizeType>::bit_processing_b_base
(
    bit_processing_b_base const &  c
)
    : base_type( c )
    , bits( *static_cast<derived_type *>(this) )
{
}

/** Submits part of a byte for processing.  The lowest-order bits are submitted
    starting from the relatively highest-order bit to the lowest.

    \pre  <code>0 &lt;= <var>bit_count</var> &lt;= CHAR_BIT</code>.

    \param bits       The byte from which the values are submitted.
    \param bit_count  The number of bits to submit.

    \post  This object's \c process_bit member function has been called for
           each qualifying bit within \p bits, starting from the bit at the
           2<sup><var>bit_count</var> - 1</sup>-place, going down to the
           ones-place.  (If \p bit_count is zero, no calls are made.)
 */
template < class Derived, typename SizeType >
inline
void
bit_processing_b_base<Derived, SizeType>::process_bits
(
    unsigned char  bits,
    size_type      bit_count
)
{
    // Parameter check
    BOOST_ASSERT( (0u <= bit_count) && (bit_count <= CHAR_BIT) );

    while ( bit_count-- )
        this->bits( bits & (0x01u << bit_count) );
}

/** Submits multiple copies of a single bit value for processing.

    \param value      The bit value to be submitted.
    \param bit_count  The number of bits to submit.

    \post  This object's \c process_bit member function has been called
           \p bit_count times with \p value as the parameter.
 */
template < class Derived, typename SizeType >
inline
void
bit_processing_b_base<Derived, SizeType>::process_bit_copies
(
    bool       value,
    size_type  bit_count
)
{
    while ( bit_count-- )
        this->bits( value );
}

/** Submits a byte for processing.  The bits are submitted starting from the
    highest-order bit to the lowest.  Note that this member function enables the
    inherited input-processing member functions \c #process_byte_copies,
    \c #process_block, and \c #process_bytes.

    \param byte  The byte value to be submitted.

    \post  This object's \c process_bit member function has been called for
           each bit in the given \p byte, starting from the high-order bit,
           going down to the low-order bit.

    \see  #process_bits(unsigned char,#size_type)
 */
template < class Derived, typename SizeType >
inline
void
bit_processing_b_base<Derived, SizeType>::process_byte( unsigned char byte )
{
    this->process_bits( byte, CHAR_BIT );
}

/** Submits an octet for processing.  The bits are submitted starting from the
    highest-order bit to the lowest.  Note that a byte may be larger than eight
    bits, so the lowest-significant bits within the containing byte (or larger
    register) are chosen.  This way, you don't have to shift up your octet value
    by <code>sizeof(<var>octet</var>) * CHAR_BIT - 8</code> (or whatever) bits.

    \param octet  The octet value to be submitted.

    \post  This object's \c process_bit member function has been called for
           each qualifying bit in \p octet, starting from the bit at the
           128s-place, going down to the ones-place.

    \see  #process_bits(unsigned char,#size_type)
 */
template < class Derived, typename SizeType >
inline
void
bit_processing_b_base<Derived, SizeType>::process_octet( uint_least8_t octet )
{
    // CHAR_BIT >= 8 (C-1999, section 5.2.4.2.1) -> all octet values can safely
    // be converted to "unsigned char," no matter what "uint_least8_t" is.
    this->process_bits( octet, 8u );
}

/** Changes a bit processor to have the same observable state as a given
    processor.  (No function object proxies are reseated, however.)

    \param c  The source object with the new state.

    \return  \c *this

    \post  \c #bits \e still points to \c *this
 */
template < class Derived, typename SizeType >
inline
bit_processing_b_base<Derived, SizeType> &
bit_processing_b_base<Derived, SizeType>::operator =
(
    bit_processing_b_base const &  c
)
{
    // Do nothing, so "bits" can't be reseated
    this->base_type::operator =( c );
    return *this;
}


//  Queued bit-processing base class template member definitions  ------------//

/** Represents the number of submitted bits that are queued until that queue
    is emptied as a single block to be hashed.  The count of unhashed bits
    is always less than this value.  (The input processing member functions,
    like \c #process_bit, trigger \c update_hash right after a bit fills the
    queue.)
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
typename queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::length_type const  queued_bit_processing_base<Derived, LengthType,
 QueueLength, SizeType>::queue_length;

/** Constructs an empty processing queue.

    \post  <code>#bits_read() == 0</code>.
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused.
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::queued_bit_processing_base()
    : bit_count(), queued_bits()
{
}

/** Returns the number of bits that have been processed, both those that have
    been hashed and those that are on queue.  (Beware if the number of
    submissions has exceeded <code>std::numeric_limits&lt; #length_type
    &gt;::max()</code>.)

    \return  How many bits have been submitted, hashed and queued.
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
typename queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::length_type
queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::bits_read() const
{
    return this->bit_count;
}

/** Returns the number of bits that are still in the queue, unhashed.  Hashing
    occurs only after every \c #queue_length bit submissions, so this member
    function can confirm queued stragglers. (The identities of hashed bits are
    not retrievable.)

    \return  How many bits are queued to be hashed.

    \see  #bits_read()
    \see  #queue_length
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
typename queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::length_type
queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::bits_unbuffered() const
{
    return this->bit_count % queue_length;
}

/** Copies the last submitted bits that have not yet hashed starting from the
    oldest submission.  Use \c #bits_unbuffered()  for advance notice of how
    many iterations are done.  (Always less than \c #queue_length elements are
    copied.)

    \pre  At least \c #bits_unbuffered() more elements are free to be created
          and/or assigned through \p o.

    \tparam OutputIterator  The type of the iterator submitted.  It should match
                            the requirements of either an output or a forward
                            (or above) mutable iterator over something that can
                            receive \c bool values via dereferenced assignment.

    \param o  The iterator starting the destination range.

    \return  \p o after copying.

    \see  #bits_unbuffered()
    \see  #queue_length
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
template < typename OutputIterator >
inline
OutputIterator
queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::copy_unbuffered( OutputIterator o ) const
{
    // Parameter check
    BOOST_CONCEPT_ASSERT( (boost::OutputIterator<OutputIterator, bool>) );

    return std::copy( this->queued_bits.begin(), this->queued_bits.begin() +
     this->bits_unbuffered(), o );
}

/** Changes an object to be like it was default-constructed.

    \post  <code>#bits_read() == 0</code>.
    \post  <code>#copy_unbuffered(<var>o</var>)</code> leaves \p o unused.

    \see  #queued_bit_processing_base()
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
void
queued_bit_processing_base<Derived, LengthType, QueueLength, SizeType>::reset()
{
    // With this setting, any used bits in "queued_bits" will be ignored.
    this->bit_count = 0u;
}

/** Changes an object to be like the given object.

    \param c  The object with the new state to be copied.

    \post  <code>*this == <var>c</var></code>.
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
void
queued_bit_processing_base<Derived, LengthType, QueueLength, SizeType>::assign(
 queued_bit_processing_base const &c )
{
    // Just rip off the automatically-defined copy-assignment operator
    *this = c;
}

/** Swaps the content of this object with another.

    \param other  The other object to trade state with this object.

    \post  <code>*this == <var>old_other</var> &amp;&amp; <var>old_this</var> ==
           <var>other</var></code>.
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
void
queued_bit_processing_base<Derived, LengthType, QueueLength, SizeType>::swap(
 queued_bit_processing_base &other )
{
    // Use the appropriate swap via Koeing look-up
    using std::swap;

    // Swap members
    swap( this->bit_count, other.bit_count );
    swap( this->queued_bits, other.queued_bits );
}

/** Submits a single bit to the processing queue.  If this bit fills the queue,
    the queue's bits are hashed and the queue is emptied.

    \param bit  The bit value to be submitted.

    \post  <code>#bits_read() == <var>old_this</var>.bits_read() + 1</code>.
    \post  <code>#bits_unbuffered() == bits_read() % #queue_length ? 0 :
           <var>old_this</var>.bits_unbuffered() + 1</code>.
    \post  This object's \c update_hash member function has been called,
           accepting all the bits from the queue in submission order.
    \post  If <code>bits_read() % queue_length == 0</code>, then
           <code>#copy_unbuffered(<var>o1</var>)</code> leaves \p o1 unused,
           otherwise <code>std::distance(
           <var>old_this</var>.copy_unbuffered(<var>o2</var>),
           copy_unbuffered(<var>o2</var>) ) == 1</code> (assuming that \p o2 is,
           at least, a forward iterator).
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
void
queued_bit_processing_base<Derived, LengthType, QueueLength,
 SizeType>::process_bit( bool bit )
{
    this->queued_bits[ this->bits_unbuffered() ] = bit;

    if ( (++this->bit_count % queue_length) == 0u )
    {
        static_cast<derived_type *>(this)->update_hash(
         this->queued_bits.begin(), this->queued_bits.end() );
    }
}

/** Compares bit processing queues for equivalence.  Such processing queues are
    equal if all of the corresponding parts of their significant state (data
    length and bit queue) are equal.  In other words, if \c *this and \p c are
    equivalent, then:
    - <code>#bits_read() == <var>c</var>.bits_read()</code>.
    - Given two empty containers \p x1 and \p x2 of the same standard-acting
      type that holds \c bool elements and has the \c push_back member function,
      after applying <code>#copy_unbuffered( std::back_inserter(<var>x1</var>)
      )</code> and <code><var>c</var>.copy_unbuffered(
      std::back_inserter(<var>x2</var>) )</code>, <code><var>x1</var> ==
      <var>x2</var></code>.

    \param c  The right-side operand to be compared.

    \retval true   \c *this and \p c are equivalent.
    \retval false  \c *this and \p c are not equivalent.

    \see  #bits_read()
    \see  #copy_unbuffered(OutputIterator)
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
bool
queued_bit_processing_base<Derived, LengthType, QueueLength, SizeType>::operator
 ==( queued_bit_processing_base const &c ) const
{
    return ( this->bit_count == c.bit_count ) && std::equal(
     this->queued_bits.begin(), this->queued_bits.begin() +
     this->bits_unbuffered(), c.queued_bits.begin() );
}

/** Compares bit processing queues for non-equivalence.  Such processing queues
    are unequal if at least one set of corresponding parts of their significant
    state (data length or bit queue) are unequal.

    \param c  The right-side operand to be compared.

    \retval true   \c *this and \p c are not equivalent.
    \retval false  \c *this and \p c are equivalent.

    \see  #operator==(queued_bit_processing_base const&)const
 */
template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType >
inline
bool
queued_bit_processing_base<Derived, LengthType, QueueLength, SizeType>::operator
 !=( queued_bit_processing_base const &c ) const
{
    return !this->operator ==( c );
}


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_OPERATIONS_HPP
