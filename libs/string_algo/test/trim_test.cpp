//  Boost string_algo library tri_test.cpp file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <iostream>
#include <boost/string_algo/trim.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void trim_test()
{
    string str1("     1x x x x1     ");
    string str2("     2x x x x2     ");
    string str3("    ");

    // *** value passing tests *** //

    // general string test
    BOOST_CHECK( trim_left_copy( str1 )=="1x x x x1     " ) ;
    BOOST_CHECK( trim_right_copy( str1 )=="     1x x x x1" ) ;
    BOOST_CHECK( trim_copy( str1 )=="1x x x x1" ) ;

    // spaces-only string test
    BOOST_CHECK( trim_left_copy( str3 )=="" );
    BOOST_CHECK( trim_right_copy( str3 )=="" );
    BOOST_CHECK( trim_copy( str3 )=="" );

    // empty string check 
    BOOST_CHECK( trim_left_copy( string("") )=="" );
    BOOST_CHECK( trim_right_copy( string("") )=="" );
    BOOST_CHECK( trim_copy( string("") )=="" );

    // iterator tests
    string str;
    trim_left_copy_if( std::back_inserter(str), str1.begin(), str1.end(), is_space<char>() );
    BOOST_CHECK( str=="1x x x x1     " );

    str.clear();
    trim_right_copy_if( std::back_inserter(str), str1.begin(), str1.end(), is_space<char>() );
    BOOST_CHECK( str=="     1x x x x1" );

    str.clear();
    trim_copy_if( std::back_inserter(str), str1.begin(), str1.end(), is_space<char>() );
    BOOST_CHECK( str=="1x x x x1" );

    // *** inplace tests *** //

    // general string test
    trim_left( str1 );
    BOOST_CHECK( str1=="1x x x x1     " );
    trim_right( str1 );
    BOOST_CHECK( str1=="1x x x x1" );
    trim( str2 );
    BOOST_CHECK( str2=="2x x x x2" );
    
    // spaces-only string test
    str3 = "    "; trim_left( str3 );
    BOOST_CHECK( str3=="" );
    str3 = "    "; trim_right( str3 );
    BOOST_CHECK( str3=="" );
    str3 = "    "; trim( str3 );
    BOOST_CHECK( str3=="" );

    // empty string check 
    str3 = ""; trim_left( str3 );
    BOOST_CHECK( str3=="" );
    str3 = ""; trim_right( str3 );
    BOOST_CHECK( str3=="" );
    str3 = ""; trim( str3 );
    BOOST_CHECK( str3=="" );

    // *** non-standard predicate tests *** //
    BOOST_CHECK( 
        trim_copy_if( 
            string("123abc456"), 
            is_classified<char>(std::ctype_base::digit) )=="abc" );
    BOOST_CHECK( trim_copy_if( string("<>abc<>"), is_from( string("<<>>") ) )=="abc" );
}
