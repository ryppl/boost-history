//  Boost string_algo library conv_test.cpp file  ---------------------------//

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

void conv_test()
{
    string str1("AbCdEfG 123 xxxYYYzZzZ");
    string str2("AbCdEfG 123 xxxYYYzZzZ");
    string str3("");

    // *** value passing tests *** //

    BOOST_CHECK( tolower_copy( str1 )=="abcdefg 123 xxxyyyzzzz" );
    BOOST_CHECK( toupper_copy( str1 )=="ABCDEFG 123 XXXYYYZZZZ" );

    BOOST_CHECK( tolower_copy( str3 )=="" );
    BOOST_CHECK( toupper_copy( str3 )=="" );

    // *** inplace tests *** //

    tolower( str1 );
    BOOST_CHECK( str1=="abcdefg 123 xxxyyyzzzz" );
    toupper( str2 );
    BOOST_CHECK( str2=="ABCDEFG 123 XXXYYYZZZZ" );

    tolower( str3 );
    BOOST_CHECK( str3=="" );
    toupper( str3 );
    BOOST_CHECK( str3=="" );
}
