// Boost MD5 example program file  -------------------------------------------//

// (C) Copyright 2008 Daryle Walker.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for the library's home page.

#include <boost/coding/md5.hpp>  // for boost::coding::md5_computer

#include <boost/lexical_cast.hpp>  // for boost::lexical_cast, bad_lexical_cast

#include <algorithm>  // for std::for_each
#include <exception>  // for std::exception
#include <iostream>   // for std::cerr, cout, cin
#include <iterator>   // for std::istream_iterator
#include <ostream>    // for std::endl


// Error return codes
enum
{
    bad_argument_format = -3,
    bad_argument_count  = -2,
    process_error       = -1,
    program_ok          =  0,
    bad_checksum        = +1
};

// Using declarations
using std::cerr;
using std::endl;


// Main program
int
main
(
    int     argc,
    char *  argv[]
)
try
{
    using boost::coding::md5_digest;
    using std::cout;

    typedef std::istream_iterator<unsigned char>  byte_read_iterator;

    // Perform a MD5 checksum over the bytes read from standard input.
    // (TODO: put code to force std::cin into binary mode)
    boost::coding::md5_computer  checksummer;

    checksummer.bytes = std::for_each( byte_read_iterator(std::cin),
     byte_read_iterator(), checksummer.bytes );

    md5_digest const  fingerprint = checksummer();

    // Use the results based on the program's mode
    switch ( argc )
    {
    case 1:
        // No extra arguments -> act as Un*x-like filter, converting bytes from
        // input to message digest text sent to output
        cout << fingerprint << endl;
        return program_ok;
    case 2:
        // One extra argument -> match the input bytes' checksum to the given
        // argument
        try
        {
            if ( fingerprint == boost::lexical_cast<md5_digest>(argv[ 1 ]) )
            {
                cout << "The checksums matched (as '" << fingerprint << "')." <<
                 endl;
                return program_ok;
            }
            else
            {
                cout << "The checksums did not match (wanted: '" << argv[ 1 ] <<
                 "', got: '" << fingerprint << "')." << endl;
                return bad_checksum;
            }
        }
        catch ( boost::bad_lexical_cast &e )
        {
            cerr << "The given checksum ('" << argv[ 1 ] <<
             "') was not in the md5 format, giving error '" << e.what() << "'."
             << endl;
            return bad_argument_format;
        }
    default:
        // No other argument combinations allowed
        cerr << "Improper number of arguments (0 or 1, not " << (argc - 1) <<
         ")." << endl;
        return bad_argument_count;
    }
}
catch ( std::exception &e )
{
    cerr << "Found an exception with '" << e.what() << "'." << endl;
    return process_error;
}
catch ( ... )
{
    cerr << "Found an unknown exception." << endl;
    return process_error;
}
