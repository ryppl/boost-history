//  Boost iomanip_test.cpp test file  ----------------------------------------//

//  (C) Copyright Daryle Walker 2002.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  Revision History
//   24 Jul 2002  Initial version (Daryle Walker)

#define  BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>  // main, BOOST_TEST, etc.

#include <boost/cstdlib.hpp>     // for boost::exit_success
#include <boost/io/iomanip.hpp>  // for boost::io::newl, etc.

#include <sstream>  // for std::stringstream
#include <string>   // for std::string


// Test program
int
test_main
(
    int         ,   // "argc" is unused
    char *      []  // "argv" is unused
)
{
    using boost::io::newl;
    using boost::io::skipl;

    std::stringstream  ss;
    ss << "Hello" << newl << "There";
    BOOST_TEST( ss.str() == "Hello\nThere" );

    std::string  s;
    ss.seekg( 0 );
    ss >> skipl >> s;
    BOOST_TEST( s == "There" );

    return boost::exit_success;
}
