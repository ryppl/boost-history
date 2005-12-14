//  Boost cstdio_simple_output_test.cpp test file  ---------------------------//

//  Copyright 2005 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   13 Dec 2005  Initial version (Daryle Walker)

#include <boost/io/cstdio/simple_output.hpp>  // for iputc, iputs

#include <boost/test/auto_unit_test.hpp>  // for BOOST_AUTO_UNIT_TEST, etc.

#include <sstream>    // for std::ostringstream
#include <string>     // for std::string
#include <strstream>  // for std::ostrstream


BOOST_AUTO_TEST_SUITE( simple_output_tests );

// Simple character output that successfully writes
BOOST_AUTO_TEST_CASE( iputc_pass_test )
{
    std::ostringstream  oss;
    char const          sample = 'x';

    BOOST_CHECK( boost::io::cstdio::iputc(sample, oss) );
    BOOST_CHECK( oss );
    BOOST_CHECK_EQUAL( oss.str(), std::string(1u, sample) );
}

// Simple character output that fails to write
BOOST_AUTO_TEST_CASE( iputc_fail_test )
{
    char const       sample = 'x';
    char             buffer[ 1 ];
    std::ostrstream  oss( buffer, sizeof(buffer) );

    oss.freeze();     // prevent re-allocations
    oss.put( '\0' );  // exhaust the only element available

    BOOST_CHECK( not boost::io::cstdio::iputc(sample, oss) );
    BOOST_CHECK( not oss );
}

// Simple C-string output that successfully writes
BOOST_AUTO_TEST_CASE( iputs_pass_test )
{
    std::ostringstream  oss;
    char const          sample[] = "Hello world";

    BOOST_CHECK( boost::io::cstdio::iputs(sample, oss) );
    BOOST_CHECK( oss );
    BOOST_CHECK_EQUAL( oss.str(), sample );
}

// Simple C-string output that fails to write
BOOST_AUTO_TEST_CASE( iputs_fail_test )
{
    char const       sample[] = "Hello world";
    char             buffer[ sizeof(sample) / 2u ];
    std::ostrstream  oss( buffer, sizeof(buffer) );

    oss.freeze();  // prevent re-allocations

    BOOST_CHECK( not boost::io::cstdio::iputs(sample, oss) );
    BOOST_CHECK( not oss );
}

BOOST_AUTO_TEST_SUITE_END();
