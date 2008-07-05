// Boost coding/coding_shell.hpp header file  --------------------------------//

// (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for documentation.

/** \file
    \brief  Definitions of coding shell helper class templates

    Contains the definition of templates that encapsulate bit- and byte-encoding
    computer types, given a coding kernel.
 */

#ifndef BOOST_CODING_CODING_SHELL_HPP
#define BOOST_CODING_CODING_SHELL_HPP

#include <boost/coding_fwd.hpp>

#include <boost/assert.hpp>         // for BOOST_ASSERT
#include <boost/mpl/if.hpp>         // for boost::mpl::if_c
#include <boost/mpl/int.hpp>        // for boost::mpl::int_
#include <boost/typeof/typeof.hpp>  // for BOOST_AUTO

#include <algorithm>  // for std::copy, equal
#include <climits>    // for CHAR_BIT


namespace boost
{
namespace coding
{


//  Byte-processing coding shell class template declaration  -----------------//

template < class ByteProcessor, typename SizeType >
class byte_coding_shell
{
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
    typedef ByteProcessor                   processor_type;
    typedef SizeType                             size_type;
    typedef typename ByteProcessor::value_type  value_type;

    // Member data
    applicator  bytes;

    // Lifetime management (use automatic copy constructor and destructor)
    explicit  byte_coding_shell( processor_type const &p = processor_type() )
      : bytes( p )  {}

    // Accessors
    processor_type const &  context() const  { return this->bytes.source; }
    processor_type &        context()        { return this->bytes.source; }

    // Processors
    void  process_byte( unsigned char byte )  { this->bytes( byte ); }
    void  process_byte_copies( unsigned char value, size_type byte_count )
      { while ( byte_count-- )  this->bytes( value ); }
    void  process_block( void const *bytes_begin, void const *bytes_end )
    {
        BOOST_ASSERT( !bytes_begin == !bytes_end );

        for ( BOOST_AUTO(p, static_cast<unsigned char const *>( bytes_begin )) ;
         p != bytes_end ; ++p )
            this->bytes( *p );
    }
    void  process_bytes( void const *buffer, size_type byte_count )
    {
        BOOST_ASSERT( buffer || !byte_count );

        for ( BOOST_AUTO(p, static_cast<unsigned char const *>( buffer )) ;
         byte_count-- ; ++p )
            this->bytes( *p );
    }

    // Finishers
    value_type  checksum() const  { return this->context()(); }

    // Operators (use automatic copy-assignment)
    bool  operator ==( byte_coding_shell const &o ) const
      { return this->bytes == o.bytes; }
    bool  operator !=( byte_coding_shell const &o ) const
      { return !this->operator ==( o ); }

    value_type  operator ()() const  { return this->checksum(); }

};  // byte_coding_shell


//  Bit-processing coding shell class template declarations  -----------------//

template < class BitProcessor, bool BigEndian >
class bit_to_byte_processor
{
public:
    // Types
    typedef BitProcessor                   processor_type;
    typedef typename BitProcessor::value_type  value_type;

    // Constants
    static  bool const  reads_start_from_high_order_bit = BigEndian;

    // Member data
    processor_type  inner;

    // Lifetime management (use automatic copy constructor and destructor)
    explicit bit_to_byte_processor( processor_type const &p = processor_type() )
      : inner( p )  {}

    // Operators (use automatic copy-assignment)
    void  operator ()( unsigned char byte )
    {
        using mpl::if_c;
        using mpl::int_;

        typedef typename if_c< BigEndian, int_<1>, int_<0> >::type     starting;
        typedef typename if_c< BigEndian, int_<-1>, int_<+1> >::type  direction;

        for ( int  i = 0, j = starting::value * (CHAR_BIT - 1) ; i < CHAR_BIT ;
         ++i, j += direction::value )
            this->inner( byte & (0x01u << j) );
    }

    bool  operator ==( bit_to_byte_processor const &o ) const
      { return this->inner == o.inner; }
    bool  operator !=( bit_to_byte_processor const &o ) const
      { return !this->operator ==( o ); }

    value_type  operator ()() const  { return this->inner(); }

};  // bit_to_byte_processor

template < class BitProcessor, bool BigEndian, typename SizeType >
class bit_coding_shell
    : private byte_coding_shell< bit_to_byte_processor<BitProcessor, BigEndian>,
       SizeType >
{
    typedef byte_coding_shell< bit_to_byte_processor<BitProcessor, BigEndian>,
     SizeType >  base_type;

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
    typedef BitProcessor                   processor_type;
    typedef SizeType                            size_type;
    typedef typename BitProcessor::value_type  value_type;

    // Constants
    static  bool const  reads_start_from_high_order_bit = BigEndian;

    // Member data
    using base_type::bytes;

    applicator  bits;

    // Lifetime management (use automatic destructor)
    explicit  bit_coding_shell( processor_type const &p = processor_type() )
        : base_type( typename base_type::processor_type(p) )
        , bits( &this->base_type::context().inner )
    {
    }
    bit_coding_shell( bit_coding_shell const &c )
        : base_type( typename base_type::processor_type(c.context()) )
        , bits( &this->base_type::context().inner )
    {
    }

    // Accessors
    processor_type const &  context() const  { return *this->bits.source; }
    processor_type &        context()        { return *this->bits.source; }

    // Processors
    using base_type::process_byte;
    using base_type::process_byte_copies;
    using base_type::process_block;
    using base_type::process_bytes;

    void  process_bit( bool bit )  { this->bits( bit ); }
    void  process_bits( unsigned char bits, size_type bit_count )
    {
        using mpl::if_c;
        using mpl::int_;

        BOOST_ASSERT( (0u <= bit_count) && (bit_count <= CHAR_BIT) );

        typedef typename if_c< BigEndian, int_<1>, int_<0> >::type     starting;
        typedef typename if_c< BigEndian, int_<-1>, int_<+1> >::type  direction;

        for ( int  i = (static_cast<int>(bit_count) - 1) * starting::value ;
         bit_count-- ; i += direction::value )
            this->bits( bits & (0x01u << i) );
    }
    void  process_bit_copies( bool value, size_type bit_count )
      { while ( bit_count-- )  this->bits( value ); }

    void  process_octet( uint_least8_t octet )
      { this->process_bits( octet, 8 ); }

    // Finishers
    using base_type::checksum;

    // Operators
    bit_coding_shell &  operator =( bit_coding_shell const &c )
    {
        // Make sure "bits" isn't reseated
        static_cast<base_type &>( *this ) = static_cast<base_type const &>( c );
        return *this;
    }
    bool  operator ==( bit_coding_shell const &o ) const
      { return this->bits == o.bits; }
    bool  operator !=( bit_coding_shell const &o ) const
      { return !this->operator ==( o ); }

    using base_type::operator ();

};  // bit_coding_shell


//  Byte-processing coding shell class template member definitions  ----------//

// Put something here


//  Bit-processing coding shell class templates member defintions  -----------//

template < class BitProcessor, bool BigEndian >
bool const  bit_to_byte_processor<BitProcessor,
 BigEndian>::reads_start_from_high_order_bit;

template < class BitProcessor, bool BigEndian, typename SizeType >
bool const  bit_coding_shell<BitProcessor, BigEndian,
 SizeType>::reads_start_from_high_order_bit;


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_CODING_SHELL_HPP
