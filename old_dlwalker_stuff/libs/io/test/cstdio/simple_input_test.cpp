//  Boost cstdio_simple_input_test.cpp test file  ----------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   18 Dec 2005  Initial version (Daryle Walker)

#include <boost/io/cstdio/simple_input.hpp>  // for igetc, iungetc, igets

#include <boost/test/unit_test.hpp>  // for BOOST_AUTO_TEST_CASE, etc.

#include <cstring>    // for std::strcmp
#include <sstream>    // for std::istringstream, std::stringstream
#include <string>     // for std::char_traits, std::string
#include <strstream>  // for std::istrstream


typedef std::char_traits<char>  traits_t;


BOOST_AUTO_TEST_SUITE( simple_input_tests );

// Simple character input that successfully reads
BOOST_AUTO_TEST_CASE( igetc_pass_test )
{
    char const          sample = 'x';
    std::istringstream  iss( std::string(1u, sample) );

    BOOST_CHECK( traits_t::eq_int_type(boost::io::cstdio::igetc( iss ),
     traits_t::to_int_type( sample )) );
}

// Simple character input that fails to read
BOOST_AUTO_TEST_CASE( igetc_fail_test )
{
    std::istringstream  iss( std::string("") );

    BOOST_CHECK( traits_t::eq_int_type(boost::io::cstdio::igetc( iss ),
     traits_t::eof()) );
    BOOST_CHECK( iss.eof() );
}

// Simple character put-back that successfully pushes
BOOST_AUTO_TEST_CASE( iungetc_pass_test )
{
    char const         sample1 = 'x', sample2 = 'y';
    std::stringstream  ss( std::string(1u, sample1) );

    BOOST_CHECK( traits_t::eq_int_type(traits_t::to_int_type( sample1 ),
     ss.get()) );
    BOOST_CHECK( boost::io::cstdio::iungetc(sample2, ss) );
    BOOST_CHECK( traits_t::eq_int_type(traits_t::to_int_type( sample2 ),
     ss.peek()) );
}

// Simple character put-back that fails to push
BOOST_AUTO_TEST_CASE( iungetc_fail_test )
{
    std::istrstream  iss( "Hello world" );

    BOOST_CHECK( traits_t::eq_int_type(traits_t::to_int_type( 'H' ),
     iss.get()) );

    //iss.freeze();

    BOOST_CHECK( not boost::io::cstdio::iungetc('i', iss) );
    BOOST_CHECK( not iss );
}

// Simple C-string input that successfully reads
BOOST_AUTO_TEST_CASE( igets_pass_test )
{
    using boost::io::cstdio::igets;
    using std::strcmp;

    char                temp[] = "00000000000000000000000000";
    std::istringstream  iss( std::string("Hello there,\nWorld") );

    BOOST_CHECK( igets(temp, 3, iss) );
    BOOST_CHECK_EQUAL( 0, strcmp(temp, "He") );

    BOOST_CHECK( igets(temp, 13, iss) );
    BOOST_CHECK( 0 != strcmp(temp, "llo there,\nW") );
    BOOST_CHECK_EQUAL( 0, strcmp(temp, "llo there,\n") );

    BOOST_CHECK( igets(temp, sizeof(temp), iss) );
    BOOST_CHECK_EQUAL( 0, strcmp(temp, "World") );
    BOOST_CHECK( iss.eof() );
}

// Simple C-string input that fails to read
BOOST_AUTO_TEST_CASE( igets_fail_test )
{
    char                temp[ 10 ];
    std::istringstream  iss( std::string("") );

    BOOST_CHECK( (iss.get(), iss.fail()) );  // start off in a bad state
    BOOST_CHECK( not boost::io::cstdio::igets(temp, sizeof(temp), iss) );
}

BOOST_AUTO_TEST_SUITE_END();
