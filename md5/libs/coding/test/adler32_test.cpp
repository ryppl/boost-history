// Boost adler32_test.cpp test file  -----------------------------------------//

// (C) Copyright 2008 Daryle Walker.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for the library's home page.


#define BOOST_TEST_MODULE  "Adler-32 tests"

#include <boost/coding/adler32.hpp>

#include <boost/archive/xml_iarchive.hpp>  // for boost::archive::xml_iarchive
#include <boost/archive/xml_oarchive.hpp>  // for boost::archive::xml_oarchive
#include <boost/cstdint.hpp>               // for boost::uint_least32_t
#include <boost/foreach.hpp>               // for BOOST_FOREACH
#include <boost/serialization/nvp.hpp>     // for boost::serialization::make_nvp
#include <boost/test/unit_test.hpp>        // unit testing framework

#include <algorithm>  // for std::for_each
#include <climits>    // for UCHAR_MAX
#include <cstddef>    // for std::size_t
#include <istream>    // for std::basic_istream
#include <ostream>    // for std::basic_ostream
#include <sstream>    // for std::stringstream


#pragma mark Intro stuff

// Put any using-ed types & templates, and typedefs here
using boost::coding::adler32_computer;


// Put custom types/templates, helper functions, and objects here
namespace
{

// Sample Adler-32 checksum values
boost::uint_least32_t const  adler32_empty_data = 1u;

// Completely read an archive from a stream
template < typename Ch, class Tr, typename T >
void  read_xml_archive( std::basic_istream<Ch, Tr> &i, T &target, char const
 *name )
{
    boost::archive::xml_iarchive  ar( i );

    ar >> boost::serialization::make_nvp( name, target );
}

// Completely write an archive to a stream
template < typename Ch, class Tr, typename T >
void  write_xml_archive( std::basic_ostream<Ch, Tr> &o, T const &target, char
 const *name )
{
    boost::archive::xml_oarchive  ar( o );

    ar << boost::serialization::make_nvp( name, target );
}

}  // unnamed namespace


// Mark any unprintable tested types here
BOOST_TEST_DONT_PRINT_LOG_VALUE( boost::coding::adler32_context );
BOOST_TEST_DONT_PRINT_LOG_VALUE( boost::coding::adler32_computer );


#pragma mark -
#pragma mark Adler-32 Suite

// Test the operations of the Adler-32 computation classes and function
BOOST_AUTO_TEST_SUITE( adler32_computer_suite )

// Default construction, byte-submission, checksum, and inspectors tests
BOOST_AUTO_TEST_CASE( adler32_computer_default_or_submission_test )
{
    using std::size_t;

    // A byte or less
    {
        // No bytes, default constructed
        adler32_computer     c1;

        BOOST_CHECK_EQUAL( c1.augmented_byte_sum(), 1u );
        BOOST_CHECK_EQUAL( c1.sum_of_byte_sums(), 0u );

        BOOST_CHECK_EQUAL( c1.checksum(), adler32_empty_data );

        // Single byte
        unsigned char const  value = 0x37u;
        unsigned long        x_value = value;

        c1.process_byte( value );
        BOOST_CHECK_EQUAL( c1.augmented_byte_sum(), ++x_value );
        BOOST_CHECK_EQUAL( c1.sum_of_byte_sums(), x_value );
        BOOST_CHECK_EQUAL( c1.checksum(), (x_value << 16) | x_value );
    }

    // Byte buffers
    {
        // Two pointers
        adler32_computer     c2;
        unsigned char const  values[3] = { 0x37u, 0x48u, 0x1Cu };
        size_t const         values_sum = values[0] + values[1] + values[2],
         values_ssum = 3 * values[ 0 ] + 2 * values[ 1 ] + values[ 2 ] + 3;

        c2.process_block( values, values + 3 );
        BOOST_CHECK_EQUAL( c2.augmented_byte_sum(), 1u + values_sum );
        BOOST_CHECK_EQUAL( c2.sum_of_byte_sums(), values_ssum );
        BOOST_CHECK_EQUAL( c2.checksum(), (values_ssum << 16) | (1u +
         values_sum) );

        // Pointer and length
        adler32_computer  c3;

        c3.process_bytes( values, 3 );
        BOOST_CHECK_EQUAL( c3.augmented_byte_sum(), 1u + values_sum );
        BOOST_CHECK_EQUAL( c3.sum_of_byte_sums(), values_ssum );
        BOOST_CHECK_EQUAL( c3.checksum(), (values_ssum << 16) | (1u +
         values_sum) );
    }

    // Combining
    {
        adler32_computer     c4, c5, c6;
        unsigned char const  values[5] = { 0x37u, 0x48u, 0x1Cu, 0xB9u, 0xFDu };

        c4.process_block( values, values + 3 );
        c5.process_bytes( values + 3, 2 );
        BOOST_CHECK_NE( c4.augmented_byte_sum(), c5.augmented_byte_sum() );
        BOOST_CHECK_NE( c4.sum_of_byte_sums(), c5.sum_of_byte_sums() );
        BOOST_CHECK_NE( c4.checksum(), c5.checksum() );

        c6.process_block( values, values + 5 );
        BOOST_CHECK_NE( c4.augmented_byte_sum(), c6.augmented_byte_sum() );
        BOOST_CHECK_NE( c4.sum_of_byte_sums(), c6.sum_of_byte_sums() );
        BOOST_CHECK_NE( c4.checksum(), c6.checksum() );
        BOOST_CHECK_NE( c5.augmented_byte_sum(), c6.augmented_byte_sum() );
        BOOST_CHECK_NE( c5.sum_of_byte_sums(), c6.sum_of_byte_sums() );
        BOOST_CHECK_NE( c5.checksum(), c6.checksum() );
        c4.append_computation( c5, 2 );
        BOOST_CHECK_EQUAL( c4.augmented_byte_sum(), c6.augmented_byte_sum() );
        BOOST_CHECK_EQUAL( c4.sum_of_byte_sums(), c6.sum_of_byte_sums() );
        BOOST_CHECK_EQUAL( c4.checksum(), c6.checksum() );

        // Make case to force first.augmented_byte_sum() to be zero
        adler32_computer  c7, c8, c9;

        c7.process_byte_copies( 1u, 65520u );
        BOOST_REQUIRE_EQUAL( c7.augmented_byte_sum(), 0u );
        c8.process_byte( 15u );
        BOOST_CHECK_EQUAL( c8.augmented_byte_sum(), 16u );

        c9.process_byte_copies( 1u, 65520u );
        c9.process_byte( 15u );
        BOOST_CHECK_EQUAL( c9.augmented_byte_sum(), 15u );
        c7.append_computation( c8, 1 );
        BOOST_CHECK_EQUAL( c7.augmented_byte_sum(), c9.augmented_byte_sum() );
        BOOST_CHECK_EQUAL( c7.sum_of_byte_sums(), c9.sum_of_byte_sums() );
        BOOST_CHECK_EQUAL( c7.checksum(), c9.checksum() );
    }
}

// Equality operators test
BOOST_AUTO_TEST_CASE( adler32_computer_equality_test )
{
    adler32_computer  c1, c2, c3, c4;

    // Make some non-default
    c3.process_byte( 0x55u );
    c4.process_byte( 0x55u );

    // Compare
    BOOST_CHECK( c1 == c1 ); BOOST_CHECK( c1 == c2 ); BOOST_CHECK( c1 != c3 );
     BOOST_CHECK( c1 != c4 );
    BOOST_CHECK( c2 == c1 ); BOOST_CHECK( c2 == c2 ); BOOST_CHECK( c2 != c3 );
     BOOST_CHECK( c2 != c4 );
    BOOST_CHECK( c3 != c1 ); BOOST_CHECK( c3 != c2 ); BOOST_CHECK( c3 == c3 );
     BOOST_CHECK( c3 == c4 );
    BOOST_CHECK( c4 != c1 ); BOOST_CHECK( c4 != c2 ); BOOST_CHECK( c4 == c3 );
     BOOST_CHECK( c4 == c4 );

    // Make difference at sum-of-byte-sums level
    c4.process_byte_copies( UCHAR_MAX, 65521u / UCHAR_MAX );
    c4.process_byte( 65521u % UCHAR_MAX );
    BOOST_CHECK_EQUAL( c3.augmented_byte_sum(), c4.augmented_byte_sum() );
    BOOST_CHECK_NE( c3.sum_of_byte_sums(), c4.sum_of_byte_sums() );
    BOOST_CHECK_NE( c3.checksum(), c4.checksum() );
    BOOST_CHECK( c3 != c4 );
}

// Construction from checksum test
BOOST_AUTO_TEST_CASE( adler32_computer_checksum_constructor_test )
{
    // Establish two different computers
    adler32_computer  c1, c2;

    c1.process_byte_copies( 255u, 2 );
    BOOST_REQUIRE_NE( c1, c2 );
    BOOST_CHECK_NE( c1.augmented_byte_sum(), c2.augmented_byte_sum() );
    BOOST_CHECK_NE( c1.sum_of_byte_sums(), c2.sum_of_byte_sums() );
    BOOST_CHECK_NE( c1.checksum(), c2.checksum() );

    // Copy state only through the checksum
    adler32_computer  c3( c1.checksum() );

    BOOST_CHECK_EQUAL( c3, c1 );
    BOOST_CHECK_NE( c3, c2 );
}

// Copy construction and assignments test
BOOST_AUTO_TEST_CASE( adler32_computer_copy_constructor_status_test )
{
    // Get a non-default valued computer
    adler32_computer  c1;

    c1.process_byte( 0x01u );
    BOOST_CHECK_NE( c1.augmented_byte_sum(), 1u );
    BOOST_CHECK_NE( c1.sum_of_byte_sums(), 0u );

    // Copy constructor
    adler32_computer  c2, c3( c1 );

    BOOST_CHECK_EQUAL( c3, c1 );
    BOOST_CHECK_NE( c3, c2 );

    // Assignment operator
    c3 = c2;
    BOOST_CHECK_EQUAL( c3, c2 );
    BOOST_CHECK_NE( c3, c1 );

    // Assignment method
    c3.assign( c1 );
    BOOST_CHECK_EQUAL( c3, c1 );
    BOOST_CHECK_NE( c3, c2 );

    // Reset to default
    c3.reset();
    BOOST_CHECK_EQUAL( c3, c2 );
    BOOST_CHECK_NE( c3, c1 );

    // Swap
    c3.swap( c1 );
    BOOST_CHECK_NE( c3, c2 );
    BOOST_CHECK_EQUAL( c1, c2 );

    // Free-function swap
    boost::coding::swap( c1, c3 );
    BOOST_CHECK_EQUAL( c3, c2 );
    BOOST_CHECK_NE( c1, c2 );

    // Set to new checksum
    c3.configure( c1.checksum() );
    BOOST_CHECK_EQUAL( c3, c1 );
    BOOST_CHECK_NE( c3, c2 );
}

// Threshold(s) test
BOOST_AUTO_TEST_CASE( adler32_computer_threshold_test )
{
    // Go past initial-conditions threshold, assuming byte-sums have been held
    // in a 32-bit register without a reduction step (yet).
    adler32_computer     c;
    unsigned long const  base = 65521u, it = 5803u;

    c.process_byte_copies( UCHAR_MAX, it );
    BOOST_CHECK_EQUAL( c.augmented_byte_sum(), (( UCHAR_MAX % base ) * it + 1u)
     % base );
    BOOST_CHECK_EQUAL( c.sum_of_byte_sums(), (( UCHAR_MAX % base ) * ( it * (it
     + 1u) / 2u % base ) + it) % base );

    // Go past worst-case threshold, assuming byte-sums have been held in a
    // 32-bit register without a reduction step (yet).  The worst-case would be
    // when both sums are at Base - 1, which can be done by entering Base - 2
    // values of 1.  Or, the fastest & simplest way to do it with 8-bit bytes is
    // to enter 128 values of 255, a value of 239, then 128 more values of 255,
    // as discovered by Mark Adler on 2008-Aug-30 in message
    // <news:93740e5f-4ab8-4c1d-b324-3c86466e818a@b30g2000prf.googlegroups.com>.
    // (I don't know the proof of either method, yet.)
    unsigned long const  rt = 5552u, rbase = base - 1u;

    c.reset();
    c.process_byte_copies( 255u, 128 );
    c.process_byte( 239u );
    c.process_byte_copies( 255u, 128 );
    BOOST_REQUIRE_EQUAL( c.augmented_byte_sum(), rbase );
    BOOST_REQUIRE_EQUAL( c.sum_of_byte_sums(), rbase );
    c.process_byte_copies( UCHAR_MAX, rt );
    BOOST_CHECK_EQUAL( c.augmented_byte_sum(), (( UCHAR_MAX % base ) * rt +
     rbase) % base );
    BOOST_CHECK_EQUAL( c.sum_of_byte_sums(), (( UCHAR_MAX % base ) * ( rt * (rt
     + 1u) / 2u % base ) + ( (rt + 1u) * rbase )) % base );
}

// Function-object computation test
BOOST_AUTO_TEST_CASE( adler32_computer_byte_input_test )
{
    using std::for_each;

    // Compare straight usage
    adler32_computer     c1, c2;
    unsigned char const  values[ 3 ] = { 0x37u, 0x48u, 0x1Cu };

    c1.process_bytes( values, 3 );
    BOOST_FOREACH( unsigned char b, values )
        c2.bytes( b );
    BOOST_CHECK_EQUAL( c1, c2 );

    // Use with algorithms
    adler32_computer  c3, c4;

    BOOST_CHECK_EQUAL( c3, c4 );
    for_each( values, values + 3, c3.bytes );
    BOOST_CHECK_EQUAL( c3, c4 );
    BOOST_CHECK_NE( c3, c2 );
    c3.bytes = for_each( values, values + 3, c3.bytes );
    BOOST_CHECK_NE( c3, c4 );
    BOOST_CHECK_EQUAL( c3, c2 );

    // Assignment
    c4.bytes = c3.bytes;
    BOOST_CHECK_EQUAL( c4, c3 );

    // Re-do
    c4.reset();
    BOOST_CHECK_NE( c4, c3 );
    c4.bytes = for_each( values, values + 3, c4.bytes );
    BOOST_CHECK_EQUAL( c4, c3 );
}

// Archiving test
BOOST_AUTO_TEST_CASE( adler32_computer_serialization_test )
{
    // Make differing computers, to compare/contrast against a test computer
    adler32_computer  c1, c2;

    c2.bytes( 0x01u );
    BOOST_REQUIRE_NE( c1, c2 );

    adler32_computer  test = c2;

    BOOST_REQUIRE_NE( test, c1 );
    BOOST_REQUIRE_EQUAL( test, c2 );

    // Write to an archive
    std::stringstream  ss;
    write_xml_archive( ss, test, "test" );

    // Reset the test computer to ensure reading works
    test.reset();
    BOOST_REQUIRE_EQUAL( test, c1 );
    BOOST_REQUIRE_NE( test, c2 );

    // Read from an archive
    BOOST_TEST_MESSAGE( ss.str() );
    read_xml_archive( ss, test, "test" );

    // Confirm the change and proper read-back
    BOOST_CHECK_NE( test, c1 );
    BOOST_CHECK_EQUAL( test, c2 );

    // Maybe an unused computer should be tested
    BOOST_REQUIRE_NE( test, c1 );
    ss.str( "" );
    write_xml_archive( ss, c1, "test" );
    BOOST_TEST_MESSAGE( ss.str() );
    read_xml_archive( ss, test, "test" );
    BOOST_CHECK_EQUAL( test, c1 );
    BOOST_CHECK_NE( test, c2 );
}

// Adler-32 context object tests
BOOST_AUTO_TEST_CASE( adler32_context_test )
{
    using boost::coding::adler32_context;

    // Default construction and production
    adler32_context const  c1;
    adler32_computer       scratch;

    BOOST_CHECK_EQUAL( c1(), scratch.context()() );

    // Equality
    adler32_context  c2;

    BOOST_CHECK( c1 == c2 );
    BOOST_CHECK( !(c2 != c1) );

    // Consumption
    unsigned char const  test_value = 0x02u;

    c2( test_value );
    scratch.process_byte( test_value );
    BOOST_CHECK_EQUAL( c2(), scratch.context()() );

    // Inequality
    BOOST_CHECK( c1 != c2 );
    BOOST_CHECK( !(c2 == c1) );

    // Copy construction
    adler32_context  test( c2 );

    BOOST_CHECK_EQUAL( test, c2 );
    BOOST_REQUIRE_NE( test, c1 );

    // More consumption, but make sure the modulus turns over
    int const  test_length = 65521u / test_value + 1u;

    for ( int  i = test_length ; i ; --i )
        c2( test_value );
    scratch.process_byte_copies( test_value, test_length );
    BOOST_CHECK_EQUAL( c2(), scratch.checksum() );

    // Quick S11N check
    std::stringstream  ss;

    write_xml_archive( ss, c1, "test" );
    BOOST_TEST_MESSAGE( ss.str() );
    read_xml_archive( ss, test, "test" );
    BOOST_CHECK_EQUAL( test, c1 );
    BOOST_CHECK_NE( test, c2 );

    // Assimilation
    test( c2, test_length + 1 );
    BOOST_CHECK_EQUAL( test, c2 );
    BOOST_CHECK_NE( test, c1 );

    // Checksum construction
    test = adler32_context( c1() );
    BOOST_CHECK_EQUAL( test, c1 );
    BOOST_CHECK_NE( test, c2 );
}

// Single-call function test
BOOST_AUTO_TEST_CASE( adler32_function_test )
{
    using boost::coding::adler32;

    // Empty check
    BOOST_CHECK_EQUAL( adler32(NULL, 0u), adler32_empty_data );

    // Check against the Wikipedia sample, which is "Wikipedia" in ASCII
    unsigned char const  wp_sample[] = { 87,105,107, 105,112,101, 100,105,97 };
    std::size_t const    wp_length = sizeof(wp_sample) / sizeof(wp_sample[0]);

    BOOST_CHECK_EQUAL( adler32(wp_sample, wp_length), 0x11E60398ul );

    // Check against another component
    unsigned char const  test_value = 0x6Fu;
    adler32_computer     scratch;

    scratch.process_byte( test_value );
    BOOST_CHECK_EQUAL( adler32(&test_value, 1u), scratch.checksum() );
}

BOOST_AUTO_TEST_SUITE_END()
