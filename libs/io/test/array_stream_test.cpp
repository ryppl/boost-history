//  Boost array_stream_test.cpp test file  -----------------------------------//

//  (C) Copyright Daryle Walker 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  Revision History
//   23 May 2003  Initial version (Daryle Walker)

#include <boost/cstdlib.hpp>          // for boost::exit_success
#include <boost/io/array_stream.hpp>  // for boost::io::basic_array_istream, etc.
#include <boost/test/minimal.hpp>     // for main, BOOST_TEST

#include <algorithm>  // for std::equal
#include <cstddef>    // for std::size_t


// Global objects
char const         alphabet[] = "abcdefghijklmnopqrstuvwxyz";
std::size_t const  alphabet_length = sizeof(alphabet) / sizeof(alphabet[0]) - 1;


// Test program
int
test_main
(
    int         ,   // "argc" is unused
    char *      []  // "argv" is unused
)
{
    typedef boost::io::basic_array_streambuf<alphabet_length, char>  astreambuf;
    typedef boost::io::basic_array_istream<alphabet_length, char>    aistream;
    typedef boost::io::basic_array_ostream<alphabet_length, char>    aostream;
    typedef boost::io::basic_array_stream<alphabet_length, char>     aiostream;

    // Write test
    aostream      os1;
    astreambuf &  osb1 = *os1.rdbuf();
    BOOST_TEST( os1.is_using_internal_streambuf() );

    os1 << alphabet;
    BOOST_TEST( std::equal(os1.array_begin(), os1.array_end(), alphabet) );

    // Read test
    aistream      is1;
    astreambuf &  isb1 = *is1.rdbuf();
    BOOST_TEST( is1.std::istream::rdbuf() == &isb1 );
    BOOST_TEST( is1.std::istream::rdbuf() == is1.rdbuf() );
    BOOST_TEST( is1.is_using_internal_streambuf() );

    is1.std::istream::rdbuf( &osb1 );
    BOOST_TEST( is1.std::istream::rdbuf() != &isb1 );
    BOOST_TEST( is1.std::istream::rdbuf() != is1.rdbuf() );
    BOOST_TEST( !is1.is_using_internal_streambuf() );

    char  a, b;
    is1 >> a >> b;
    BOOST_TEST( a == 'a' );
    BOOST_TEST( b == 'b' );

    return boost::exit_success;
}
