//  Boost string_algo library substr_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <utility>
#include <vector>

#include <boost/container_traits.hpp>
#include <boost/type_traits.hpp>

// equals predicate is used for result comparison
#include <boost/string_algo/predicate.hpp>

// test tools
#include <boost/test/test_tools.hpp>


using namespace std;
using namespace boost;

namespace sa = ::boost::string_algo;

template< typename T >
void argument_cv_test( const T& C, const string& strResult )
{
    BOOST_CHECK( equals( make_range(sa::begin(C),sa::end(C)), strResult ) );
    BOOST_CHECK( sa::size(C)==strResult.size() );
    BOOST_CHECK( sa::empty(C)==strResult.empty() );
}

template< typename T >
void argument_test( T& C, const string& strResult )
{
    BOOST_CHECK( equals( make_range(sa::begin(C),sa::end(C)), strResult ) );
    BOOST_CHECK( sa::size(C)==strResult.size() );
    BOOST_CHECK( sa::empty(C)==strResult.empty() );
}

void container_test()
{
    BOOST_CHECKPOINT( "type test" );
        
    // types test
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<string>::result_iterator, 
        string::iterator>::value) );
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<const string>::result_iterator, 
        string::const_iterator>::value) );
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<char[4]>::result_iterator, char*>::value) );
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<const char[4]>::result_iterator, 
        const char*>::value) );
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<char*>::result_iterator, char*>::value) );
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<const char*>::result_iterator, 
        const char*>::value) );
    BOOST_REQUIRE( (is_same< 
        sa::container_traits<
            pair<string::iterator, string::iterator> >::result_iterator, string::iterator>::value) );
    BOOST_REQUIRE( (is_same< 
         sa::container_traits<
            const pair<string::iterator, string::iterator> >::result_iterator, 
            string::iterator>::value) );

    BOOST_CHECKPOINT( "non-empty sequence test" );
    
    string str1("abc");
    vector<char> vec1( str1.begin(), str1.end() );
    pair<string::iterator, string::iterator> pair1=
        make_pair( str1.begin(), str1.end() );
    char ach1[]="abc";
    char *pch1="abc";

    // begin/end tests
    argument_cv_test( str1, "abc" );
    argument_test( str1, "abc" );
    argument_cv_test( vec1, "abc" );
    argument_test( vec1, "abc" );
    argument_cv_test( pair1, "abc" );
    argument_test( pair1, "abc" );
    argument_cv_test( ach1, "abc" );
    argument_test( ach1, "abc" );
    argument_cv_test( pch1, "abc" );
    argument_test( pch1, "abc" );

    BOOST_CHECKPOINT( "empty sequence test" );

    string str2;
    vector<char> vec2( str2.begin(), str2.end() );
    pair<string::iterator, string::iterator> pair2=
        make_pair( str2.begin(), str2.end() );
    char ach2[]="";
    char *pch2=0;

    // begin/end tests
    argument_cv_test( str2, "" );
    argument_test( str2, "" );
    argument_cv_test( vec2, "" );
    argument_test( vec2, "" );
    argument_cv_test( pair2, "" );
    argument_test( pair2, "" );
    argument_cv_test( ach2, "" );
    argument_test( ach2, "" );
    argument_cv_test( pch2, "" );
    argument_test( pch2, "" );
};


// test main 
int test_main( int, char*[] )
{
    container_test();

    return 0;
}
