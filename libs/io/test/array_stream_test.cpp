//  Boost array_stream_test.cpp test file  -----------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   19 Jul 2003  Initial version (Daryle Walker)

#include <boost/bind.hpp>             // for boost::bind, _1
#include <boost/io/array_stream.hpp>  // for boost::io::basic_array_istream, etc.
#include <boost/test/unit_test.hpp>   // for main, BOOST_CHECK, etc.

#include <algorithm>   // for std::count_if, std::equal
#include <cstddef>     // for std::size_t
#include <ios>         // for std::ios_base, std::streamsize
#include <iosfwd>      // for std::ios
#include <list>        // for std::list
#include <string>      // for std::string, std::char_traits


// Global objects
char const         alphabet[] = "abcdefghijklmnopqrstuvwxyz";
std::size_t const  alphabet_length = sizeof(alphabet) / sizeof(alphabet[0]) - 1;

// Types
typedef boost::io::basic_array_streambuf<alphabet_length, char>  astreambuf;
typedef boost::io::basic_array_istream<alphabet_length, char>    aistream;
typedef boost::io::basic_array_ostream<alphabet_length, char>    aostream;
typedef boost::io::basic_array_stream<alphabet_length, char>     aiostream;


// Unit test for streambuf-wrappers
void
streambuf_wrapping_unit_test
(
)
{
    using std::ios;

    // Check initial settings for each wrapper
    aostream      aos;
    astreambuf &  aosb = *aos.rdbuf();
    BOOST_CHECK( aos.is_using_internal_streambuf() );
    BOOST_CHECK_EQUAL( aos.rdbuf(), &aosb );
    BOOST_CHECK_EQUAL( aos.rdbuf(), aos.ios::rdbuf() );

    aistream      ais;
    astreambuf &  aisb = *ais.rdbuf();
    BOOST_CHECK( ais.is_using_internal_streambuf() );
    BOOST_CHECK_EQUAL( ais.rdbuf(), &aisb );
    BOOST_CHECK_EQUAL( ais.rdbuf(), ais.ios::rdbuf() );

    aiostream     as;
    astreambuf &  asb = *as.rdbuf();
    BOOST_CHECK( as.is_using_internal_streambuf() );
    BOOST_CHECK_EQUAL( as.rdbuf(), &asb );
    BOOST_CHECK_EQUAL( as.rdbuf(), as.ios::rdbuf() );

    // Now change a stream's buffer and recheck
    ais.ios::rdbuf( &asb );
    BOOST_CHECK( ais.ios::rdbuf() != &aisb );
    BOOST_CHECK( ais.ios::rdbuf() != ais.rdbuf() );
    BOOST_CHECK( !ais.is_using_internal_streambuf() );
}

// Unit test for array streambuf (constructors)
void
astreambuf_constructor_unit_test
(
)
{
    using boost::bind;
    using std::count_if;
    using std::equal;
    using std::streamsize;

    typedef astreambuf::traits_type  traits_type;

    // Default construction and const accessors
    astreambuf          asb_d;
    astreambuf const &  casb_d = asb_d;
    BOOST_CHECK_EQUAL( streamsize(astreambuf::array_size),
     static_cast<streamsize>(count_if( asb_d.array_begin(), asb_d.array_end(),
     bind(traits_type::eq, '\0', _1) )) );
    BOOST_CHECK_EQUAL( streamsize(astreambuf::array_size),
     static_cast<streamsize>(count_if( casb_d.array_begin(), casb_d.array_end(),
     bind(traits_type::eq, '\0', _1) )) );

    // Array-subsequence-based construction
    astreambuf  asb_a( alphabet, alphabet + alphabet_length );
    BOOST_CHECK( equal(asb_a.array_begin(), asb_a.array_end(), alphabet,
     traits_type::eq) );

    // Iterator-based construction
    // (Can't use std::vector, because its iterators may be char*)
    std::list<char>  l( alphabet, alphabet + alphabet_length );
    astreambuf       asb_i( l.begin(), l.end() );
    BOOST_CHECK( equal(asb_i.array_begin(), asb_i.array_end(), alphabet,
     traits_type::eq) );
}

// Unit test for array output stream
void
aostream_unit_test
(
)
{
    aostream  aos;
    aos << alphabet;
    BOOST_CHECK( std::equal(aos.array_begin(), aos.array_end(), alphabet,
     aostream::traits_type::eq) );
}

// Unit test for array input stream
void
aistream_unit_test
(
)
{
    aistream     ais( alphabet, alphabet + alphabet_length );
    std::string  scratch;
    ais >> scratch;
    BOOST_CHECK_EQUAL( scratch, alphabet );
}

// Unit test for array combined stream
void
aiostream_unit_test
(
)
{
    aiostream    aios;
    std::string  scratch;

    // Output
    aios << alphabet;
    BOOST_CHECK( std::equal(aios.array_begin(), aios.array_end(), alphabet,
     aiostream::traits_type::eq) );

    // Input
    aios.seekg( 0, std::ios_base::beg );
    aios >> scratch;
    BOOST_CHECK_EQUAL( scratch, alphabet );
}

// Unit test for read and write counts
void
read_write_count_unit_test
(
)
{
    using std::streamsize;
    using std::string;

    aostream           aos;
    char const         my_string[] = "cde";
    std::size_t const  my_string_length = sizeof(my_string)
     / sizeof(my_string[0]) - 1;

    aos << 'a';
    aos.put( 'b' );
    aos.write( my_string, my_string_length );
    BOOST_CHECK_EQUAL( streamsize(2 + my_string_length),
     aos.rdbuf()->characters_written() );
    BOOST_CHECK_EQUAL( 0, aos.rdbuf()->characters_read() );
    BOOST_CHECK( string(aos.array_begin()) == "abcde" );

    aistream  ais( alphabet, alphabet + alphabet_length );
    char      temp1 = '\0', temp2 = '\0';
    char      temp3[ my_string_length + 1 ] = { '\0' };

    ais >> temp1;
    ais.get( temp2 );
    ais.read( temp3, my_string_length );
    BOOST_CHECK_EQUAL( streamsize(2 + my_string_length),
     ais.rdbuf()->characters_read() );
    BOOST_CHECK_EQUAL( 0, ais.rdbuf()->characters_written() );
    BOOST_CHECK( (temp1 == 'a') && (temp2 == 'b') && (string( temp3 )
     == "cde") );

    ais.unget();
    BOOST_CHECK_EQUAL( streamsize(2 + my_string_length - 1),
     ais.rdbuf()->characters_read() );

    aiostream  aios;
    BOOST_CHECK_EQUAL( 0, aios.rdbuf()->characters_written() );
    BOOST_CHECK_EQUAL( 0, aios.rdbuf()->characters_read() );
}

// Unit test for using and checking the opening mode
void
openmode_unit_test
(
)
{
    using std::ios_base;

    ios_base::openmode const  m_in = ios_base::in;
    ios_base::openmode const  m_out = ios_base::out;
    ios_base::openmode const  m_io = m_in | m_out;
    ios_base::openmode const  m_0 = static_cast<ios_base::openmode>( 0 );
    std::list<char> const     l( alphabet, alphabet + alphabet_length );

    {
        astreambuf  asb1;
        BOOST_CHECK_EQUAL( m_io, asb1.open_mode() );
    }

    {
        astreambuf  asb2( m_in );
        BOOST_CHECK_EQUAL( m_in, asb2.open_mode() );
    }

    {
        astreambuf  asb3( m_out );
        BOOST_CHECK_EQUAL( m_out, asb3.open_mode() );
    }

    {
        astreambuf  asb4( m_io );
        BOOST_CHECK_EQUAL( m_io, asb4.open_mode() );
    }

    {
        astreambuf  asb5( m_0 );
        BOOST_CHECK_EQUAL( m_0, asb5.open_mode() );
    }

    {
        aistream  ais1;
        BOOST_CHECK_EQUAL( m_in, ais1.rdbuf()->open_mode() );
    }

    {
        aistream  ais2( m_in );
        BOOST_CHECK_EQUAL( m_in, ais2.rdbuf()->open_mode() );
    }

    {
        aistream  ais3( alphabet, alphabet + alphabet_length, m_out );
        BOOST_CHECK_EQUAL( m_io, ais3.rdbuf()->open_mode() );
    }

    {
        aistream  ais4( l.begin(), l.end(), m_io );
        BOOST_CHECK_EQUAL( m_io, ais4.rdbuf()->open_mode() );
    }

    {
        aistream  ais5( m_0 );
        BOOST_CHECK_EQUAL( m_in, ais5.rdbuf()->open_mode() );
    }

    {
        aostream  aos1;
        BOOST_CHECK_EQUAL( m_out, aos1.rdbuf()->open_mode() );
    }

    {
        aostream  aos2( l.begin(), l.end(), m_in );
        BOOST_CHECK_EQUAL( m_io, aos2.rdbuf()->open_mode() );
    }

    {
        aostream  aos3( m_out );
        BOOST_CHECK_EQUAL( m_out, aos3.rdbuf()->open_mode() );
    }

    {
        aostream  aos4( alphabet, alphabet + alphabet_length, m_io );
        BOOST_CHECK_EQUAL( m_io, aos4.rdbuf()->open_mode() );
    }

    {
        aostream  aos5( m_0 );
        BOOST_CHECK_EQUAL( m_out, aos5.rdbuf()->open_mode() );
    }

    {
        aiostream  as1;
        BOOST_CHECK_EQUAL( m_io, as1.rdbuf()->open_mode() );
    }

    {
        aiostream  as2( alphabet, alphabet + alphabet_length, m_in );
        BOOST_CHECK_EQUAL( m_in, as2.rdbuf()->open_mode() );
    }

    {
        aiostream  as3( l.begin(), l.end(), m_out );
        BOOST_CHECK_EQUAL( m_out, as3.rdbuf()->open_mode() );
    }

    {
        aiostream  as4( m_io );
        BOOST_CHECK_EQUAL( m_io, as4.rdbuf()->open_mode() );
    }

    {
        aiostream  as5( m_0 );
        BOOST_CHECK_EQUAL( m_0, as5.rdbuf()->open_mode() );
    }
}

// Unit test for seeking functions
void
seek_unit_test
(
)
{
    using std::ios_base;
    using std::string;

    typedef std::char_traits<char>  traits_type;
    typedef traits_type::off_type   off_type;
    typedef traits_type::pos_type   pos_type;

    pos_type const  invalid( off_type(-1) );

    // input shifting tests with input stream
    {
        aistream  ai( alphabet, alphabet + alphabet_length );
        BOOST_CHECK_EQUAL( off_type(0), off_type(ai.tellg()) );
        BOOST_CHECK( traits_type::eq('a',
         traits_type::to_char_type( ai.peek() )) );
        BOOST_CHECK_EQUAL( invalid, ai.rdbuf()->pubseekoff(off_type( 0 ),
         ios_base::cur, ios_base::out) );

        BOOST_CHECK( ai.seekg(off_type(-1), ios_base::end) );
        BOOST_CHECK_EQUAL( off_type(25), off_type(ai.tellg()) );
        BOOST_CHECK( traits_type::eq('z',
         traits_type::to_char_type( ai.peek() )) );

        char  temp = '\0';
        BOOST_CHECK( ai.seekg(off_type(-13), ios_base::cur) );
        BOOST_CHECK_EQUAL( off_type(12), off_type(ai.tellg()) );
        BOOST_CHECK( traits_type::eq('m',
         traits_type::to_char_type( ai.peek() )) );
        BOOST_CHECK( ai.get(temp) );
        BOOST_CHECK( traits_type::eq('m', temp) );
        BOOST_CHECK_EQUAL( off_type(13), off_type(ai.tellg()) );

        BOOST_CHECK( ai.seekg(pos_type( off_type(1) )) );
        BOOST_CHECK_EQUAL( off_type(1), off_type(ai.tellg()) );
        BOOST_CHECK( traits_type::eq('b',
         traits_type::to_char_type( ai.peek() )) );
    }

    // output shifting tests with output stream
    {
        aostream  ao( alphabet, alphabet + alphabet_length );
        BOOST_CHECK_EQUAL( off_type(0), off_type(ao.tellp()) );
        BOOST_CHECK_EQUAL( string(ao.array_begin(), ao.array_end()), alphabet );
        BOOST_CHECK_EQUAL( invalid, ao.rdbuf()->pubseekoff(off_type( 0 ),
         ios_base::cur, ios_base::in) );

        BOOST_CHECK( ao.seekp(off_type(-1), ios_base::end) );
        BOOST_CHECK_EQUAL( off_type(25), off_type(ao.tellp()) );
        BOOST_CHECK( ao.put('Z') );
        BOOST_CHECK_EQUAL( off_type(26), off_type(ao.tellp()) );
        BOOST_CHECK_EQUAL( string(ao.array_begin(), ao.array_end()),
         "abcdefghijklmnopqrstuvwxyZ" );

        BOOST_CHECK( ao.seekp(off_type(-14), ios_base::cur) );
        BOOST_CHECK_EQUAL( off_type(12), off_type(ao.tellp()) );
        BOOST_CHECK( ao.put('M') );
        BOOST_CHECK_EQUAL( off_type(13), off_type(ao.tellp()) );
        BOOST_CHECK_EQUAL( string(ao.array_begin(), ao.array_end()),
         "abcdefghijklMnopqrstuvwxyZ" );

        BOOST_CHECK( ao.seekp(pos_type( off_type(1) )) );
        BOOST_CHECK_EQUAL( off_type(1), off_type(ao.tellp()) );
        BOOST_CHECK( ao.put('B') );
        BOOST_CHECK_EQUAL( off_type(2), off_type(ao.tellp()) );
        BOOST_CHECK_EQUAL( string(ao.array_begin(), ao.array_end()),
         "aBcdefghijklMnopqrstuvwxyZ" );
    }

    // dual shifting tests with stream buffer
    {
        astreambuf  as( alphabet, alphabet + alphabet_length );
        BOOST_CHECK_EQUAL( 0, as.characters_read() );
        BOOST_CHECK_EQUAL( 0, as.characters_written() );

        pos_type  temp = as.pubseekpos( pos_type(off_type( 13 )) );
        BOOST_CHECK_EQUAL( pos_type(off_type( 13 )), temp );
        BOOST_CHECK_EQUAL( 13, as.characters_read() );
        BOOST_CHECK_EQUAL( 13, as.characters_written() );

        temp = as.pubseekoff( off_type(-1), ios_base::end );
        BOOST_CHECK_EQUAL( pos_type(off_type( 25 )), temp );
        BOOST_CHECK_EQUAL( 25, as.characters_read() );
        BOOST_CHECK_EQUAL( 25, as.characters_written() );

        temp = as.pubseekoff( off_type(-5), ios_base::cur );
        BOOST_CHECK_EQUAL( invalid, temp );
        BOOST_CHECK_EQUAL( 25, as.characters_read() );
        BOOST_CHECK_EQUAL( 25, as.characters_written() );

        temp = as.pubseekoff( off_type(-8), ios_base::cur, ios_base::out );
        BOOST_CHECK_EQUAL( pos_type(off_type( 17 )), temp );
        temp = as.pubseekoff( off_type(5), ios_base::beg, ios_base::in );
        BOOST_CHECK_EQUAL( pos_type(off_type( 5 )), temp );
        BOOST_CHECK_EQUAL( 5, as.characters_read() );
        BOOST_CHECK_EQUAL( 17, as.characters_written() );
    }
}


// Unit test program
boost::unit_test_framework::test_suite *
init_unit_test_suite
(
    int         ,   // "argc" is unused
    char *      []  // "argv" is unused
)
{
    boost::unit_test_framework::test_suite *  test
     = BOOST_TEST_SUITE( "array stream test" );

    test->add( BOOST_TEST_CASE(streambuf_wrapping_unit_test) );
    test->add( BOOST_TEST_CASE(astreambuf_constructor_unit_test) );
    test->add( BOOST_TEST_CASE(aostream_unit_test) );
    test->add( BOOST_TEST_CASE(aistream_unit_test) );
    test->add( BOOST_TEST_CASE(aiostream_unit_test) );
    test->add( BOOST_TEST_CASE(read_write_count_unit_test) );
    test->add( BOOST_TEST_CASE(openmode_unit_test) );
    test->add( BOOST_TEST_CASE(seek_unit_test) );

    return test;
}
