//  Boost string_algo library substr_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <boost/string_algo/find.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void find_test()
{
    string str1("123abcxxxabcXXXabc321");
    string str2("abc");
    string str3("");
    vector<int> vec1( str1.begin(), str1.end() );

    // find results
    iterator_range<string::iterator> nc_result;
    iterator_range<string::const_iterator> cv_result;
    
    iterator_range<vector<int>::iterator> nc_vresult;
    iterator_range<vector<int>::const_iterator> cv_vresult;

    // basic tests
    nc_result=find_first( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_first( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    nc_result=find_first( str1, string("abc") );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_first( str1, str2 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    nc_result=find_last( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 18) );

    cv_result=find_last( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 18) );

    nc_result=find_last( str1, string("abc") );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 18) );

    cv_result=find_last( str1, str2 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 15) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 18) );

    nc_result=find_nth( str1.begin(), str1.end(), str2.begin(), str2.end(), 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 9) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 12) );

    cv_result=find_nth( str1.begin(), str1.end(), str2.begin(), str2.end(), 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 9) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 12) );

    nc_result=find_nth( str1, string("abc"), 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 9) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 12) );

    cv_result=find_nth( str1, str2, 1 );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 9) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 12) );
        
    // multi-type comparison test
    nc_vresult=find_first( vec1, string("abc") );
    BOOST_CHECK( 
        (distance<vector<int>::const_iterator>( vec1.begin(),nc_vresult.begin()) == 3) &&
        (distance<vector<int>::const_iterator>( vec1.begin(),nc_vresult.end()) == 6) );

    cv_vresult=find_first( vec1, str2 );
    BOOST_CHECK( 
        (distance<vector<int>::const_iterator>( vec1.begin(),cv_vresult.begin()) == 3) &&
        (distance<vector<int>::const_iterator>( vec1.begin(),cv_vresult.end()) == 6) );

    // overflow test
    nc_result=find_first( str2, string("abcd") );
    BOOST_CHECK( nc_result.begin()==nc_result.end() );
    cv_result=find_first( str2, string("abcd") );
    BOOST_CHECK( cv_result.begin()==cv_result.end() );

    //! Empty string test
    nc_result=find_first( str3, string("abcd") );
    BOOST_CHECK( nc_result.begin()==nc_result.end() );
    nc_result=find_first( str1, string("") );
    BOOST_CHECK( nc_result.begin()==nc_result.end() );

    cv_result=find_first( str3, string("abcd") );
    BOOST_CHECK( cv_result.begin()==cv_result.end() );
    cv_result=find_first( str1, string("") );
    BOOST_CHECK( cv_result.begin()==cv_result.end() ); 
}

