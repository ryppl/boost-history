//  Boost string_algo library predicate_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <vector>
#include <iostream>
#include <boost/string_algo/predicate.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void predicate_test()
{
    string str1("123xxx321");
    string str1_prefix("123");
    string str2("abc");
    string str3("");
    string str4("abc");
    vector<int> vec1( str1.begin(), str1.end() );

    // Basic tests
    BOOST_CHECK( starts_with( str1, string("123") ) );
    BOOST_CHECK( !starts_with( str1, string("1234") ) );

    BOOST_CHECK( 
        starts_with( 
            str1.begin(), 
            str1.end(), 
            str1_prefix.begin(), 
            str1_prefix.end() ) );

    BOOST_CHECK( ends_with( str1, string("321") ) );
    BOOST_CHECK( !ends_with( str1, string("123") ) );

    BOOST_CHECK( contains( str1, string("xxx") ) );
    BOOST_CHECK( !contains( str1, string("yyy") ) );
    BOOST_CHECK( 
        contains( 
            str1.begin(), 
            str1.end(), 
            str1_prefix.begin(), 
            str1_prefix.end() ) );

    BOOST_CHECK( equals( str2, string("abc") ) );
    BOOST_CHECK( !equals( str1, string("yyy") ) );
    BOOST_CHECK( 
        boost::equals( 
            str2.begin(), 
            str2.end(), 
            str4.begin(), 
            str4.end() ) );


    // multi-type comparison test
    BOOST_CHECK( starts_with( vec1, string("123") ) );
    BOOST_CHECK( ends_with( vec1, string("321") ) );
    BOOST_CHECK( contains( vec1, string("xxx") ) );
    BOOST_CHECK( equals( vec1, str1 ) );

    // overflow test
    BOOST_CHECK( !starts_with( str2, string("abcd") ) );
    BOOST_CHECK( !ends_with( str2, string("abcd") ) );
    BOOST_CHECK( !contains( str2, string("abcd") ) );
    BOOST_CHECK( !equals( str2, string("abcd") ) );

    // equal test
    BOOST_CHECK( starts_with( str2, string("abc") ) );
    BOOST_CHECK( ends_with( str2, string("abc") ) );
    BOOST_CHECK( contains( str2, string("abc") ) );
    BOOST_CHECK( equals( str2, string("abc") ) );

    //! Empty string test
    BOOST_CHECK( starts_with( str2, string("") ) );
    BOOST_CHECK( ends_with( str2, string("") ) );
    BOOST_CHECK( contains( str2, string("") ) );
    BOOST_CHECK( equals( str3, string("") ) );
}
