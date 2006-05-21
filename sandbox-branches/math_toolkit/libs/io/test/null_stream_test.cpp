//  Boost null_stream_test.cpp test file  ------------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   24 Nov 2003  Initial version (Daryle Walker)

#include <boost/io/null_stream.hpp>  // for ...io::basic_onullstream, etc.
#include <boost/test/unit_test.hpp>  // for main, BOOST_CHECK, etc.

#include <cstddef>  // for std::size_t, NULL
#include <cstdio>   // for EOF


// Unit test for null-stream-buffer
void
nullbuf_unit_test
(
)
{
    using std::size_t;

    // Initialization
    boost::io::nullbuf  nb;

    BOOST_CHECK_EQUAL( 0, nb.pcount() );

    // Single-character output
    BOOST_CHECK_EQUAL( static_cast<int>('A'), nb.sputc('A') );
    BOOST_CHECK_EQUAL( 1, nb.pcount() );

    // Multiple-character output
    char const    message[] = "Hi there, Boosters!\n";
    size_t const  message_size = sizeof( message ) / sizeof( message[0] );
    size_t const  message_length = message_size - 1;

    BOOST_CHECK_EQUAL( message_length, nb.sputn(message, message_length) );
    BOOST_CHECK_EQUAL( (1 + message_length), nb.pcount() );

    BOOST_CHECK_EQUAL( 0, nb.sputn(NULL, message_length) );
    BOOST_CHECK_EQUAL( (1 + message_length), nb.pcount() );

    BOOST_CHECK_EQUAL( 0, nb.sputn(message, -3) );
    BOOST_CHECK_EQUAL( (1 + message_length), nb.pcount() );

    BOOST_CHECK_EQUAL( 0, nb.sputn(NULL, -3) );
    BOOST_CHECK_EQUAL( (1 + message_length), nb.pcount() );
}

// Unit test for null output stream
void
onullstream_unit_test
(
)
{
    using std::size_t;

    // Initialization
    boost::io::onullstream  ons;

    BOOST_CHECK_EQUAL( 0, ons.rdbuf()->pcount() );

    // Multiple-character output
    char const    message[] = "Hi there, Boosters!\n";
    size_t const  message_size = sizeof( message ) / sizeof( message[0] );
    size_t const  message_length = message_size - 1;

    BOOST_CHECK( ons.write(message, message_length) );
    BOOST_CHECK_EQUAL( message_length, ons.rdbuf()->pcount() );

    // Single-character output
    BOOST_CHECK( ons.put('B') );
    BOOST_CHECK_EQUAL( (message_length + 1), ons.rdbuf()->pcount() );
}

// Unit test for null input stream
void
inullstream_unit_test
(
)
{
    boost::io::inullstream  ins;
    char                    temp = '\0';
    boost::io::nullbuf      temp_nb;

    BOOST_CHECK_EQUAL( EOF, ins.get() );
    BOOST_CHECK( !ins );
    ins.clear();

    BOOST_CHECK( !ins.get(temp) );
    ins.clear();

    BOOST_CHECK( !ins.get(&temp, 1) );
    ins.clear();

    BOOST_CHECK( !ins.get(temp_nb) );
    ins.clear();
    BOOST_CHECK_EQUAL( 0, temp_nb.pcount() );

    BOOST_CHECK( !ins.getline(&temp, 1) );
    ins.clear();

    BOOST_CHECK( ins.ignore() );  // sets only EOF, not "fail" nor "bad"
    BOOST_CHECK( ins.eof() );
    ins.clear();

    BOOST_CHECK_EQUAL( EOF, ins.peek() );
    BOOST_CHECK( ins );
    ins.clear();  // my system sets EOF (by mistake?)!

    BOOST_CHECK( !ins.read(&temp, 1) );
    ins.clear();
    BOOST_CHECK_EQUAL( 0, ins.readsome(&temp, 1) );
    BOOST_CHECK( ins );  // "showmanyc" still returns 0, not -1; stream's good

    BOOST_CHECK( !ins.putback('A') );
    ins.clear();
    BOOST_CHECK( !ins.unget() );
    ins.clear();
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
     = BOOST_TEST_SUITE( "null-stream test" );

    test->add( BOOST_TEST_CASE(nullbuf_unit_test) );
    test->add( BOOST_TEST_CASE(onullstream_unit_test) );
    test->add( BOOST_TEST_CASE(inullstream_unit_test) );

    return test;
}
