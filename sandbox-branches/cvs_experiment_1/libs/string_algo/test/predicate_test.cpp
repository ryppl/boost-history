//  Boost string_algo library predicate_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <vector>
#include <iostream>
#include <boost/string_algo.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void predicate_test()
{
    string str1("123xxx321");
    string str2("abc");
    string str3("");
    vector<int> vec1( str1.begin(), str1.end() );

    // Basic tests
    BOOST_CHECK( starts_with( str1, string("123") ) );
    BOOST_CHECK( !starts_with( str1, string("1234") ) );
    BOOST_CHECK( ends_with( str1, string("321") ) );
    BOOST_CHECK( !ends_with( str1, string("123") ) );
    BOOST_CHECK( contains( str1, string("xxx") ) );
    BOOST_CHECK( !contains( str1, string("yyy") ) );

    // multi-type comparison test
    BOOST_CHECK( starts_with( vec1, string("123") ) );
    BOOST_CHECK( ends_with( vec1, string("321") ) );
    BOOST_CHECK( contains( vec1, string("xxx") ) );

    // overflow test
    BOOST_CHECK( !starts_with( str2, string("abcd") ) );
    BOOST_CHECK( !ends_with( str2, string("abcd") ) );
    BOOST_CHECK( !contains( str2, string("abcd") ) );

    //! Empty string test
    BOOST_CHECK( starts_with( str2, string("") ) );
    BOOST_CHECK( ends_with( str2, string("") ) );
    BOOST_CHECK( contains( str2, string("") ) );
}
