//  Boost string_algo library substr_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

#include <boost/string_algo/container_traits.hpp>

#include <boost/type_traits.hpp>

// equals predicate is used for result comparison
//#include <boost/string_algo/predicate.hpp>

//#include <boost/test/test_tools.hpp>
//#include <boost/preprocessor/seq/enum.hpp>

using namespace std;
using namespace boost;

namespace sa = ::boost::string_algo;

void Print( char C )
{
    cout << C;
}

template< typename T >
void argument_test_cv( const T& C )
{
    std::for_each( sa::begin(C), sa::end(C), Print );
    cout << endl;
}

template< typename T >
void argument_test( T& C )
{
    std::for_each( sa::begin(C), sa::end(C), Print );
    cout << endl;
}

void container_test()
{
    string str1("abc");
    argument_test( str1 );
    std::pair<string::const_iterator,string::const_iterator> Pair=
        make_pair( str1.begin(), str1.end() );
    argument_test( Pair );

    argument_test_cv( str1 );
    argument_test_cv( make_pair( str1.begin(), str1.end() ) );

    argument_test("123");
//  argument_test(L"123");

    argument_test_cv("123");
//  argument_test_cv(L"123");

    char* pch="456";
    const char* pcch="567";

    argument_test(pch);
    argument_test_cv(pch);

    argument_test(pcch);
    argument_test_cv(pcch);

    string str2;
    char ach[]="";

    BOOST_CHECK( sa::empty(str2) );
    BOOST_CHECK( sa::empty("") );
    BOOST_CHECK( sa::empty(ach) );
    BOOST_CHECK( sa::empty((char*)0) );
};


// test main 
int test_main( int, char*[] )
{
    container_test();

    return 0;
}
