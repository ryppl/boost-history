//  Boost value_stream_test.cpp test file  -----------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   29 Dec 2003  Initial version (Daryle Walker)

#include <boost/io/value_stream.hpp>  // for ...io::basic_ivaluestream, etc.
#include <boost/test/unit_test.hpp>   // for main, BOOST_CHECK, etc.

#include <algorithm>  // for std::count
#include <cstddef>    // for std::size_t, NULL
#include <cstdio>     // for EOF
#include <ios>        // for std::streamsize
#include <limits>     // for std::numeric_limits
#include <string>     // for std::string


// Unit test for value-stream-buffer
void
valuebuf_unit_test
(
)
{
    using boost::io::valuebuf;
    using std::count;
    using std::numeric_limits;
    using std::size_t;
    using std::streamsize;

    // Initialization
    valuebuf  vb;

    BOOST_CHECK_EQUAL( 0, vb.gcount() );
    BOOST_CHECK_EQUAL( '\0', vb.gvalue() );

    // Availability check
    BOOST_CHECK_EQUAL( numeric_limits<streamsize>::max(), vb.in_avail() );
    BOOST_CHECK_EQUAL( 0, vb.gcount() );

    // Single-character input
    BOOST_CHECK_EQUAL( (int)'\0', vb.sgetc() );
    BOOST_CHECK_EQUAL( 0, vb.gcount() );

    BOOST_CHECK_EQUAL( (int)'\0', vb.sbumpc() );
    BOOST_CHECK_EQUAL( 1, vb.gcount() );

    BOOST_CHECK_EQUAL( (int)'\0', vb.snextc() );
    BOOST_CHECK_EQUAL( 2, vb.gcount() );

    BOOST_CHECK_EQUAL( (int)'\0', vb.sgetc() );
    BOOST_CHECK_EQUAL( 2, vb.gcount() );

    // Multiple-character input
    char          buffer[] = { 'A', 'B', 'C', 'D' };
    size_t const  buffer_size = sizeof( buffer ) / sizeof( buffer[0] );

    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, '\0') );

    BOOST_CHECK_EQUAL( buffer_size, vb.sgetn(buffer, buffer_size) );
    BOOST_CHECK_EQUAL( 2 + buffer_size, vb.gcount() );
    BOOST_CHECK_EQUAL( buffer_size, count(buffer, buffer + buffer_size, '\0') );

    BOOST_CHECK_EQUAL( 0, vb.sgetn(NULL, buffer_size) );
    BOOST_CHECK_EQUAL( 2 + buffer_size, vb.gcount() );

    BOOST_CHECK_EQUAL( 0, vb.sgetn(buffer, -3) );
    BOOST_CHECK_EQUAL( 2 + buffer_size, vb.gcount() );
    BOOST_CHECK_EQUAL( buffer_size, count(buffer, buffer + buffer_size, '\0') );

    BOOST_CHECK_EQUAL( 0, vb.sgetn(NULL, -3) );
    BOOST_CHECK_EQUAL( 2 + buffer_size, vb.gcount() );

    // Put-back support
    BOOST_CHECK_EQUAL( (int)'\0', vb.sungetc() );
    BOOST_CHECK_EQUAL( buffer_size + 1, vb.gcount() );

    BOOST_CHECK_EQUAL( (int)'\0', vb.sputbackc('\0') );
    BOOST_CHECK_EQUAL( buffer_size, vb.gcount() );

    BOOST_CHECK_EQUAL( EOF, vb.sputbackc('&') );
    BOOST_CHECK_EQUAL( buffer_size, vb.gcount() );

    // Another check for the operations that don't change 'gcount'
    BOOST_CHECK_EQUAL( numeric_limits<streamsize>::max(), vb.in_avail() );
    BOOST_CHECK_EQUAL( buffer_size, vb.gcount() );

    BOOST_CHECK_EQUAL( (int)'\0', vb.sgetc() );
    BOOST_CHECK_EQUAL( buffer_size, vb.gcount() );

    // Re-do these operations with a non-default character value
    valuebuf  vb2( '2' );

    BOOST_CHECK_EQUAL( 0, vb2.gcount() );
    BOOST_CHECK_EQUAL( '2', vb2.gvalue() );

    BOOST_CHECK_EQUAL( numeric_limits<streamsize>::max(), vb2.in_avail() );
    BOOST_CHECK_EQUAL( 0, vb2.gcount() );

    BOOST_CHECK_EQUAL( EOF, vb2.sputbackc('+') );
    BOOST_CHECK_EQUAL( 0, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sputbackc('2') );
    BOOST_CHECK_EQUAL( -1, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sungetc() );
    BOOST_CHECK_EQUAL( -2, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sgetc() );
    BOOST_CHECK_EQUAL( -2, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.snextc() );
    BOOST_CHECK_EQUAL( -1, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sgetc() );
    BOOST_CHECK_EQUAL( -1, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sbumpc() );
    BOOST_CHECK_EQUAL( 0, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sgetc() );
    BOOST_CHECK_EQUAL( 0, vb2.gcount() );

    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, '2') );

    BOOST_CHECK_EQUAL( buffer_size, vb2.sgetn(buffer, buffer_size) );
    BOOST_CHECK_EQUAL( buffer_size, vb2.gcount() );
    BOOST_CHECK_EQUAL( buffer_size, count(buffer, buffer + buffer_size, '2') );

    BOOST_CHECK_EQUAL( numeric_limits<streamsize>::max(), vb2.in_avail() );
    BOOST_CHECK_EQUAL( buffer_size, vb2.gcount() );

    BOOST_CHECK_EQUAL( (int)'2', vb2.sgetc() );
    BOOST_CHECK_EQUAL( buffer_size, vb2.gcount() );
}

// Unit test for value input stream
void
ivaluestream_unit_test
(
)
{
    using std::count;
    using std::fill_n;
    using std::string;

    boost::io::ivaluestream  ivs( 'q' );

    BOOST_CHECK_EQUAL( 0, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( 'q', ivs.rdbuf()->gvalue() );

    BOOST_CHECK_EQUAL( 0, ivs.gcount() );
    BOOST_CHECK( ivs );

    BOOST_CHECK_EQUAL( (int)'q', ivs.get() );
    BOOST_CHECK_EQUAL( 1, ivs.rdbuf()->gcount() );
    BOOST_CHECK( ivs );

    char  temp = '\0';
    BOOST_CHECK( ivs.get(temp) );
    BOOST_CHECK_EQUAL( 'q', temp );
    BOOST_CHECK_EQUAL( 2, ivs.rdbuf()->gcount() );

    char               buffer[] = { 'A', 'B', 'C', 'D' };
    std::size_t const  buffer_size = sizeof( buffer ) / sizeof( buffer[0] );
    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, 'q') );
    BOOST_CHECK( ivs.get(buffer, buffer_size) );
    BOOST_CHECK_EQUAL( buffer_size + 1, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( string(buffer_size - 1, 'q'), buffer );

    fill_n( buffer, buffer_size, '4' );
    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, 'q') );
    BOOST_CHECK( !ivs.getline(buffer, buffer_size) );
    BOOST_CHECK_EQUAL( 2 * buffer_size, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( string(buffer_size - 1, 'q'), buffer );
    ivs.clear();

    fill_n( buffer, buffer_size, '5' );
    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, 'q') );
    BOOST_CHECK( !ivs.get(buffer, buffer_size, 'q') );
    BOOST_CHECK_EQUAL( 2 * buffer_size, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( string(""), buffer );
    ivs.clear();

    fill_n( buffer, buffer_size, '6' );
    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, 'q') );
    BOOST_CHECK( ivs.getline(buffer, buffer_size, 'q') );
    BOOST_CHECK_EQUAL( 2 * buffer_size + 1, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( string(""), buffer );
    ivs.clear();

    // skipped istream.get( streambuf, etc. ) tests, for now....

    BOOST_CHECK( ivs.ignore() );
    BOOST_CHECK_EQUAL( 2 * buffer_size + 2, ivs.rdbuf()->gcount() );

    BOOST_CHECK( ivs.ignore(3, (int)'q') );
    BOOST_CHECK_EQUAL( 2 * buffer_size + 3, ivs.rdbuf()->gcount() );

    BOOST_CHECK( ivs.ignore(2, (int)'.') );
    BOOST_CHECK_EQUAL( 2 * buffer_size + 5, ivs.rdbuf()->gcount() );

    BOOST_CHECK_EQUAL( (int)'q', ivs.peek() );
    BOOST_CHECK_EQUAL( 2 * buffer_size + 5, ivs.rdbuf()->gcount() );

    fill_n( buffer, buffer_size, '7' );
    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, 'q') );
    BOOST_CHECK( ivs.read(buffer, buffer_size) );
    BOOST_CHECK_EQUAL( 3 * buffer_size + 5, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( buffer_size, count(buffer, buffer + buffer_size, 'q') );

    fill_n( buffer, buffer_size, '8' );
    BOOST_CHECK_EQUAL( 0, count(buffer, buffer + buffer_size, 'q') );
    BOOST_CHECK_EQUAL( buffer_size, ivs.readsome(buffer, buffer_size) );
    BOOST_CHECK( ivs );
    BOOST_CHECK_EQUAL( 4 * buffer_size + 5, ivs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( buffer_size, count(buffer, buffer + buffer_size, 'q') );

    BOOST_CHECK( ivs.putback('q') );
    BOOST_CHECK_EQUAL( 4 * buffer_size + 4, ivs.rdbuf()->gcount() );

    BOOST_CHECK( !ivs.putback('S') );
    BOOST_CHECK_EQUAL( 4 * buffer_size + 4, ivs.rdbuf()->gcount() );
    ivs.clear();

    BOOST_CHECK( ivs.unget() );
    BOOST_CHECK_EQUAL( 4 * buffer_size + 3, ivs.rdbuf()->gcount() );
}

// Unit test for value output stream
void
ovaluestream_unit_test
(
)
{
    // Initialization
    boost::io::ovaluestream  ovs;

    BOOST_CHECK_EQUAL( 0, ovs.rdbuf()->gcount() );
    BOOST_CHECK_EQUAL( '\0', ovs.rdbuf()->gvalue() );

    // Multiple-character output
    BOOST_CHECK( !ovs.write("Hi", 2) );
    BOOST_CHECK_EQUAL( 0, ovs.rdbuf()->gcount() );

    // Single-character output
    BOOST_CHECK( !ovs.put('B') );
    BOOST_CHECK_EQUAL( 0, ovs.rdbuf()->gcount() );
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
     = BOOST_TEST_SUITE( "value-stream test" );

    test->add( BOOST_TEST_CASE(valuebuf_unit_test) );
    test->add( BOOST_TEST_CASE(ivaluestream_unit_test) );
    test->add( BOOST_TEST_CASE(ovaluestream_unit_test) );

    return test;
}
