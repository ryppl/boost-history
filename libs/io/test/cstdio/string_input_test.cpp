//  Boost string_input_test.cpp test file  -----------------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   18 Dec 2005  Initial version (Daryle Walker)

#include <boost/io/cstdio/string_input.hpp>  // for igets

#include <boost/test/unit_test.hpp>  // for BOOST_AUTO_TEST_CASE, etc.

#include <sstream>  // for std::istringstream
#include <string>   // for std::string


BOOST_AUTO_TEST_SUITE( string_input_tests );

// C++-string input that successfully reads
BOOST_AUTO_TEST_CASE( cpp_igets_pass_test )
{
    using boost::io::cstdio::igets;

    std::string         temp( "Hello There,\nWorld" );
    std::istringstream  iss( temp );

    BOOST_CHECK( igets(temp, iss) );
    BOOST_CHECK_EQUAL( temp, "Hello There," );
    BOOST_CHECK( igets(temp, iss) );
    BOOST_CHECK_EQUAL( temp, "World" );
    BOOST_CHECK( iss.eof() );
}

// C++-string input that fails to read
BOOST_AUTO_TEST_CASE( cpp_igets_fail_test )
{
    std::string         temp( "" );
    std::istringstream  iss( temp );

    BOOST_CHECK( (iss.get(), iss.fail()) );  // start off in a bad state
    BOOST_CHECK( not boost::io::cstdio::igets(temp, iss) );
}

BOOST_AUTO_TEST_SUITE_END();
