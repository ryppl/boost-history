//  Boost string_algo library substr_test.cpp file  ------------------//

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

void find_test()
{
    string str1("123abcxxxabc321");
    string str2("abc");
    string str3("");
    vector<int> vec1( str1.begin(), str1.end() );

    // find results
    pair<string::iterator, string::iterator> nc_result;
    pair<string::const_iterator, string::const_iterator> cv_result;
    
    pair<vector<int>::iterator, vector<int>::iterator> nc_vresult;
    pair<vector<int>::const_iterator, vector<int>::const_iterator> cv_vresult;

    // basic tests
    nc_result=find_first( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( string( nc_result.first, nc_result.second)==str2 );
    cv_result=find_first( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( string( cv_result.first, cv_result.second)==str2 );

    nc_result=find_last( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( string( nc_result.first, nc_result.second)==str2 );
    cv_result=find_last( str1.begin(), str1.end(), str2.begin(), str2.end() );
    BOOST_CHECK( string( cv_result.first, cv_result.second)==str2 );

    nc_result=BOOST_STRING_NON_CONST_FUNCTION(find_first)( str1, string("abc") );
    BOOST_CHECK( string( nc_result.first, nc_result.second)==string("abc") );
    cv_result=find_first( str1, str2 );
    BOOST_CHECK( string( cv_result.first, cv_result.second)==string("abc") );

    // multi-type comparison test
    nc_vresult=BOOST_STRING_NON_CONST_FUNCTION(find_first)( vec1, string("abc") );
    BOOST_CHECK( string( nc_vresult.first, nc_vresult.second)==string("abc") );
    cv_vresult=find_first( vec1, str2 );
    BOOST_CHECK( string( cv_vresult.first, cv_vresult.second)==string("abc") );

    // overflow test
    nc_result=BOOST_STRING_NON_CONST_FUNCTION(find_first)( str2, string("abcd") );
    BOOST_CHECK( nc_result.first==nc_result.second );
    cv_result=find_first( str2, string("abcd") );
    BOOST_CHECK( cv_result.first==cv_result.second );

    //! Empty string test
    nc_result=BOOST_STRING_NON_CONST_FUNCTION(find_first)( str3, string("abcd") );
    BOOST_CHECK( nc_result.first==nc_result.second );
    nc_result=BOOST_STRING_NON_CONST_FUNCTION(find_first)( str1, string("") );
    BOOST_CHECK( nc_result.first==nc_result.second );

    cv_result=find_first( str3, string("abcd") );
    BOOST_CHECK( cv_result.first==cv_result.second );
    cv_result=find_first( str1, string("") );
    BOOST_CHECK( cv_result.first==cv_result.second ); 
}

void replace_test()
{
    string str1("123abcxxxabc321");
    string str2("abc");

    // non-modify test
    
    // basic tests
    BOOST_CHECK( replace_first_copy( string("1abc3abc2"), string("abc"), string("YYY") )==string("1YYY3abc2") );
    BOOST_CHECK( replace_first_copy( string("1abc3abc2"), string("abc"), string("Z") )==string("1Z3abc2") );
    BOOST_CHECK( replace_first_copy( string("1abc3abc2"), string("abc"), string("XXXX") )==string("1XXXX3abc2") );

    BOOST_CHECK( replace_last_copy( string("1abc3abc2"), string("abc"), string("YYY") )==string("1abc3YYY2") );
    BOOST_CHECK( replace_last_copy( string("1abc3abc2"), string("abc"), string("Z") )==string("1abc3Z2") );
    BOOST_CHECK( replace_last_copy( string("1abc3abc2"), string("abc"), string("XXXX") )==string("1abc3XXXX2") );

    BOOST_CHECK( replace_all_copy( string("1abc3abc2"), string("abc"), string("YYY") )==string("1YYY3YYY2") );
    BOOST_CHECK( replace_all_copy( string("1abc3abc2"), string("abc"), string("Z") )==string("1Z3Z2") );
    BOOST_CHECK( replace_all_copy( string("1abc3abc2"), string("abc"), string("XXXX") )==string("1XXXX3XXXX2") );

    BOOST_CHECK( erase_first_copy( string("1abc3abc2"), string("abc") )==string("13abc2") );
    BOOST_CHECK( erase_last_copy( string("1abc3abc2"), string("abc") )==string("1abc32") );
    BOOST_CHECK( erase_all_copy( string("1abc3abc2"), string("abc") )==string("132") );

    // nth variants test
    BOOST_CHECK( replace_nth_copy( string("1abc3abc2"), string("abc"), 0, string("YYY") )==string("1YYY3abc2") );
    BOOST_CHECK( replace_nth_copy( string("1abc3abc2"), string("abc"), 1, string("Z") )==string("1abc3Z2") );
    BOOST_CHECK( replace_nth_copy( string("1abc3abc2"), string("abc"), 2, string("XXXX") )==string("1abc3abc2") );

    // empty-string test
    BOOST_CHECK( replace_first_copy( string("1abc3abc2"), string(""), string("YYY") )==string("1abc3abc2") );
    BOOST_CHECK( replace_first_copy( string(""), string("abc"), string("Z") )==string("") );
    BOOST_CHECK( replace_last_copy( string("1abc3abc2"), string(""), string("YYY") )==string("1abc3abc2") );
    BOOST_CHECK( replace_last_copy( string(""), string("abc"), string("Z") )==string("") );
    BOOST_CHECK( replace_all_copy( string("1abc3abc2"), string(""), string("YYY") )==string("1abc3abc2") );
    BOOST_CHECK( replace_all_copy( string(""), string("abc"), string("Z") )==string("") );

    // in-place test
    replace_first( str1, string("x"), string("456") );
    BOOST_CHECK( str1==string("123abc456xxabc321") );
    replace_last( str1, string("x"), string("456") );
    BOOST_CHECK( str1==string("123abc456x456abc321") );

    replace_all( str1, string("abc"), string("X") );
    BOOST_CHECK( str1==string("123X456x456X321") );
    replace_all( str1, string("X"), string("mno") );
    BOOST_CHECK( str1==string("123mno456x456mno321") );
    replace_all( str1, string("mno"), string("ZZZ") );
    BOOST_CHECK( str1==string("123ZZZ456x456ZZZ321") );
    erase_first( str1, string("ZZZ" ) );
    BOOST_CHECK( str1==string("123456x456ZZZ321") );
    erase_all( str1, string("Z") );
    BOOST_CHECK( str1==string("123456x456321") );
}

void substr_test()
{
    find_test();
    replace_test();
}
