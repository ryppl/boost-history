//  Boost string_algo library string_predicate_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/string_algo.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void string_predicate_test()
{
    string str1("123xxx321");
    string str2("abc");
    string str3("");

    // Basic tests
    BOOST_CHECK( is_prefix( str1, string("123") ) );
    BOOST_CHECK( !is_prefix( str1, string("1234") ) );
    BOOST_CHECK( is_suffix( str1, string("321") ) );
    BOOST_CHECK( !is_suffix( str1, string("123") ) );

    // multi-type comparison test
    BOOST_CHECK( is_prefix( str2, string("abc") ) );
    BOOST_CHECK( is_suffix( str2, string("abc") ) );

    // overflow test
    BOOST_CHECK( !is_prefix( str2, string("abcd") ) );
    BOOST_CHECK( !is_suffix( str2, string("abcd") ) );
}
