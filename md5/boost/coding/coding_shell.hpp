// Boost coding/coding_shell.hpp header file  --------------------------------//
/** \file
    \brief  Definitions of the coding shell helper class templates.

    \author  Daryle Walker

    Contains the definition of templates that encapsulate bit- and byte-encoding
    computer types, given a coding kernel.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_CODING_SHELL_HPP
#define BOOST_CODING_CODING_SHELL_HPP

#include <boost/coding_fwd.hpp>

#include <boost/assert.hpp>         // for BOOST_ASSERT
#include <boost/mpl/bool.hpp>       // for boost::mpl::bool_
#include <boost/mpl/if.hpp>         // for boost::mpl::if_
#include <boost/mpl/int.hpp>        // for boost::mpl::int_
#include <boost/typeof/typeof.hpp>  // for BOOST_AUTO

#include <algorithm>  // for std::copy, equal
#include <climits>    // for CHAR_BIT
#include <cstddef>    // for std::size_t


namespace boost
{
namespace coding
{


//  Byte-processing coding shell class template declaration  -----------------//

/** \brief  A presentation layer for encapsulating byte-processing.

    This class template can wrap a byte-computation class type.  This template
    can provide an easy interface for a computation type, or a family of
    computation types can share an already-implemented interface.  The
    separation of concerns between presentation and computation can ease
    debugging, since the two families of methods are in different types.

    \pre  <i>(Move \p ByteProcessor support list here)</i>.

    \tparam ByteProcessor  The byte-processing class type, where all of this
                           class template's functionality is routed to.  It must
                           support:
                           - copying (either through assignment operator or
                             constructor); default construction recommended.
                           - a member type \c consumed_type that compatible
                             with a byte, like <code>unsigned char</code>.
                           - a member type \c product_type (that's "normal")
                           - a (non-<code>const</code>) member <code>operator
                             ()</code> that takes a single \c consumed_type
                             argument.
                           - equality operators (at least <code>==</code>),
                             member or free.
                           - a (<code>const</code>) member <code>operator
                             ()</code> that takes nothing and returns a
                             \c product_type value.
 */
template < class ByteProcessor >
class byte_coding_shell
{
    typedef byte_coding_shell  self_type;

    class applicator
    {
        friend class byte_coding_shell;
        ByteProcessor  source;
        explicit  applicator( ByteProcessor const &p = ByteProcessor() )
          : source( p )  {}
    public:
        void  operator ()( unsigned char byte )  { this->source( byte ); }
        bool  operator ==( applicator const &o ) const
          { return this->source == o.source; }
        bool  operator !=( applicator const &o ) const
          { return !this->operator ==( o ); }
    };

public:
    // Types
    /** \brief  Type of the byte-processing engine

        Represents the type check-summing bytes.
     */
    typedef ByteProcessor                     processor_type;
    /** \brief  Type of size values

        Represents the type used for sizing parameters.
     */
    typedef std::size_t                            size_type;
    /** \brief  Type of the byte-processing result

        Represents the type of hashes generated.
     */
    typedef typename ByteProcessor::product_type  value_type;

    // Member data
    /** \brief  Proxy for byte-oriented application interface

        Accesses an interface where <code>this-&gt;bytes</code> can be used as a
        function object that can take a single <code>unsigned char</code> value
        as input.  It cannot be reseated from <code>*this</code>, but assignment
        to it will copy the other's owner's state to <code>*this</code>,
        enabling algorithms that return updated function objects to work.
     */
    applicator  bytes;

    // Lifetime management (use automatic copy constructor and destructor)
    //! Default and member-initialization construction
    explicit  byte_coding_shell( processor_type const &p = processor_type() );

    /*! \name Access */ //@{
    // Accessors
    //! Non-mutable access to byte-processing engine
    processor_type const &  context() const;
    //! Mutable access to byte-processing engine
    processor_type &        context();//@}

    /*! \name Input reading */ //@{
    // Processors
    //! Enters a whole byte
    void  process_byte( unsigned char byte );
    //! Enters several bytes, all of the same value
    void  process_byte_copies( unsigned char value, size_type byte_count );
    //! Enters a range of bytes in memory
    void  process_block( void const *bytes_begin, void const *bytes_end );
    //! Enters a byte buffer in memory
    void  process_bytes( void const *buffer, size_type byte_count );//@}

    /*! \name Output writing */ //@{
    // Finishers
    //! Returns a completed hash
    value_type  checksum() const;//@}

    /*! \name Operators */ //@{
    // Operators (use automatic copy-assignment)
    //! Equals
    bool  operator ==( self_type const &o ) const;
    //! Not-equals
    bool  operator !=( self_type const &o ) const;//@}

};  // byte_coding_shell


//  Byte-processing coding shell class template member definitions  ----------//

/** Constructs a byte-processor shell set to a given engine.

    \param p  The byte-processor engine to copy as the initial state.  If not
              given, a default-constructed engine is used.

    \post  \c #bytes uses a copy of \p p and refers to \c *this.
 */
template < class ByteProcessor >
inline
byte_coding_shell<ByteProcessor>::byte_coding_shell
(
    processor_type const &  p  // = processor_type()
)
    : bytes( p )
{}

/** Provides non-mutable access to the byte-processor engine.

    \return  A \c const reference to the internal engine.
 */
template < class ByteProcessor >
inline typename byte_coding_shell<ByteProcessor>::processor_type const &
byte_coding_shell<ByteProcessor>::context() const
{ return this->bytes.source; }

/** Provides mutable access to the byte-processor engine.

    \post  None, besides what tinkering with the result may do.

    \return  A non-<code>const</code> reference to the internal engine.
 */
template < class ByteProcessor >
inline typename byte_coding_shell<ByteProcessor>::processor_type &
byte_coding_shell<ByteProcessor>::context()
{ return this->bytes.source; }

/** Submits a byte for processing.

    \param byte  The byte value to be submitted.

    \post  Calls <code>this-&gt;bytes( <var>byte</var> )</code>.
 */
template < class ByteProcessor >
inline void
byte_coding_shell<ByteProcessor>::process_byte( unsigned char byte )
{ this->bytes( byte ); }

/** Submits multiple copies of a single byte value for processing.

    \param value       The byte value to be submitted.
    \param byte_count  The number of bytes to submit.

    \post  Calls <code>this-&gt;bytes( <var>value</var> )</code> \p byte_count
           times.
 */
template < class ByteProcessor >
inline void
byte_coding_shell<ByteProcessor>::process_byte_copies
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

    \post  Calls <code>this-&gt;bytes( <var>x</var> )</code> for each byte \p x
           in the given range, starting from the one at the address
           \p bytes_begin to the byte just before the \p bytes_end mark.  (If
           the parameters are identical, then nothing is done.)
 */
template < class ByteProcessor >
inline void
byte_coding_shell<ByteProcessor>::process_block
(
    void const *  bytes_begin,
    void const *  bytes_end
)
{
    BOOST_ASSERT( !bytes_begin == !bytes_end );  // Can't test for reachable!

    for ( BOOST_AUTO(p, static_cast<unsigned char const *>( bytes_begin )) ;
     p != bytes_end ; ++p )
        this->bytes( *p );
}

/** Submits bytes, bounded by a pointer and length, for processing.

    \pre  If \p buffer is \c NULL, then \p byte_count must be zero.

    \param buffer      The start of the byte range to be submitted.
    \param byte_count  Number of (leading) bytes to use from the range.

    \post  Calls <code>this-&gt;bytes( <var>x</var> )</code> for each byte \p x
           in the given range, starting from the one at the address \p buffer,
           going forward for \p byte_count bytes total.
 */
template < class ByteProcessor >
inline void
byte_coding_shell<ByteProcessor>::process_bytes
(
    void const *  buffer,
    size_type     byte_count
)
{
    BOOST_ASSERT( buffer || !byte_count );

    for ( BOOST_AUTO(p, static_cast<unsigned char const *>( buffer )) ;
     byte_count-- ; ++p )
        this->bytes( *p );
}

/** Provides the computed check-sum of all the submitted values.  Since this is
    a \c const member function, the engine must act as if value-submission is
    complete whenever this function is called.

    \return  The check-sum.
 */
template < class ByteProcessor >
inline typename byte_coding_shell<ByteProcessor>::value_type
byte_coding_shell<ByteProcessor>::checksum() const
{ return this->context()(); }

/** Compares byte-processor shells for equivalence.  Such shells are equal if
    their internal engines are equal.

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are equivalent.
    \retval false  \c *this and \p o are not equivalent.
 */
template < class ByteProcessor >
inline bool
byte_coding_shell<ByteProcessor>::operator ==( self_type const &o ) const
{ return this->bytes == o.bytes; }

/** Compares byte-processor shells for non-equivalence.  Such shells are unequal
    if their internal engines are unequal.

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are not equivalent.
    \retval false  \c *this and \p o are equivalent.
 */
template < class ByteProcessor >
inline bool
byte_coding_shell<ByteProcessor>::operator !=( self_type const &o ) const
{ return !this->operator ==( o ); }


//  Byte-processing bit-processor wrapper class template declaration  --------//

/** \brief  A conversion layer: a byte-processor from a bit-processor.

    This class template can represent a byte-level computation class type built
    around a bit-level computation class type.  For each byte submitted to this
    template, \c CHAR_BIT calls to the inner bit-submission routine are done.
    This template enables using a bit-processing object in an interface that
    takes byte-processing objects.

    \pre  <i>(Move \p BitProcessor support list here)</i>.

    \tparam BitProcessor  The bit-processing class type, where all of this
                          class template's functionality is routed to.  It must
                          support:
                          - copying (either through assignment operator or
                            constructor); default construction recommended.
                          - a member type \c consumed_type that compatible
                            with a Boolean, like <code>bool</code>.
                          - a member type \c product_type (that's "normal")
                          - a (non-<code>const</code>) member <code>operator
                            ()</code> that takes a single \c consumed_type
                            argument.
                          - equality operators (at least <code>==</code>),
                            member or free.
                          - a (<code>const</code>) member <code>operator
                            ()</code> that takes nothing and returns a
                            \c product_type value.
    \tparam BigEndian     The order the bits within a byte are processed.  If
                          \c true, the most-significant bit of the byte is
                          processed first, going down to the least-significant
                          bit.  If \p BigEndian is set to \c false, iteration
                          goes from least- to most-significant bit.
 */
template < class BitProcessor, bool BigEndian >
class bit_to_byte_processor
{
    typedef bit_to_byte_processor  self_type;

public:
    // Types
    /** \brief  Type of the bit-processing engine

        Represents the type check-summing byte-packed bits.
     */
    typedef BitProcessor                       processor_type;
    /** \brief  Type of the bit-to-byte-processing output

        Represents the result type passed to the byte-processing client.
     */
    typedef typename BitProcessor::product_type  product_type;
    /** \brief  Type of the byte-to-bit-processing input

        Represents the data type passed from the byte-processing client, i.e.
        bytes.
     */
    typedef unsigned char                       consumed_type;

    /** \brief  The bit-wise endian setting

        Represents which way the bits within a submitted byte are processed.
     */
    typedef mpl::bool_<BigEndian>  read_from_highest;

    // Member data
    /** \brief  The wrapped bit-processing engine

        Since this is a basic helper class, the wrapped object is fully exposed.
     */
    processor_type  inner;

    // Lifetime management (use automatic copy constructor and destructor)
    //! Default and member-initialization construction
    explicit bit_to_byte_processor(processor_type const &p = processor_type() );

    /*! \name Operators */ //@{
    // Operators (use automatic copy-assignment)
    //! Application, consumer
    void  operator ()( consumed_type byte );

    //! Equals
    bool  operator ==( self_type const &o ) const;
    //! Not-equals
    bool  operator !=( self_type const &o ) const;

    //! Application, producer
    product_type  operator ()() const;//@}

private:
    // Bit-loop implementation
    void  consume_bits( unsigned char byte, int amount )
    {
        typedef mpl::int_<+1>   one_type;
        typedef mpl::int_< 0>  zero_type;
        typedef mpl::int_<-1>  one_ntype;

        typedef typename mpl::if_<read_from_highest, one_type, zero_type>::type
          starting_type;
        typedef typename mpl::if_<read_from_highest, one_ntype, one_type>::type
          direction_type;

        BOOST_ASSERT( (amount >= 0) && (amount <= CHAR_BIT) );

        for ( int  i = 0, j = starting_type::value * (amount - 1) ; i < amount ;
         ++i, j += direction_type::value )
            this->inner( byte & (0x01u << j) );
    }

    // Don't let bit_coding_shell manually duplicate consume_bits
    friend class bit_coding_shell<BitProcessor, BigEndian>;

};  // bit_to_byte_processor


//  Byte-processing bit-processor wrapper class template member definitions  -//

/** Constructs a byte-processor engine from to a given bit-processor engine.

    \param p  The bit-processor engine to copy as the initial state.  If not
              given, a default-constructed engine is used.

    \post  <code>#inner == <var>p</var></code>.
 */
template < class BitProcessor, bool BigEndian >
inline
bit_to_byte_processor<BitProcessor, BigEndian>::bit_to_byte_processor
(
    processor_type const &  p  // = processor_type()
)
    : inner( p )
{}

/** Submits a byte for processing, bit-wise in the initialized endian order.

    \param byte  The byte value to be submitted.

    \post  Calls <code>this-&gt;inner( <var>x</var> )</code> \c CHAR_BIT times,
           with \p x iterating from either the highest-order bit (if
           \c #read_from_highest is \c true) or lowest-order bit (if \c false)
           towards, and including, the opposite-order bit.
 */
template < class BitProcessor, bool BigEndian >
inline void
bit_to_byte_processor<BitProcessor, BigEndian>::operator ()(consumed_type byte)
{ this->consume_bits( byte, CHAR_BIT ); }

/** Compares byte-processors for equivalence.  Such engines are equal if their
    internal bit-processors are equal.

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are equivalent.
    \retval false  \c *this and \p o are not equivalent.
 */
template < class BitProcessor, bool BigEndian >
inline bool
bit_to_byte_processor<BitProcessor, BigEndian>::operator ==
( self_type const &o ) const
{ return this->inner == o.inner; }

/** Compares byte-processors for non-equivalence.  Such engines are unequal if
    their internal bit-processors are unequal.

    \param o  The right-side operand to be compared.

    \retval true   \c *this and \p o are not equivalent.
    \retval false  \c *this and \p o are equivalent.
 */
template < class BitProcessor, bool BigEndian >
inline bool
bit_to_byte_processor<BitProcessor, BigEndian>::operator !=
( self_type const &o ) const
{ return !this->operator ==( o ); }

/** Provides the computed check-sum of all the submitted bytes, through a
    standard generator interface.  Since this is a \c const member function, the
    engine must act as if bit-submission is complete whenever this function is
    called.

    \return  The check-sum.
 */
template < class BitProcessor, bool BigEndian >
inline typename bit_to_byte_processor<BitProcessor, BigEndian>::product_type
bit_to_byte_processor<BitProcessor, BigEndian>::operator ()() const
{ return this->inner(); }


//  Bit-processing coding shell class template declaration  ------------------//

/** \brief  A presentation layer for encapsulating bit-processing.

    This class template can wrap a bit-computation class type.  This template
    can provide an easy interface for a computation type, or a family of
    computation types can share an already-implemented interface.  The
    separation of concerns between presentation and computation can ease
    debugging, since the two families of methods are in different types.

    This class template extends \c boost::coding::byte_coding_shell with
    bit-level operations, but doesn't publicly inherit from the base class
    template to hide the base's \c processor_type and equality operators.

    \pre  <i>(Move \p BitProcessor support list here)</i>.

    \tparam BitProcessor  The bit-processing class type, where all of this
                          class template's functionality is routed to.  It must
                          support:
                          - copying (either through assignment operator or
                            constructor); default construction recommended.
                          - a member type \c consumed_type that compatible
                            with a Boolean, like <code>bool</code>.
                          - a member type \c product_type (that's "normal")
                          - a (non-<code>const</code>) member <code>operator
                            ()</code> that takes a single \c consumed_type
                            argument.
                          - equality operators (at least <code>==</code>),
                            member or free.
                          - a (<code>const</code>) member <code>operator
                            ()</code> that takes nothing and returns a
                            \c product_type value.
    \tparam BigEndian     The order the bits within a byte are processed.  If
                          \c true, the most-significant bit of the byte is
                          processed first, going down to the least-significant
                          bit.  If \p BigEndian is set to \c false, iteration
                          goes from least- to most-significant bit.

    \see  boost::coding::byte_coding_shell
    \see  boost::coding::bit_to_byte_processor
 */
template < class BitProcessor, bool BigEndian >
class bit_coding_shell
    : public byte_coding_shell<bit_to_byte_processor<BitProcessor, BigEndian> >
{
    typedef byte_coding_shell< bit_to_byte_processor<BitProcessor, BigEndian> >
      base_type;
    typedef bit_coding_shell  self_type;

    class applicator
    {
        friend class bit_coding_shell;
        BitProcessor *  source;
        bool            own;
        explicit  applicator( BitProcessor *pp )
          : source( pp ), own( false )  {}
    public:
        applicator( applicator const &c )
          : source( new BitProcessor(*c.source) ), own( true )  {}
        ~applicator()  { if ( own ) delete source; }
        applicator &  operator =( applicator const &c )
          { *this->source = *c.source; return *this; }
        void  operator ()( bool bit )  { (*this->source)( bit ); }
        bool  operator ==( applicator const &o ) const
          { return *this->source == *o.source; }
        bool  operator !=( applicator const &o ) const
          { return !this->operator ==( o ); }
    };

public:
    // Types
    /** \brief  Type of the bit-processing engine

        Represents the type check-summing bits.
     */
    typedef BitProcessor                     processor_type;
    /** \brief  Type of size values

        Represents the type used for sizing parameters.
     */
    typedef std::size_t                           size_type;
    /** \brief  Type of the bit-processing result

        Represents the type of hashes generated.
     */
    typedef typename BitProcessor::product_type  value_type;

    /** \brief  The bit-wise endian setting (as a integral meta-constant)

        Represents which way the bits within a submitted byte are processed.
     */
    typedef mpl::bool_<BigEndian>  read_from_highest;

    // Member data
    /** \brief  Proxy for bit-oriented application interface

        Accesses an interface where <code>this-&gt;bits</code> can be used as a
        function object that can take a single <code>bool</code> value as input.
        It cannot be reseated from <code>*this</code>, but assignment to it will
        copy the other's owner's state to <code>*this</code>, enabling
        algorithms that return updated function objects to work.

        \todo  The current implementation stores a reference pointer to
               \c byte_coding_shell::bytes' wrapped engine, but copies of this
               sub-object will allocate (with regular <code>operator new</code>)
               independent copies of that engine.  This will cause throws or
               crashes in situations when default memory allocation is full or
               disabled.  (And there's no choice for an alternate allocation
               scheme.)  Maybe a <code>boost::variant&lt; BitProcessor *,
               BitProcessor &gt;</code> can be used, but it has its own
               complicated storage issues, especially during assignment.
     */
    applicator  bits;

    // Lifetime management (use automatic destructor)
    //! Default and member-initialization construction
    explicit  bit_coding_shell( processor_type const &p = processor_type() );
    //! Copy construction
    bit_coding_shell( self_type const &c );

    /*! \name Access */ //@{
    // Accessors
    //! Non-mutable access to bit-processing engine
    processor_type const &  context() const;
    //! Mutable access to bit-processing engine
    processor_type &        context();//@}

    /*! \name Input reading */ //@{
    // Processors
    //! Enters one bit for hashing
    void  process_bit( bool bit );
    //! Enters lowest-significant part of a byte
    void  process_bits( unsigned char bits, size_type bit_count );
    //! Enters several bits, all of the same value
    void  process_bit_copies( bool value, size_type bit_count );//@}

    /*! \name Operators */ //@{
    // Operators
    //! Copy-assignment
    self_type &  operator =( self_type const &c );

};  // bit_coding_shell


//  Bit-processing coding shell class template member definitions  -----------//

/** Constructs a bit-processor shell set to a given engine.

    \param p  The bit-processor engine to copy as the initial state.  If not
              given, a default-constructed engine is used.

    \post  \c #bytes and \c #bits use a copy of \p p and refer to \c *this.
 */
template < class BitProcessor, bool BigEndian >
inline
bit_coding_shell<BitProcessor, BigEndian>::bit_coding_shell
(
    processor_type const &  p  // = processor_type()
)
    : base_type( typename base_type::processor_type(p) )
    , bits( &this->base_type::context().inner )
{}

/** Constructs a bit-processor shell with a copy of another's the significant
    state.

    \param c  The bit-processor shell containing the engine to copy.

    \post  \c #bytes and \c #bits use a copy of
           <code><var>c</var>.context()</code>, but refer to \c *this.
 */
template < class BitProcessor, bool BigEndian >
inline
bit_coding_shell<BitProcessor, BigEndian>::bit_coding_shell(self_type const &c)
    : base_type( typename base_type::processor_type(c.context()) )
    , bits( &this->base_type::context().inner )
{}

/** Provides non-mutable access to the bit-processor engine.  This overload
    exists to give direct access to the bit-processor engine, since the base
    class's version will return access to the wrapping byte-processing engine.

    \return  A \c const reference to the internal engine.
 */
template < class BitProcessor, bool BigEndian >
inline BitProcessor const &
bit_coding_shell<BitProcessor, BigEndian>::context() const
{ return *this->bits.source; }

/** Provides mutable access to the bit-processor engine.  This overload exists
    to give direct access to the bit-processor engine, since the base class's
    version will return access to the wrapping byte-processing engine.

    \post  None, besides what tinkering with the result may do.

    \return  A non-<code>const</code> reference to the internal engine.
 */
template < class BitProcessor, bool BigEndian >
inline BitProcessor &
bit_coding_shell<BitProcessor, BigEndian>::context()
{ return *this->bits.source; }

/** Submits a single bit for processing.

    \param bit  The bit value to be submitted.

    \post  Calls <code>this-&gt;bits( <var>bit</var> )</code>.
 */
template < class BitProcessor, bool BigEndian >
inline void
bit_coding_shell<BitProcessor, BigEndian>::process_bit( bool bit )
{ this->bits( bit ); }

/** Submits lowest-order part of a byte for processing, bit-wise in the
    initialized endian order.

    \pre  <code>0 &lt;= <var>bit_count</var> &lt;= CHAR_BIT</code>.

    \param bits       The byte from which the values are submitted.
    \param bit_count  The number of bits to submit.

    \post  Calls <code>this-&gt;bits( <var>x</var> )</code> \p bit_count times,
           with \p x iterating from either the 2<sup><var>bit_count</var> -
           1</sup>-place (if \c #read_from_highest is \c true) or the ones-place
           (if \c false) towards, and including, the other mentioned bit.  (If
           \p bit_count is zero, no calls are made.)
 */
template < class BitProcessor, bool BigEndian >
inline void
bit_coding_shell<BitProcessor, BigEndian>::process_bits
(
    unsigned char  bits,
    size_type      bit_count
)
{ this->base_type::context().consume_bits( bits, bit_count ); }

/** Submits multiple copies of a single bit value for processing.

    \param value      The bit value to be submitted.
    \param bit_count  The number of bits to submit.

    \post  Calls <code>this-&gt;bits( <var>value</var> )</code> \p bit_count
           times.
 */
template < class BitProcessor, bool BigEndian >
inline void
bit_coding_shell<BitProcessor, BigEndian>::process_bit_copies
(
    bool       value,
    size_type  bit_count
)
{
    while ( bit_count-- )
        this->bits( value );
}

/** Changes a bit-processing shell to have the same observable state as a given
    shell.  (No function object proxies are reseated, however.)  This overload
    exists because the default action would perform an unnecessary extra copy.

    \param c  The source object with the new state.

    \return  \c *this

    \post  \c #bits and \c #bytes \e still point to \c *this
 */
template < class BitProcessor, bool BigEndian >
inline bit_coding_shell<BitProcessor, BigEndian> &
bit_coding_shell<BitProcessor, BigEndian>::operator =( self_type const &c )
{
    static_cast<base_type &>( *this ) = static_cast<base_type const &>( c );
    // "bits" can't be reseated; but if it got assigned, it would ultimately
    // perform a copy of the core bit-processing engine, which was already done
    // in the base class's version of the copy-assignment operator.  (This would
    // be bad of the engine's assignment took a lot of time or resources.)  So
    // we can skip it.
    return *this;
}


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_CODING_SHELL_HPP
