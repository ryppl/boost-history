//  Boost string_algo library substr_test.cpp file  ------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <boost/string_algo/replace.hpp>
#include <boost/string_algo/erase.hpp>
#include <boost/string_algo/find.hpp>
#include <boost/test/test_tools.hpp>

using namespace std;
using namespace boost;

void replace_test()
{
    string str1("123abcxxxabc321");
    string str2("abc");
    vector<char> vec1( str1.begin(), str1.end() );
    list<char> list1( str1.begin(), str1.end() );

    // inmutable tests
	iterator_range<string::const_iterator> const_range=
		find_first( str1, string("abc" ) );

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

    BOOST_CHECK( replace_range_copy( str1, const_range.begin(), const_range.end(), string("YYY") )==string("123YYYxxxabc321") );
    BOOST_CHECK( replace_range_copy( str1, const_range.begin(), const_range.end(), string("Z") )==string("123Zxxxabc321") );
    BOOST_CHECK( replace_range_copy( str1, const_range.begin(), const_range.end(), string("XXXX") )==string("123XXXXxxxabc321") );

    BOOST_CHECK( erase_range_copy( str1, const_range.begin(), const_range.end() )==string("123xxxabc321") );
    BOOST_CHECK( erase_first_copy( string("1abc3abc2"), string("abc") )==string("13abc2") );
    BOOST_CHECK( erase_last_copy( string("1abc3abc2"), string("abc") )==string("1abc32") );
    BOOST_CHECK( erase_all_copy( string("1abc3abc2"), string("abc") )==string("132") );

    // nth variants test
    BOOST_CHECK( replace_nth_copy( string("1abc3abc2"), string("abc"), 0, string("YYY") )==string("1YYY3abc2") );
    BOOST_CHECK( replace_nth_copy( string("1abc3abc2"), string("abc"), 1, string("Z") )==string("1abc3Z2") );
    BOOST_CHECK( replace_nth_copy( string("1abc3abc2"), string("abc"), 2, string("XXXX") )==string("1abc3abc2") );

    // output iterator variants test
    string strout;
    replace_first_copy( back_inserter(strout), string("1abc3abc2"), string("abc"), string("YYY") );
    BOOST_CHECK( strout==string("1YYY3abc2") );
    strout.clear();
    replace_last_copy( back_inserter(strout), string("1abc3abc2"), string("abc"), string("YYY") );
    BOOST_CHECK( strout==string("1abc3YYY2") );
    strout.clear();
    replace_all_copy( back_inserter(strout), string("1abc3abc2"), string("abc"), string("YYY") );
    BOOST_CHECK( strout==string("1YYY3YYY2") );
    strout.clear();
    replace_range_copy( back_inserter(strout), str1, const_range.begin(), const_range.end(), string("YYY") );
    BOOST_CHECK( strout==string("123YYYxxxabc321") );
    strout.clear();
    erase_first_copy( back_inserter(strout), string("1abc3abc2"), string("abc") );
    BOOST_CHECK( strout==string("13abc2") );
    strout.clear();
    erase_last_copy( back_inserter(strout), string("1abc3abc2"), string("abc") );
    BOOST_CHECK( strout==string("1abc32") );
    strout.clear();
    erase_range_copy( back_inserter(strout), str1, const_range.begin(), const_range.end() );
    BOOST_CHECK( strout==string("123xxxabc321") );
    strout.clear();
    erase_all_copy( back_inserter(strout), string("1abc3abc2"), string("abc") );
    BOOST_CHECK( strout==string("132") );
    strout.clear();

    // empty-string test
    BOOST_CHECK( replace_first_copy( string("1abc3abc2"), string(""), string("YYY") )==string("1abc3abc2") );
    BOOST_CHECK( replace_first_copy( string(""), string("abc"), string("Z") )==string("") );
    BOOST_CHECK( replace_last_copy( string("1abc3abc2"), string(""), string("YYY") )==string("1abc3abc2") );
    BOOST_CHECK( replace_last_copy( string(""), string("abc"), string("Z") )==string("") );
    BOOST_CHECK( replace_all_copy( string("1abc3abc2"), string(""), string("YYY") )==string("1abc3abc2") );
    BOOST_CHECK( replace_all_copy( string(""), string("abc"), string("Z") )==string("") );

    // in-place test ( string )
	iterator_range<string::iterator> range=find_first( str1, string("abc") );
	
	replace_range( str1, range.begin(), range.end(), string("abc#abc") );
	BOOST_CHECK( str1==string("123abc#abcxxxabc321") );
    replace_first( str1, string("x"), string("yyyy") );
	BOOST_CHECK( str1==string("123abc#abcyyyyxxabc321") );
    replace_first( str1, string("yyyy"), string("zzz") );
    BOOST_CHECK( str1==string("123abc#abczzzxxabc321") );
    replace_first( str1, string("zzz"), string("456") );
    BOOST_CHECK( str1==string("123abc#abc456xxabc321") );
    replace_last( str1, string("x"), string("456") );
    BOOST_CHECK( str1==string("123abc#abc456x456abc321") );

    replace_all( str1, string("abc"), string("X") );
	BOOST_CHECK( str1==string("123X#X456x456X321") );
    replace_all( str1, string("X"), string("mno") );
	BOOST_CHECK( str1==string("123mno#mno456x456mno321") );
    replace_all( str1, string("mno"), string("ZZZ") );
	BOOST_CHECK( str1==string("123ZZZ#ZZZ456x456ZZZ321") );
    erase_first( str1, string("ZZZ") );
	BOOST_CHECK( str1==string("123#ZZZ456x456ZZZ321") );
    erase_all( str1, string("Z") );
	BOOST_CHECK( str1==string("123#456x456321") );

    // in-place test ( vector )
    replace_first( vec1, string("x"), string("yyyy") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123abcyyyyxxabc321") );
    replace_first( vec1, string("yyyy"), string("zzz") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123abczzzxxabc321") );
    replace_first( vec1, string("zzz"), string("456") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123abc456xxabc321") );
    replace_last( vec1, string("x"), string("456") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123abc456x456abc321") );

    replace_all( vec1, string("abc"), string("X") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123X456x456X321") );
    replace_all( vec1, string("X"), string("mno") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123mno456x456mno321") );
    replace_all( vec1, string("mno"), string("ZZZ") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123ZZZ456x456ZZZ321") );
    erase_first( vec1, string("ZZZ") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123456x456ZZZ321") );
    erase_all( vec1, string("Z") );
    BOOST_CHECK( string(vec1.begin(), vec1.end())==string("123456x456321") );

    // in-place test ( list )
    replace_first( list1, string("x"), string("yyyy") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123abcyyyyxxabc321") );
    replace_first( list1, string("yyyy"), string("zzz") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123abczzzxxabc321") );
    replace_first( list1, string("zzz"), string("456") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123abc456xxabc321") );
    replace_last( list1, string("x"), string("456") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123abc456x456abc321") );

    replace_all( list1, string("abc"), string("X") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123X456x456X321") );
    replace_all( list1, string("X"), string("mno") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123mno456x456mno321") );
    replace_all( list1, string("mno"), string("ZZZ") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123ZZZ456x456ZZZ321") );
    erase_first( list1, string("ZZZ") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123456x456ZZZ321") );
    erase_all( list1, string("Z") );
    BOOST_CHECK( string(list1.begin(), list1.end())==string("123456x456321") );
}
