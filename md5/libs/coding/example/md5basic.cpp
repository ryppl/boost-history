// Boost MD5 simple example program file  ------------------------------------//

// (C) Copyright 2008 Daryle Walker.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for the library's home page.

#include <boost/coding/md5.hpp>  // for boost::coding::md5

#include <cstring>    // for std::strlen
#include <cstdlib>    // for EXIT_SUCCESS, EXIT_FAILURE
#include <exception>  // for std::exception
#include <iostream>   // for std::cerr, cout
#include <ostream>    // for std::endl


// Using declarations
using std::cerr;
using std::endl;


// Program driver
int
main( int argc, char *argv[] )
try
{
    // Check-sum each command-line string
    for ( int  i = 1 ; i < argc ; ++i )
        std::cout << boost::coding::md5( argv[i], std::strlen(argv[ i ]) ) <<
         endl;

    return EXIT_SUCCESS;
}
catch ( std::exception const &e )
{
    cerr << e.what() << endl;
    return EXIT_FAILURE;
}
catch ( ... )
{
    cerr << "unknown problem" << endl;
    return EXIT_FAILURE;
}
