//  Boost string_algo library trim_test.cpp file  ---------------------------//

//  Copyright Pavol Droba 2002-2003. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <iostream>
#include <boost/string_algo/trim.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

namespace sa=boost::string_algo;

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
    sa::trim_left_copy( std::back_inserter(str), str1, is_space<char>() );
    BOOST_CHECK( str=="1x x x x1     " );

    str.clear();
    sa::trim_right_copy( std::back_inserter(str), str1, is_space<char>() );
    BOOST_CHECK( str=="     1x x x x1" );

    str.clear();
    sa::trim_copy( std::back_inserter(str), str1, is_space<char>() );
    BOOST_CHECK( str=="1x x x x1" );

    str.clear();
    sa::trim_left_copy( 
        std::back_inserter(str), 
        "     1x x x x1     ", 
        is_space<char>() );
    BOOST_CHECK( str=="1x x x x1     " );

    str.clear();
    sa::trim_right_copy( 
        std::back_inserter(str), 
        "     1x x x x1     ", 
        is_space<char>() );
    BOOST_CHECK( str=="     1x x x x1" );

    str.clear();
    sa::trim_copy( 
        std::back_inserter(str), 
        "     1x x x x1     ", 
        is_space<char>() );
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
        sa::trim_copy( 
            string("123abc456"), 
            is_classified<char>(std::ctype_base::digit) )=="abc" );
    BOOST_CHECK( sa::trim_copy( string("<>abc<>"), is_any_of<char>( "<<>>" ) )=="abc" );
}

// test main 
int test_main( int, char*[] )
{
    trim_test();
    
    return 0;
}
