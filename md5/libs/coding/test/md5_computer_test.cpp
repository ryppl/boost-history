// Boost md5_computer_test.cpp test file  ------------------------------------//

// (C) Copyright 2008 Daryle Walker.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for the library's home page.


#include <boost/coding/md5.hpp>

#include <boost/array.hpp>           // for boost::array
#include <boost/cstdint.hpp>         // for uint_least(8|32|64)_t
#include <boost/foreach.hpp>         // for BOOST_FOREACH
#include <boost/lexical_cast.hpp>    // for boost::lexical_cast
#include <boost/test/unit_test.hpp>  // unit testing framework

#include <algorithm>  // for std::reverse, for_each
#include <climits>    // for CHAR_BIT
#include <cstddef>    // for std::size_t


#pragma mark Intro stuff

// Put any using-ed types & templates, and typedefs here
using boost::array;
using boost::coding::md5_computer;
using boost::coding::md5_digest;

typedef array<bool, md5_computer::bits_per_block::value>  bit_queue;


// Put custom types/templates and helper functions here
namespace
{

// Sample MD buffer values
md5_computer::buffer_type const  md5_initial = { {0x67452301ul, 0xEFCDAB89ul,
 0x98BADCFEul, 0x10325476ul} };  // from RFC 1321, section 3.3
md5_computer::buffer_type const  md5_empty = { {0xD98C1DD4ul, 0x04B2008Ful,
 0x980980E9ul, 0x7E42F8ECul} };  // from RFC 1321, section A.5, result 1

// Sample MD values
md5_digest const  md5_empty_data = { {0xD98C1DD4ul, 0x04B2008Ful, 0x980980E9ul,
 0x7E42F8ECul} };  // see md5_empty

}  // unnamed namespace


// Mark any unprintable tested types here


#pragma mark -
#pragma mark MD5 Digest Computer Suite

// Test the operations of the MD5 computation class and function
BOOST_AUTO_TEST_SUITE( md5_computer_suite )

// Default construction test and inspectors (no submission)
BOOST_AUTO_TEST_CASE( md5_computer_default_constructor_status_test )
{
    md5_computer  c;
    BOOST_CHECK_EQUAL( c.bits_read(), 0u );
    BOOST_CHECK_EQUAL( c.bits_unbuffered(), 0u );

    md5_computer::buffer_type const  b = c.last_buffer();
    BOOST_CHECK_EQUAL_COLLECTIONS( b.begin(), b.end(), md5_initial.begin(),
     md5_initial.end() );

    bit_queue  bq = { {false} };
    BOOST_CHECK_EQUAL( bq.begin(), c.copy_unbuffered(bq.begin()) );
}

// Basic bit/byte-submission tests and inspectors (no hashing)
BOOST_AUTO_TEST_CASE( md5_computer_simple_submission_test )
{
    using std::size_t;

    bit_queue  scratch;

    // Single bit
    {
        md5_computer  c1;
        c1.process_bit( true );
        BOOST_CHECK_EQUAL( c1.bits_read(), 1u );
        BOOST_CHECK_EQUAL( c1.bits_unbuffered(), 1u );

        md5_computer::buffer_type const  b1 = c1.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b1.begin(), b1.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 1> const  r1 = { {true} };
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c1.copy_unbuffered(scratch.begin()), r1.begin(), r1.end() );
    }

    {
        md5_computer  c2;
        c2.process_bit( false );
        BOOST_CHECK_EQUAL( c2.bits_read(), 1u );
        BOOST_CHECK_EQUAL( c2.bits_unbuffered(), 1u );

        md5_computer::buffer_type const  b2 = c2.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b2.begin(), b2.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 1> const  r2 = { {false} };
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c2.copy_unbuffered(scratch.begin()), r2.begin(), r2.end() );
    }

    // Multiple bits, under a byte
    {
        md5_computer         c3;
        size_t const         bits_to_use = 7u;  // only 6 will be necessary
        unsigned char const  value = 0x37u;     // 0110111
        c3.process_bits( value, bits_to_use );
        BOOST_CHECK_EQUAL( c3.bits_read(), bits_to_use );
        BOOST_CHECK_EQUAL( c3.bits_unbuffered(), bits_to_use );

        md5_computer::buffer_type const  b3 = c3.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b3.begin(), b3.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 7> const  r3 = { {false, true, true, false, true, true,
         true} };
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c3.copy_unbuffered(scratch.begin()), r3.begin(), r3.end() );
    }

    {
        md5_computer  c4;
        size_t const  bits_to_use = 6u;
        c4.process_bit_copies( false, bits_to_use );
        BOOST_CHECK_EQUAL( c4.bits_read(), bits_to_use );
        BOOST_CHECK_EQUAL( c4.bits_unbuffered(), bits_to_use );

        md5_computer::buffer_type const  b4 = c4.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b4.begin(), b4.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 6> const  r4 = {{false, false, false, false, false, false}};
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c4.copy_unbuffered(scratch.begin()), r4.begin(), r4.end() );
    }

    {
        md5_computer  c5;
        size_t const  bits_to_use = 5u;
        c5.process_bit_copies( true, bits_to_use );
        BOOST_CHECK_EQUAL( c5.bits_read(), bits_to_use );
        BOOST_CHECK_EQUAL( c5.bits_unbuffered(), bits_to_use );

        md5_computer::buffer_type const  b5 = c5.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b5.begin(), b5.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 5> const  r5 = { {true, true, true, true, true} };
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c5.copy_unbuffered(scratch.begin()), r5.begin(), r5.end() );
    }

    // Whole bytes, one value
    {
        md5_computer         c6;
        unsigned char const  value = 0x37u;  // {'0' x (CHAR_BIT-6)}110111
        c6.process_byte( value );
        BOOST_CHECK_EQUAL( c6.bits_read(), CHAR_BIT );
        BOOST_CHECK_EQUAL( c6.bits_unbuffered(), CHAR_BIT );

        md5_computer::buffer_type const  b6 = c6.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b6.begin(), b6.end(),
         md5_initial.begin(), md5_initial.end() );

        // The usual initialization routine won't work because there will be an
        // unknown number of "false" entries in the beginning.  (Can't assume
        // that CHAR_BIT is 8.)  But C++'s rules state that incomplete array
        // initialization lists are filled with the default value (i.e.
        // "false") at the trailing unspecified entries.  So specify "r6"
        // backwards (i.e. lowest bit first), then reverse it!
        array<bool, CHAR_BIT>  r6 = { {true, true, true, false, true, true} };
        std::reverse( r6.begin(), r6.end() );
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c6.copy_unbuffered(scratch.begin()), r6.begin(), r6.end() );
    }

    {
        md5_computer         c7;
        size_t const         bytes_to_use = 3u;
        unsigned char const  value = 0x37u;  // {'0' x (CHAR_BIT-6)}110111
        BOOST_REQUIRE_LT( CHAR_BIT * bytes_to_use,
         md5_computer::bits_per_block::value );
        c7.process_byte_copies( value, bytes_to_use );
        BOOST_CHECK_EQUAL( c7.bits_read(), CHAR_BIT * bytes_to_use );
        BOOST_CHECK_EQUAL( c7.bits_unbuffered(), CHAR_BIT * bytes_to_use );

        md5_computer::buffer_type const  b7 = c7.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b7.begin(), b7.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 3 * CHAR_BIT>  r7;
        for ( size_t  i = 0u ; i < 3 * CHAR_BIT ; ++i )
        {
            // Copy "value" thrice bit-wise, highest bit first
            r7[ i ] = value & ( 0x01u << (CHAR_BIT - 1 - ( i % CHAR_BIT )) );
        }
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c7.copy_unbuffered(scratch.begin()), r7.begin(), r7.end() );
    }

    // Byte buffers
    {
        // Two pointers
        md5_computer                   c8;
        array<unsigned char, 3> const  values = { {0x37u, 0x48u, 0x1Cu} };
        size_t const                   bytes_to_use = 3u;
        BOOST_REQUIRE_LT( CHAR_BIT * bytes_to_use,
         md5_computer::bits_per_block::value );
        c8.process_block( values.begin(), values.end() );
        BOOST_CHECK_EQUAL( c8.bits_read(), CHAR_BIT * bytes_to_use );
        BOOST_CHECK_EQUAL( c8.bits_unbuffered(), CHAR_BIT * bytes_to_use );

        md5_computer::buffer_type const  b8 = c8.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b8.begin(), b8.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 3 * CHAR_BIT>  r8;
        for ( size_t  i = 0u ; i < 3 ; ++i )
        {
            for ( size_t  j = 0u ; j < CHAR_BIT ; ++j )
            {
                // Copy "values", each time highest bit first
                r8[ i * CHAR_BIT + j ] = values[ i ] & ( 0x01u << (CHAR_BIT - 1
                 - j) );
            }
        }
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c8.copy_unbuffered(scratch.begin()), r8.begin(), r8.end() );

        // Pointer and length
        md5_computer  c9;
        c9.process_bytes( values.begin(), bytes_to_use );
        BOOST_CHECK_EQUAL( c9.bits_read(), CHAR_BIT * bytes_to_use );
        BOOST_CHECK_EQUAL( c9.bits_unbuffered(), CHAR_BIT * bytes_to_use );

        md5_computer::buffer_type const  b9 = c9.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b9.begin(), b9.end(),
         md5_initial.begin(), md5_initial.end() );

        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c9.copy_unbuffered(scratch.begin()), r8.begin(), r8.end() );
    }

    // Octets, words, and double-words
    {
        md5_computer                c10;
        boost::uint_least8_t const  value = 0x37u;  // 00110111
        c10.process_octet( value );
        BOOST_CHECK_EQUAL( c10.bits_read(), 8 );
        BOOST_CHECK_EQUAL( c10.bits_unbuffered(), 8 );

        md5_computer::buffer_type const  b10 = c10.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b10.begin(), b10.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 8>  r10 = { {false, false, true, true, false, true, true,
         true} };
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c10.copy_unbuffered(scratch.begin()), r10.begin(), r10.end() );
    }

    {
        md5_computer                 c11;
        boost::uint_least32_t const  value = 0xDEAC0812ul;
          // 11011110101011000000100000010010
        c11.process_word( value );
        BOOST_CHECK_EQUAL( c11.bits_read(), 32 );
        BOOST_CHECK_EQUAL( c11.bits_unbuffered(), 32 );

        md5_computer::buffer_type const  b11 = c11.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b11.begin(), b11.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 32>  r11 = { {false, false, false, true, false, false, true,
         false, false, false, false, false, true, false, false, false, true,
         false, true, false, true, true, false, false, true, true, false, true,
         true, true, true, false} };  // 12, 08, AC, DE
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c11.copy_unbuffered(scratch.begin()), r11.begin(), r11.end() );
    }

    {
        md5_computer                 c12;
        boost::uint_least64_t const  value = 0x01234567FEDCBA98ull;
          // 0000000100100011010001010110011111111110110111001011101010011000
        c12.process_double_word( value );
        BOOST_CHECK_EQUAL( c12.bits_read(), 64 );
        BOOST_CHECK_EQUAL( c12.bits_unbuffered(), 64 );

        md5_computer::buffer_type const  b12 = c12.last_buffer();
        BOOST_CHECK_EQUAL_COLLECTIONS( b12.begin(), b12.end(),
         md5_initial.begin(), md5_initial.end() );

        array<bool, 64>  r12 = { {true, false, false, true, true, false, false,
         false, true, false, true, true, true, false, true, false, true, true,
         false, true, true, true, false, false, true, true, true, true, true,
         true, true, false, false, true, true, false, false, true, true, true,
         false, true, false, false, false, true, false, true, false, false,
         true, false, false, false, true, true, false, false, false, false,
         false, false, false, true} };  // 98, BA, DC, FE; 67, 45, 23, 01
        BOOST_CHECK_EQUAL_COLLECTIONS( scratch.begin(),
         c12.copy_unbuffered(scratch.begin()), r12.begin(), r12.end() );
    }
}

// Hashed bit/byte-submission tests and inspectors
BOOST_AUTO_TEST_CASE( md5_computer_hashed_submission_test )
{
    // Confirm quality of math library
    array<md5_digest::word_type, 64> const  hash_table =
     md5_computer::generate_hashing_table();
    BOOST_WARN_EQUAL_COLLECTIONS( hash_table.begin(), hash_table.end(),
     md5_computer::hashing_table.begin(), md5_computer::hashing_table.end() );

    // This simulates running MD5 on an empty message
    md5_computer  c;
    c.process_bit( true );
    c.process_bit_copies( false, md5_computer::bits_per_block::value -
     md5_computer::significant_bits_per_length::value - 1u );
    c.process_double_word( 0ull );
    BOOST_CHECK_EQUAL( c.bits_read(), md5_computer::bits_per_block::value );
    BOOST_CHECK_EQUAL( c.bits_unbuffered(), 0u );

    md5_computer::buffer_type const  b = c.last_buffer();
    BOOST_CHECK_EQUAL_COLLECTIONS( b.begin(), b.end(), md5_empty.begin(),
     md5_empty.end() );

    bit_queue  scratch;
    BOOST_CHECK_EQUAL( scratch.begin(), c.copy_unbuffered(scratch.begin()) );
}

// Equality operators test
BOOST_AUTO_TEST_CASE( md5_computer_equality_test )
{
    md5_computer  c1, c2, c3, c4;

    // Make some non-default, something like an empty MD5
    c3.process_bit( true );
    c3.process_bit_copies( false, md5_computer::bits_per_block::value -
     md5_computer::significant_bits_per_length::value - 1u );
    c3.process_double_word( 0ull );

    c4.process_bit( true );
    c4.process_bit_copies( false, md5_computer::bits_per_block::value -
     md5_computer::significant_bits_per_length::value - 1u );
    c4.process_double_word( 0ull );

    // Compare
    BOOST_CHECK( c1 == c1 ); BOOST_CHECK( c1 == c2 ); BOOST_CHECK( c1 != c3 );
     BOOST_CHECK( c1 != c4 );
    BOOST_CHECK( c2 == c1 ); BOOST_CHECK( c2 == c2 ); BOOST_CHECK( c2 != c3 );
     BOOST_CHECK( c2 != c4 );
    BOOST_CHECK( c3 != c1 ); BOOST_CHECK( c3 != c2 ); BOOST_CHECK( c3 == c3 );
     BOOST_CHECK( c3 == c4 );
    BOOST_CHECK( c4 != c1 ); BOOST_CHECK( c4 != c2 ); BOOST_CHECK( c4 == c3 );
     BOOST_CHECK( c4 == c4 );

    // Make the only difference at the hash level
    c2.process_bit( true );
    c2.process_bit_copies( false, md5_computer::bits_per_block::value -
     md5_computer::significant_bits_per_length::value - 1u );
    c2.process_double_word( 1ull );  // one bit differs
    BOOST_REQUIRE_EQUAL( c2.bits_read(), c3.bits_read() );
    BOOST_REQUIRE_EQUAL( c2.bits_unbuffered(), 0u );
    BOOST_CHECK( c2 != c3 );

    // Make the only difference at the queue level
    c3.process_bit( true );
    c4.process_bit( false );
    BOOST_REQUIRE_EQUAL( c3.bits_read(), c4.bits_read() );
    BOOST_REQUIRE( c3.last_buffer() == c4.last_buffer() );
    BOOST_CHECK( c3 != c4 );
}

// Copy construction and assignments test
BOOST_AUTO_TEST_CASE( md5_computer_copy_constructor_status_test )
{
    // Get a non-default valued computer, something like an empty MD5
    md5_computer  c1;
    bit_queue     scratch;

    c1.process_bit( true );
    c1.process_bit_copies( false, md5_computer::bits_per_block::value -
     md5_computer::significant_bits_per_length::value - 1u );
    c1.process_double_word( 0ull );
    c1.process_bit( false );  // one extra after buffer hash
    BOOST_CHECK( c1.bits_read() != c1.bits_unbuffered() );
    BOOST_CHECK( c1.last_buffer() == md5_empty );
    BOOST_CHECK_EQUAL(1, c1.copy_unbuffered(scratch.begin()) - scratch.begin());

    // Copy constructor
    md5_computer  c2, c3( c1 );

    BOOST_CHECK( c3 == c1 );
    BOOST_CHECK( c3 != c2 );

    // Assignment operator
    c3 = c2;
    BOOST_CHECK( c3 == c2 );
    BOOST_CHECK( c3 != c1 );

    // Assignment method
    c3.assign( c1 );
    BOOST_CHECK( c3 == c1 );
    BOOST_CHECK( c3 != c2 );

    // Reset to default
    c3.reset();
    BOOST_CHECK( c3 == c2 );
    BOOST_CHECK( c3 != c1 );

    // Swap
    c3.swap( c1 );
    BOOST_CHECK( c3 != c2 );
    BOOST_CHECK( c1 == c2 );

    // Free-function swap
    boost::coding::swap( c1, c3 );
    BOOST_CHECK( c3 == c2 );
    BOOST_CHECK( c1 != c2 );
}

// Checksum computation test
BOOST_AUTO_TEST_CASE( md5_computer_checksum_test )
{
    using boost::lexical_cast;

    // Empty MD5 message
    md5_computer      c1;
    md5_digest const  s1 = c1.checksum();

    BOOST_CHECK_EQUAL( s1, md5_empty_data );

    // Application operator interface
    md5_digest const  s2 = c1();
    BOOST_CHECK_EQUAL( s1, s2 );

    // Non-empty message below the limit of padding wrap-around
    c1.process_octet( 0x61u );  // 'a' in ASCII
    BOOST_REQUIRE( c1.bits_unbuffered() > 0u );
    BOOST_REQUIRE_LT( c1.bits_unbuffered(), md5_computer::bits_per_block::value
     - md5_computer::significant_bits_per_length::value );
    BOOST_CHECK_EQUAL( c1.checksum(),
     lexical_cast<md5_digest>("0cc175b9c0f1b6a831c399e269772661") );

    // Non-empty message above the limit of padding wrap-around
    c1.reset();
    for ( unsigned  i = 65 ; i <=  90 ; ++i )  c1.process_octet( i );
    for ( unsigned  i = 97 ; i <= 122 ; ++i )  c1.process_octet( i );
    for ( unsigned  i = 48 ; i <=  57 ; ++i )  c1.process_octet( i );
        // "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" in
        // ASCII
    BOOST_REQUIRE_GT( c1.bits_unbuffered(), md5_computer::bits_per_block::value
     - md5_computer::significant_bits_per_length::value );
    BOOST_CHECK_EQUAL( c1.checksum(),
     lexical_cast<md5_digest>("d174ab98d277d9f5a5611c2c9f419d9f") );
}

// Bit-wise function-object computation test
BOOST_AUTO_TEST_CASE( md5_computer_bit_input_test )
{
    using std::for_each;

    // Compare straight usage
    md5_computer          c1, c2;
    unsigned char const   value = 0x37u;
    array<bool, 8> const  value_bits = { {false, false, true, true, false, true,
     true, true} };

    c1.process_octet( value );  // 00110111
    BOOST_FOREACH( bool b, value_bits )
        c2.bits( b );
    BOOST_CHECK( c1 == c2 );

    // Use with algorithms, note that it NO LONGER works by reference!
    md5_computer  c3, c4;

    BOOST_CHECK( c3 == c4 );
    for_each( value_bits.begin(), value_bits.end(), c3.bits );
    BOOST_CHECK( c3 == c4 );
    BOOST_CHECK( c3 != c2 );
    c3.bits = for_each( value_bits.begin(), value_bits.end(), c3.bits );
    BOOST_CHECK( c3 != c4 );
    BOOST_CHECK( c3 == c2 );

    // Assignment
    c4.bits = c3.bits;
    BOOST_CHECK( c4 == c3 );

    // NO LONGER just a formality
    c4.reset();
    BOOST_CHECK( c4 != c3 );
    c4.bits = for_each( value_bits.begin(), value_bits.end(), c4.bits );
    BOOST_CHECK( c4 == c3 );

    // TODO: temporary bit-applicator objects call "operator new" in their copy
    // constructor.  We need to cause this allocation to fail so that can be
    // tested!  (Maybe a custom "operator new" is needed?)
}

// Byte-wise function-object computation test
BOOST_AUTO_TEST_CASE( md5_computer_byte_input_test )
{
    using std::for_each;

    // Compare straight usage
    md5_computer                   c1, c2;
    array<unsigned char, 3> const  values = { {0x37u, 0x48u, 0x1Cu} };

    c1.process_bytes( values.begin(), values.size() );
    BOOST_FOREACH( unsigned char b, values )
        c2.bytes( b );
    BOOST_CHECK( c1 == c2 );

    // Use with algorithms, note that it NO LONGER works by reference!
    md5_computer  c3, c4;

    BOOST_CHECK( c3 == c4 );
    for_each( values.begin(), values.end(), c3.bytes );
    BOOST_CHECK( c3 == c4 );
    BOOST_CHECK( c3 != c2 );
    c3.bytes = for_each( values.begin(), values.end(), c3.bytes );
    BOOST_CHECK( c3 != c4 );
    BOOST_CHECK( c3 == c2 );

    // Assignment
    c4.bytes = c3.bytes;
    BOOST_CHECK( c4 == c3 );

    // NO LONGER just a formality
    c4.reset();
    BOOST_CHECK( c4 != c3 );
    c4.bytes = for_each( values.begin(), values.end(), c4.bytes );
    BOOST_CHECK( c4 == c3 );
}

// Output archiving test
//BOOST_AUTO_TEST_CASE( md5_computer_output_test )
//{
//}

// Input archiving test
//BOOST_AUTO_TEST_CASE( md5_computer_input_test )
//{
//}

// Single-call function test
BOOST_AUTO_TEST_CASE( compute_md5_test )
{
    using boost::coding::compute_md5;

    BOOST_CHECK_EQUAL( compute_md5(0, 0), md5_empty_data );

    // How do we get consistent data when we can't count on CHAR_BIT being the
    // same everywhere?  Do we just screw over testers without 8-bit bytes or
    // ones without ASCII (or superset)?  If so, then we could use the other
    // results from RFC 1321, section A.5.
}

BOOST_AUTO_TEST_SUITE_END()
