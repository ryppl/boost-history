//  Boost array_stream_test.cpp test file  -----------------------------------//

//  (C) Copyright Daryle Walker 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears 
//  in all copies.  This software is provided "as is" without express or implied 
//  warranty, and with no claim as to its suitability for any purpose. 

//  Revision History
//   28 May 2003  Initial version (Daryle Walker)

#include <boost/io/array_stream.hpp>  // for boost::io::basic_array_istream, etc.
#include <boost/test/unit_test.hpp>   // for main, BOOST_CHECK, etc.

#include <algorithm>   // for std::count_if, std::equal
#include <cstddef>     // for std::size_t
#include <functional>  // for std::bind2nd, std::ptr_fun
#include <iosfwd>      // for std::ios
#include <list>        // for std::list
#include <string>      // for std::string


// Global objects
char const         alphabet[] = "abcdefghijklmnopqrstuvwxyz";
std::size_t const  alphabet_length = sizeof(alphabet) / sizeof(alphabet[0]) - 1;

// Types
typedef boost::io::basic_array_streambuf<alphabet_length, char>  astreambuf;
typedef boost::io::basic_array_istream<alphabet_length, char>    aistream;
typedef boost::io::basic_array_ostream<alphabet_length, char>    aostream;
typedef boost::io::basic_array_stream<alphabet_length, char>     aiostream;


// Unit test for streambuf-wrappers
void
streambuf_wrapping_unit_test
(
)
{
    using std::ios;

    // Check initial settings for each wrapper
    aostream      aos;
    astreambuf &  aosb = *aos.rdbuf();
    BOOST_CHECK( aos.is_using_internal_streambuf() );
    BOOST_CHECK_EQUAL( aos.rdbuf(), &aosb );
    BOOST_CHECK_EQUAL( aos.rdbuf(), aos.ios::rdbuf() );

    aistream      ais;
    astreambuf &  aisb = *ais.rdbuf();
    BOOST_CHECK( ais.is_using_internal_streambuf() );
    BOOST_CHECK_EQUAL( ais.rdbuf(), &aisb );
    BOOST_CHECK_EQUAL( ais.rdbuf(), ais.ios::rdbuf() );

    aiostream     as;
    astreambuf &  asb = *as.rdbuf();
    BOOST_CHECK( as.is_using_internal_streambuf() );
    BOOST_CHECK_EQUAL( as.rdbuf(), &asb );
    BOOST_CHECK_EQUAL( as.rdbuf(), as.ios::rdbuf() );

    // Now change a stream's buffer and recheck
    ais.ios::rdbuf( &asb );
    BOOST_CHECK( ais.ios::rdbuf() != &aisb );
    BOOST_CHECK( ais.ios::rdbuf() != ais.rdbuf() );
    BOOST_CHECK( !ais.is_using_internal_streambuf() );
}

// Unit test for array streambuf (constructors)
void
astreambuf_constructor_unit_test
(
)
{
    using std::ptr_fun;
    using std::bind2nd;
    using std::count_if;
    using std::equal;

    typedef astreambuf::traits_type  traits_type;

    // Default construction and const accessors
    astreambuf          asb_d;
    astreambuf const &  casb_d = asb_d;
    BOOST_CHECK_EQUAL( astreambuf::array_size, count_if(asb_d.array_begin(),
     asb_d.array_end(), bind2nd( ptr_fun(traits_type::eq), '\0' )) );
    BOOST_CHECK_EQUAL( astreambuf::array_size, count_if(casb_d.array_begin(),
     casb_d.array_end(), bind2nd( ptr_fun(traits_type::eq), '\0' )) );

    // Array-subsequence-based construction
    astreambuf  asb_a( alphabet, alphabet + alphabet_length );
    BOOST_CHECK( equal(asb_a.array_begin(), asb_a.array_end(), alphabet,
     traits_type::eq) );

    // Iterator-based construction
    // (Can't use std::vector, because its iterators may be char*)
    std::list<char>  l( alphabet, alphabet + alphabet_length );
    astreambuf       asb_i( l.begin(), l.end() );
    BOOST_CHECK( equal(asb_i.array_begin(), asb_i.array_end(), alphabet,
     traits_type::eq) );

    // Copy construction
    astreambuf  asb_c( asb_i );
    BOOST_CHECK( equal(asb_c.array_begin(), asb_c.array_end(), alphabet,
     traits_type::eq) );
}

// Unit test for array output stream
void
aostream_unit_test
(
)
{
    aostream  aos;
    aos << alphabet;
    BOOST_CHECK( std::equal(aos.array_begin(), aos.array_end(), alphabet,
     aostream::traits_type::eq) );
}

// Unit test for array input stream
void
aistream_unit_test
(
)
{
    aistream     ais( alphabet, alphabet + alphabet_length );
    std::string  scratch;
    ais >> scratch;
    BOOST_CHECK_EQUAL( scratch, alphabet );
}

// Unit test for array combined stream
void
aiostream_unit_test
(
)
{
    aiostream    aios;
    std::string  scratch;

    // Output
    aios << alphabet;
    BOOST_CHECK( std::equal(aios.array_begin(), aios.array_end(), alphabet,
     aiostream::traits_type::eq) );

    // Input
    // NOTE: there should be a get-area seeking call to switch to input,
    // but seeking isn't defined (at this time) and I "know" that the
    // internal pointers are at the right postition.  So no seeking is safer.
    aios >> scratch;
    BOOST_CHECK_EQUAL( scratch, alphabet );
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
     = BOOST_TEST_SUITE( "array stream test" );

    test->add( BOOST_TEST_CASE(streambuf_wrapping_unit_test) );
    test->add( BOOST_TEST_CASE(astreambuf_constructor_unit_test) );
    test->add( BOOST_TEST_CASE(aostream_unit_test) );
    test->add( BOOST_TEST_CASE(aistream_unit_test) );
    test->add( BOOST_TEST_CASE(aiostream_unit_test) );

    return test;
}
