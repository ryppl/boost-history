//  Boost string_algo library substr_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

// Include unit test framework
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/string_algo/regex.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

static void find_test()
{
    string str1("123a1cxxxa23cXXXa456c321");
    regex rx("a[0-9]+c");
    vector<int> vec1( str1.begin(), str1.end() );

    // find results
    iterator_range<string::iterator> nc_result;
    iterator_range<string::const_iterator> cv_result;
    
    iterator_range<vector<int>::iterator> nc_vresult;
    iterator_range<vector<int>::const_iterator> cv_vresult;

    // basic tests
    nc_result=find_regex( str1.begin(), str1.end(), rx );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_regex( str1.begin(), str1.end(), rx );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    nc_result=find_regex( str1, rx );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),nc_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),nc_result.end()) == 6) );

    cv_result=find_regex( str1, rx );
    BOOST_CHECK( 
        (distance<string::const_iterator>( str1.begin(),cv_result.begin()) == 3) &&
        (distance<string::const_iterator>( str1.begin(),cv_result.end()) == 6) );

    // multi-type comparison test
    nc_vresult=find_regex( vec1, rx );
    BOOST_CHECK( 
        (distance<vector<int>::const_iterator>( vec1.begin(),nc_vresult.begin()) == 3) &&
        (distance<vector<int>::const_iterator>( vec1.begin(),nc_vresult.end()) == 6) );

    cv_vresult=find_regex( vec1, rx );
    BOOST_CHECK( 
        (distance<vector<int>::const_iterator>( vec1.begin(),cv_vresult.begin()) == 3) &&
        (distance<vector<int>::const_iterator>( vec1.begin(),cv_vresult.end()) == 6) );

    BOOST_CHECK( find_regex_copy( str1, rx )==string("a1c") );
}

static void replace_test()
{
    string str1("123a1cxxxa23cXXXa456c321");
    regex rx1("a([0-9]+)c");
    regex rx2("([xX]+)");
    regex rx3("_[^_]*_");
    string fmt1("_A$1C_");
    string fmt2("_xXx_");
    vector<int> vec1( str1.begin(), str1.end() );

    // inmutable tests
    
    // basic tests
    BOOST_CHECK( replace_regex_copy( str1, rx1, fmt1 )==string("123_A1C_xxxa23cXXXa456c321") );
    BOOST_CHECK( replace_all_regex_copy( str1, rx1, fmt1 )==string("123_A1C_xxx_A23C_XXX_A456C_321") );
    BOOST_CHECK( erase_regex_copy( str1, rx1 )==string("123xxxa23cXXXa456c321") );
    BOOST_CHECK( erase_all_regex_copy( str1, rx1 )==string(string("123xxxXXX321")) );

    // output iterator variants test
    string strout;
    replace_regex_copy( back_inserter(strout), str1, rx1, fmt1 );
    BOOST_CHECK( strout==string("123_A1C_xxxa23cXXXa456c321") );
    strout.clear();
    replace_all_regex_copy( back_inserter(strout), str1, rx1, fmt1 );
    BOOST_CHECK( strout==string("123_A1C_xxx_A23C_XXX_A456C_321") );
    strout.clear();
    erase_regex_copy( back_inserter(strout), str1, rx1 );
    BOOST_CHECK( strout==string("123xxxa23cXXXa456c321") );
    strout.clear();
    erase_all_regex_copy( back_inserter(strout), str1, rx1 );
    BOOST_CHECK( strout==string("123xxxXXX321") );
    strout.clear();

    // in-place test
    replace_regex( str1, rx1, fmt2 );
    BOOST_CHECK( str1==string("123_xXx_xxxa23cXXXa456c321") );

    replace_all_regex( str1, rx2, fmt1 );
    BOOST_CHECK( str1==string("123__AxXxC___AxxxC_a23c_AXXXC_a456c321") );
    erase_regex( str1, rx3 );
    BOOST_CHECK( str1==string("123AxXxC___AxxxC_a23c_AXXXC_a456c321") );
    erase_all_regex( str1, rx3 );
    BOOST_CHECK( str1==string("123AxXxCa23ca456c321") );
}

int test_main( int, char*[] )
{
    find_test();
    replace_test();

    return 0;
}
