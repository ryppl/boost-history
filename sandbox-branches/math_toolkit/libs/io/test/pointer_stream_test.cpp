//  Boost pointer_stream_test.cpp test file  ---------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   04 Nov 2003  Initial version (Daryle Walker)

#include <boost/io/pointer_stream.hpp>  // for ...::basic_ipointerstream, etc.
#include <boost/test/unit_test.hpp>     // for main, BOOST_CHECK, etc.

#include <cstddef>  // for std::size_t
#include <ios>      // for std::ios_base
#include <ostream>  // for std::endl
#include <string>   // for std::string, std::char_traits


// Unit test for pointer stream-buffer
void
pointerbuf_unit_test
(
)
{
    using std::ios_base;

    std::size_t const      buffer_length = 7;
    char                   buffer[ buffer_length ] = { '\0' };
    boost::io::pointerbuf  pb( buffer, buffer + buffer_length );

    // Initialization
    BOOST_CHECK_EQUAL( pb.begin_pointer(), buffer );
    BOOST_CHECK_EQUAL( pb.end_pointer(), buffer + buffer_length );

    BOOST_CHECK_EQUAL( 0, pb.pcount() );
    BOOST_CHECK_EQUAL( 0, pb.gcount() );

    BOOST_CHECK_EQUAL( (ios_base::in | ios_base::out), pb.open_mode() );

    // New buffer
    char               buffer2[] = "Wow, pointers!";
    std::size_t const  buffer2_length = sizeof(buffer2) / sizeof(buffer2[0]) - 1;

    pb.pubsetbuf( buffer2, buffer2_length );
    BOOST_CHECK_EQUAL( pb.begin_pointer(), buffer2 );
    BOOST_CHECK_EQUAL( pb.end_pointer(), buffer2 + buffer2_length );
    BOOST_CHECK_EQUAL( 0, pb.pcount() );
    BOOST_CHECK_EQUAL( 0, pb.gcount() );
    BOOST_CHECK_EQUAL( (ios_base::in | ios_base::out), pb.open_mode() );
}

// Unit test for pointer output stream
void
opointerstream_unit_test
(
)
{
    using std::ios_base;
    using std::string;

    typedef std::char_traits<char>  traits_type;
    typedef traits_type::off_type   off_type;
    typedef traits_type::pos_type   pos_type;

    std::size_t const          buffer_length = 7;
    char                       buffer[ buffer_length ] = { '\0' };
    boost::io::opointerstream  ops( buffer, buffer + buffer_length );

    // Initialization
    BOOST_CHECK_EQUAL( ops.begin_pointer(), buffer );
    BOOST_CHECK_EQUAL( ops.end_pointer(), buffer + buffer_length );

    BOOST_CHECK_EQUAL( 0, ops.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ops.rdbuf()->gcount() );

    BOOST_CHECK_EQUAL( ios_base::out, ops.rdbuf()->open_mode() );

    // Output
    ops << "Hi" << std::endl;
    BOOST_CHECK_EQUAL( "Hi\n", string(&buffer[ 0 ]) );
    BOOST_CHECK_EQUAL( pos_type(off_type( 3 )), ops.tellp() );

    BOOST_CHECK_EQUAL( 3, ops.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ops.rdbuf()->gcount() );

    // Seeking and output
    ops.seekp( pos_type(off_type( 1 )) );
    ops.put( 'a' );
    BOOST_CHECK_EQUAL( "Ha\n", string(&buffer[ 0 ]) );
    BOOST_CHECK_EQUAL( pos_type(off_type( 2 )), ops.tellp() );
    BOOST_CHECK_EQUAL( 2, ops.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ops.rdbuf()->gcount() );

    ops.seekp( off_type(-2), ios_base::cur );
    ops.put( 'L' );
    BOOST_CHECK_EQUAL( "La\n", string(&buffer[ 0 ]) );
    BOOST_CHECK_EQUAL( pos_type(off_type( 1 )), ops.tellp() );
    BOOST_CHECK_EQUAL( 1, ops.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ops.rdbuf()->gcount() );
}

// Unit test for pointer input stream
void
ipointerstream_unit_test
(
)
{
    using std::ios_base;

    typedef std::char_traits<char>  traits_type;
    typedef traits_type::off_type   off_type;
    typedef traits_type::pos_type   pos_type;

    char                       buffer[] = "Hi there, Boosters!\n";
    std::size_t const          buffer_length = sizeof(buffer) / sizeof(buffer[0]) - 1;
    boost::io::ipointerstream  ips( buffer, buffer + buffer_length );

    // Initialization
    BOOST_CHECK_EQUAL( ips.begin_pointer(), buffer );
    BOOST_CHECK_EQUAL( ips.end_pointer(), buffer + buffer_length );

    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->gcount() );

    BOOST_CHECK_EQUAL( ios_base::in, ips.rdbuf()->open_mode() );

    // Input
    std::string  scratch;

    ips >> scratch;
    BOOST_CHECK_EQUAL( "Hi", scratch );
    BOOST_CHECK_EQUAL( static_cast<int>(' '), ips.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 2 )), ips.tellg() );

    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 2, ips.rdbuf()->gcount() );

    // Seeking and input
    ips.seekg( pos_type(off_type( 4 )) );
    BOOST_CHECK_EQUAL( static_cast<int>('h'), ips.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 4 )), ips.tellg() );
    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 4, ips.rdbuf()->gcount() );

    ips.seekg( off_type(8 - static_cast<int>( buffer_length )), ios_base::end );
    BOOST_CHECK_EQUAL( static_cast<int>(','), ips.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 8 )), ips.tellg() );
    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 8, ips.rdbuf()->gcount() );

    // Put-back
    ips.unget();
    BOOST_CHECK_EQUAL( static_cast<int>('e'), ips.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 7 )), ips.tellg() );
    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 7, ips.rdbuf()->gcount() );

    ips.putback( 'R' );
    BOOST_CHECK_EQUAL( static_cast<int>('R'), ips.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 6 )), ips.tellg() );
    BOOST_CHECK_EQUAL( 0, ips.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 6, ips.rdbuf()->gcount() );
}

// Unit test for pointer combined stream
void
pointerstream_unit_test
(
)
{
    using std::ios_base;
    using std::string;

    typedef std::char_traits<char>  traits_type;
    typedef traits_type::off_type   off_type;
    typedef traits_type::pos_type   pos_type;

    std::size_t const         buffer_length = 100;
    char                      buffer[ buffer_length ] = { '\0' };
    boost::io::pointerstream  ps( buffer, buffer + buffer_length );

    // Initialization
    BOOST_CHECK_EQUAL( ps.begin_pointer(), buffer );
    BOOST_CHECK_EQUAL( ps.end_pointer(), buffer + buffer_length );

    BOOST_CHECK_EQUAL( 0, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ps.rdbuf()->gcount() );

    BOOST_CHECK_EQUAL( (ios_base::in | ios_base::out), ps.rdbuf()->open_mode() );

    // Output
    ps << "Hi there, Boosters!" << std::endl;
    BOOST_CHECK_EQUAL( "Hi there, Boosters!\n", string(&buffer[ 0 ]) );
    BOOST_CHECK_EQUAL( pos_type(off_type( 20 )), ps.tellp() );

    BOOST_CHECK_EQUAL( 20, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ps.rdbuf()->gcount() );

    // Input
    string  scratch;

    ps.seekg( 0, ios_base::beg );
    BOOST_CHECK_EQUAL( 20, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 0, ps.rdbuf()->gcount() );

    ps >> scratch;
    BOOST_CHECK_EQUAL( "Hi", scratch );
    BOOST_CHECK_EQUAL( static_cast<int>(' '), ps.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 2 )), ps.tellg() );

    BOOST_CHECK_EQUAL( 20, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 2, ps.rdbuf()->gcount() );

    // Bad positioning
    BOOST_CHECK_EQUAL( pos_type(off_type( -1 )), ps.rdbuf()->pubseekoff(+4, ios_base::cur, ( ios_base::in | ios_base::out )) );
    BOOST_CHECK_EQUAL( 20, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 2, ps.rdbuf()->gcount() );

    // Input & output interference
    ps.rdbuf()->pubseekpos( pos_type(off_type( 10 )), (ios_base::in | ios_base::out) );
    BOOST_CHECK_EQUAL( 10, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 10, ps.rdbuf()->gcount() );

    BOOST_CHECK( ps.put('R') );
    BOOST_CHECK_EQUAL( static_cast<int>('R'), ps.get() );
    BOOST_CHECK_EQUAL( 11, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 11, ps.rdbuf()->gcount() );

    BOOST_CHECK( ps.unget() );
    BOOST_CHECK_EQUAL( 11, ps.rdbuf()->pcount() );
    BOOST_CHECK_EQUAL( 10, ps.rdbuf()->gcount() );

    ps >> scratch;
    BOOST_CHECK_EQUAL( "Roosters!", scratch );
}

// Unit test for pointer-to-const stream-buffer
void
constpointerbuf_unit_test
(
)
{
    using std::ios_base;

    std::size_t const           buffer_length = 7;
    char const                  buffer[ buffer_length ] = { '\0' };
    boost::io::constpointerbuf  pb( buffer, buffer + buffer_length );

    // Initialization
    BOOST_CHECK_EQUAL( pb.begin_pointer(), buffer );
    BOOST_CHECK_EQUAL( pb.end_pointer(), buffer + buffer_length );

    BOOST_CHECK_EQUAL( 0, pb.gcount() );

    // New buffer
    char const         buffer2[] = "Wow, pointers!";
    std::size_t const  buffer2_length = sizeof(buffer2) / sizeof(buffer2[0]) - 1;

    pb.pubsetbuf( buffer2, buffer2_length );
    BOOST_CHECK_EQUAL( pb.begin_pointer(), buffer2 );
    BOOST_CHECK_EQUAL( pb.end_pointer(), buffer2 + buffer2_length );
    BOOST_CHECK_EQUAL( 0, pb.gcount() );
}

// Unit test for pointer-to-const input stream
void
iconstpointerstream_unit_test
(
)
{
    typedef std::char_traits<char>  traits_type;
    typedef traits_type::off_type   off_type;
    typedef traits_type::pos_type   pos_type;

    char const                      buffer[] = "Hi there, Boosters!\n";
    std::size_t const               buffer_length = sizeof(buffer) / sizeof(buffer[0]) - 1;
    boost::io::iconstpointerstream  icps( buffer, buffer + buffer_length );

    // Initialization
    BOOST_CHECK_EQUAL( icps.begin_pointer(), buffer );
    BOOST_CHECK_EQUAL( icps.end_pointer(), buffer + buffer_length );

    BOOST_CHECK_EQUAL( 0, icps.rdbuf()->gcount() );

    // Input
    std::string  scratch;

    icps >> scratch;
    BOOST_CHECK_EQUAL( "Hi", scratch );
    BOOST_CHECK_EQUAL( static_cast<int>(' '), icps.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 2 )), icps.tellg() );

    BOOST_CHECK_EQUAL( 2, icps.rdbuf()->gcount() );

    // Seeking and input
    icps.seekg( pos_type(off_type( 4 )) );
    BOOST_CHECK_EQUAL( static_cast<int>('h'), icps.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 4 )), icps.tellg() );
    BOOST_CHECK_EQUAL( 4, icps.rdbuf()->gcount() );

    icps.seekg( off_type(8 - static_cast<int>( buffer_length )), std::ios_base::end );
    BOOST_CHECK_EQUAL( static_cast<int>(','), icps.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 8 )), icps.tellg() );
    BOOST_CHECK_EQUAL( 8, icps.rdbuf()->gcount() );

    // Put-back
    icps.unget();
    BOOST_CHECK_EQUAL( static_cast<int>('e'), icps.peek() );
    BOOST_CHECK_EQUAL( pos_type(off_type( 7 )), icps.tellg() );
    BOOST_CHECK_EQUAL( 7, icps.rdbuf()->gcount() );

    BOOST_CHECK( !icps.putback('R') );
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
     = BOOST_TEST_SUITE( "pointer stream test" );

    test->add( BOOST_TEST_CASE(pointerbuf_unit_test) );
    test->add( BOOST_TEST_CASE(opointerstream_unit_test) );
    test->add( BOOST_TEST_CASE(ipointerstream_unit_test) );
    test->add( BOOST_TEST_CASE(pointerstream_unit_test) );
    test->add( BOOST_TEST_CASE(constpointerbuf_unit_test) );
    test->add( BOOST_TEST_CASE(iconstpointerstream_unit_test) );

    return test;
}
