//  Boost string_algo library trim_test.cpp file  ---------------------------//

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

void case_conv_test()
{
    string str1("AbCdEfG 123 xxxYYYzZzZ");
    string str2("AbCdEfG 123 xxxYYYzZzZ");
    string str3("");

    // *** value passing tests *** //

    BOOST_CHECK( tolower( str1 )=="abcdefg 123 xxxyyyzzzz" );
    BOOST_CHECK( toupper( str1 )=="ABCDEFG 123 XXXYYYZZZZ" );

    BOOST_CHECK( tolower( str3 )=="" );
    BOOST_CHECK( toupper( str3 )=="" );

    // *** inplace tests *** //

    tolower_in( str1 );
    BOOST_CHECK( str1=="abcdefg 123 xxxyyyzzzz" );
    toupper_in( str2 );
    BOOST_CHECK( str2=="ABCDEFG 123 XXXYYYZZZZ" );

    tolower_in( str3 );
    BOOST_CHECK( str3=="" );
    toupper_in( str3 );
    BOOST_CHECK( str3=="" );
}
