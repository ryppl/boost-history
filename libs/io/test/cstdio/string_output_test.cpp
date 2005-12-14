//  Boost string_output_test.cpp test file  ----------------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   13 Dec 2005  Initial version (Daryle Walker)

#include <boost/io/cstdio/string_output.hpp>  // for iputs

#include <boost/test/auto_unit_test.hpp>  // for BOOST_AUTO_UNIT_TEST, etc.

#include <sstream>    // for std::ostringstream
#include <string>     // for std::string
#include <strstream>  // for std::ostrstream


BOOST_AUTO_TEST_SUITE( string_output_tests );

// C++-string output that successfully writes
BOOST_AUTO_TEST_CASE( cpp_iputs_pass_test )
{
    std::ostringstream  oss;
    std::string const   sample( "Hello world" );

    BOOST_CHECK( boost::io::cstdio::iputs(sample, oss) );
    BOOST_CHECK( oss );
    BOOST_CHECK_EQUAL( oss.str(), sample );
}

// C++-string output that fails to write
BOOST_AUTO_TEST_CASE( cpp_iputs_fail_test )
{
    char const         sample_base[] = "Hello world";
    std::string const  sample( sample_base );
    char               buffer[ sizeof(sample_base) / 2u ];
    std::ostrstream    oss( buffer, sizeof(buffer) );

    oss.freeze();

    BOOST_CHECK( not boost::io::cstdio::iputs(sample, oss) );
    BOOST_CHECK( not oss );
}

BOOST_AUTO_TEST_SUITE_END();
