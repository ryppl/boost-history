//  Boost iomanip_test.cpp test file  ----------------------------------------//

//  (C) Copyright Daryle Walker 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  Revision History
//   26 Oct 2003  Initial version (Daryle Walker)

#include <boost/io/iomanip.hpp>      // for boost::io::newl, etc.
#include <boost/test/unit_test.hpp>  // for main, BOOST_CHECK_EQUAL, etc.

#include <iomanip>  // for std::setbase, etc.
#include <ios>      // for std::ios_base, std::noskipws
#include <sstream>  // for std::istringstream, std::ostringstream
#include <string>   // for std::string


// Unit test for newl
void
newl_unit_test
(
)
{
    using boost::io::newl;

    std::ostringstream  oss;
    oss << "Hello" << newl << "There";
    BOOST_CHECK_EQUAL( oss.str(), "Hello\nThere" );

    // NOTE: need to test the no-flush part, someday
}

// Unit test for skipl
void
skipl_unit_test
(
)
{
    using boost::io::skipl;

    std::istringstream  iss( "Hello\nThere\nBoosters!" );
    std::string         scratch;

    // Skip over a line of text
    iss >> skipl >> scratch;
    BOOST_CHECK_EQUAL( scratch, "There" );

    // Skip over a (now) blank line
    iss >> skipl >> scratch;
    BOOST_CHECK_EQUAL( scratch, "Boosters!" );
}

// Unit test for general
void
general_unit_test
(
)
{
    using boost::io::general;
    using std::ios_base;

    std::stringstream  ss( "Hello!\n" );

    // Confirm initial state
    BOOST_REQUIRE_MESSAGE( (ios_base::skipws | ios_base::dec) == ss.flags(),
     "non-standard initial stream (flag) state" );
    BOOST_REQUIRE_MESSAGE( 0 == ss.width(),
     "non-standard initial stream (width) state" );
    BOOST_REQUIRE_MESSAGE( 6 == ss.precision(),
     "non-standard initial stream (precision) state" );
    BOOST_REQUIRE_MESSAGE( ' ' == ss.fill(),
     "non-standard initial stream (fill) state" );

    // Change that state (and confirm it)
    ss << std::noskipws << std::setbase( 8 ) << std::setfill<char>( 'G' )
     << std::setprecision( 10 ) << std::setw( 3 );
    BOOST_REQUIRE_MESSAGE( ios_base::oct == ss.flags(),
     "broken stream (flag) state change" );
    BOOST_REQUIRE_MESSAGE( 3 == ss.width(),
     "broken stream (width) state change" );
    BOOST_REQUIRE_MESSAGE( 10 == ss.precision(),
     "broken stream (precision) state change" );
    BOOST_REQUIRE_MESSAGE( 'G' == ss.fill(),
     "broken stream (fill) state change" );

    // Test the change-back
    ss << general;
    BOOST_CHECK_EQUAL( (ios_base::skipws | ios_base::dec), ss.flags() );
    BOOST_CHECK_EQUAL( 0, ss.width() );
    BOOST_CHECK_EQUAL( 6, ss.precision() );
    BOOST_CHECK_EQUAL( ' ', ss.fill() );
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
     = BOOST_TEST_SUITE( "iomanip test" );

    test->add( BOOST_TEST_CASE(newl_unit_test) );
    test->add( BOOST_TEST_CASE(skipl_unit_test) );
    test->add( BOOST_TEST_CASE(general_unit_test) );

    return test;
}
