#include <string>
#include <iostream>
#include <iterator>
#include <set>
#include <list>
#include <vector>
#include <boost/string_algo/find.hpp>
//#include <boost/string_algo/replace.hpp>
//#include <boost/string_algo/detail/util.hpp>
//#include <boost/regex.hpp>
//#include <boost/string_algo/regex.hpp>

using namespace std;
using namespace boost;

/*

void trimtest()
{
    string str1("     1x x x x1     ");
    string str2("     2x x x x2     ");
    string str3("    ");
    string str4("<>trim<>");
    string str5("123abs343");

    cout << "*" << trim_left_copy( str1 ) << "*" << endl;
    cout << "*" << trim_right_copy( str1 ) << "*" << endl;
    cout << "*" << trim_copy( str1 ) << "*" << endl;

    cout << "----" << endl;

    cout << "*" << trim_left_copy( str3 ) << "*" << endl;
    cout << "*" << trim_right_copy( str3 ) << "*" << endl;
    cout << "*" << trim_copy( str3 ) << "*" << endl;

    cout << "----" << endl;

    trim_right( str1 );
    cout << "*" << str1 << "*" << endl;
    trim_left( str1 );
    cout << "*" << str1 << "*" << endl;
    trim( str2 );
    cout << "*" << str2 << "*" << endl;

    cout << "----" << endl;

    trim_right( str3 );
    cout << "*" << str3 << "*" << endl;
    trim_left( str3 );
    cout << "*" << str3 << "*" << endl;
    trim( str3 );
    cout << "*" << str3 << "*" << endl;

    cout << "*" << trim_copy_if( str4, if_isfrom( string("<>") ) ) << "*" << endl;
    cout << "*" << trim_copy_if( str5, if_isclassified<char>(std::ctype_base::digit) ) << "*" << endl;

}

void convtest()
{
    string str1("AbCdEfG");
    string str2("aaaaBBBB");
    string str3("");

    cout << "*" << tolower_copy( str1 ) << "*" << endl;
    cout << "*" << toupper_copy( str1 ) << "*" << endl;

    cout << "----" << endl;

    tolower( str1 );
    cout << "*" << str1 << "*" << endl;
    toupper( str1 );
    cout << "*" << str1 << "*" << endl;
}

void algo()
{
    string str1("123xxx321");
    string str2("abc");
    string str3("");

    cout << (starts_with( str1, string("123") )?"true":"false") << endl; 
    cout << (starts_with( str1, string("1234") )?"true":"false") << endl; 
    cout << (ends_with( str1, string("321") )?"true":"false") << endl; 
    cout << (ends_with( str1, string("123") )?"true":"false") << endl; 

    cout << (starts_with( str2, string("abc") )?"true":"false") << endl; 
    cout << (ends_with( str2, string("abc") )?"true":"false") << endl; 

    cout << (starts_with( str2, string("abcd") )?"true":"false") << endl; 
    cout << (ends_with( str2, string("abcd") )?"true":"false") << endl; 

    cout << (contains( str1, string("xxx") )?"true":"false") << endl; 
    cout << (contains( str1, string("") )?"true":"false") << endl; 
    cout << (contains( str1, string("abc") )?"true":"false") << endl; 
}



void replace()
{
    string str1("1xxabcxxabcxx1");
    const string str2("abc");
    const string str3("YYY");
    vector<char> vec1( str1.begin(), str1.end() );
    list<char> list1( str1.begin(), str1.end() );

    cout << str1 << endl;

    cout << endl;

    ostream_iterator<char> ost(cout);

    replace_first_copy( ost, string("abcabc"), string("abc"), string("YYY") );
    cout << endl;
    replace_first_copy( 
        ost,
        str1.begin(), str1.end(), 
        str2.begin(), str2.end(),
        str3.begin(), str3.end() );

    cout << endl;

    cout << replace_first_copy( string("abcabc"), string("abc"), string("YYY") ) << endl; 
    cout << replace_first_copy( string("abcabc"), string(""), string("YYY") ) << endl; 


    cout << replace_last_copy( string("abcabc"), string("abc"), string("YYY") ) << endl; 
    cout << replace_last_copy( string("abcabc"), string(""), string("YYY") ) << endl; 
    cout << replace_all_copy( string("abcabc"), string("abc"), string("YYY") ) << endl; 
    cout << replace_all_copy( string("abcabc"), string("abd"), string("YYY") ) << endl; 
    cout << replace_all_copy( str1, str2, string("") ) << endl;

    cout << erase_all_copy( string("abcabc"), string("abc") ) << endl; 
    cout << erase_first_copy( string("abcabc"), string("abc") ) << endl; 
    cout << erase_all_copy( str1, string("") ) << endl;

    cout << endl;


    replace_first( vec1, string("xx"), string("yy") );
    cout << "vector: " << string( vec1.begin(), vec1.end() ) << endl;
    replace_first( vec1, string("yy"), string("zzz") );
    cout << "vector: " << string( vec1.begin(), vec1.end() ) << endl;
    replace_first( vec1, string("zzz"), string("xx") );
    cout << "vector: " << string( vec1.begin(), vec1.end() ) << endl;

    replace_first( list1, string("xx"), string("yy") );
    cout << "list: " << string( list1.begin(), list1.end() ) << endl;

    replace_first( str1, string("xx"), string("yy") );
    cout << str1 << endl;

    replace_all( str1, string("xx"), string("yy") );
    cout << str1 << endl;
    replace_all( str1, string("yy"), string("abc") );
    cout << str1 << endl;
    replace_all( str1, string("abc"), string("X") );
    cout << str1 << endl;
    replace_all( str1, string("X"), string("") );
    cout << str1 << endl; 


    cout << endl;
}

*/

void substr()
{
    string str1("123abcxxxabcXXXabc321");
    const string str3("123abcxxxabcXXXabc321");
    const string str2("abc");

    iterator_range<string::const_iterator> cv_result=
        find_first( str3, str2 );

    cout << string( cv_result.begin(), str3.end() ) << endl;
    cout << string( str3.begin(), cv_result.end() ) << endl;

    cout << endl;

    cv_result=find_first( str3.begin(), str3.end(), str2.begin(), str2.end() );

    cout << string( cv_result.begin(), str3.end() ) << endl;
    cout << string( str3.begin(), cv_result.end() ) << endl;

    cout << endl;

    iterator_range<string::iterator> nc_result=find_first( str1, str2 );
    cout << string( nc_result.begin(), str1.end() ) << endl;
    cout << string( str1.begin(), nc_result.end() ) << endl;

    cout << endl;

    nc_result=find_first( str1.begin(), str1.end(), str2.begin(), str2.end() );
    cout << string( nc_result.begin(), str1.end() ) << endl;
    cout << string( str1.begin(), nc_result.end() ) << endl;

    cout << endl;

    cv_result=find_last( str3, str2 );

    cout << string( cv_result.begin(), str3.end() ) << endl;
    cout << string( str3.begin(), cv_result.end() ) << endl;

    cout << endl;

    cv_result=find_last( str3.begin(), str3.end(), str2.begin(), str2.end() );

    cout << string( cv_result.begin(), str3.end() ) << endl;
    cout << string( str3.begin(), cv_result.end() ) << endl;

    cout << endl;

    nc_result=find_last( str1, str2 );
    cout << string( nc_result.begin(), str1.end() ) << endl;
    cout << string( str1.begin(), nc_result.end() ) << endl;

    cout << endl;

    nc_result=find_last( str1.begin(), str1.end(), str2.begin(), str2.end() );
    cout << string( nc_result.begin(), str1.end() ) << endl;
    cout << string( str1.begin(), nc_result.end() ) << endl;

    cout << endl;

    cv_result=find_nth( str3, str2, 1 );

    cout << string( cv_result.begin(), str3.end() ) << endl;
    cout << string( str3.begin(), cv_result.end() ) << endl;

    cout << endl;

    cv_result=find_nth( str3.begin(), str3.end(), str2.begin(), str2.end(), 1 );

    cout << string( cv_result.begin(), str3.end() ) << endl;
    cout << string( str3.begin(), cv_result.end() ) << endl;

    cout << endl;

    nc_result=find_nth( str1, str2, 1 );
    cout << string( nc_result.begin(), str1.end() ) << endl;
    cout << string( str1.begin(), nc_result.end() ) << endl;

    cout << endl;

    nc_result=find_nth( str1.begin(), str1.end(), str2.begin(), str2.end(), 1 );
    cout << string( nc_result.begin(), str1.end() ) << endl;
    cout << string( str1.begin(), nc_result.end() ) << endl;

    cout << endl;
}

/*

void equals_test()
{
    string str1("123");
    const string str2( "12345" );
    const string str3( "123" );

    if ( !boost::equals( str1.begin(), str1.end(), str2.begin(), str2.end(), 
        boost::string_algo::detail::equal_toF<char, char>() ) )
        cout << "1:false" << endl;
    if ( !boost::equals( str2.begin(), str2.end(), str1.begin(), str1.end() ) )
        cout << "2:false" << endl;
    if ( boost::equals( str1, str3 ) )
        cout << "3:true" << endl;
}

void contatiner()
{
    string str1("123xxx123yyy123");
    string str2("456");

    const char cstr[]="123";

    ostream_iterator<char> ost(cout);

    erase_first_copy( 
        str1.begin(), str1.end(), 
        cstr, cstr+3,
        ost );

    cout << endl;

}



void regextest()
{
    string str1("123xxx123yyy123");
    string str2("xxxyyy");
    vector<int> vec1( str1.begin(), str1.end() );
    regex rx("([0-9]+)");
    
    iterator_range<string::iterator> r=
        boost::find_regex_nc( 
            str2, 
            rx );

    cout << string( r.begin(), r.end() ) << endl;

    replace_regex(
        vec1,
        rx,
        string("a$1a") );

    cout << string( vec1.begin(), vec1.end() ) << endl;

    cout << 
        replace_all_regex_copy(
            str1,
            rx,
            string("A$1A") ) << endl;

    cout << erase_regex_copy( str1, rx ) << endl;
    cout << erase_all_regex_copy( str1, rx ) << endl; 
}

*/


int main()
{
//  trimtest();
//  convtest();
//  algo();
//  equals_test();
    substr();
//  replace();
//  contatiner();
//  regextest();

    cout << "Done." << endl;
    cin.get();
}
