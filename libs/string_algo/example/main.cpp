#include <string>
#include <iostream>
#include <boost/string_algo.hpp>

using namespace std;
using namespace boost;

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

void substr()
{
    string str1("123abcxxxabcXXXabc321");
    const string str2("abc");

    pair<string::iterator, string::iterator> fresult=
        find_last( str1.begin(), str1.end(), str2.begin(), str2.end() );

    cout << string( fresult.first, str1.end() ) << endl;
    cout << string( str1.begin(), fresult.second ) << endl;

    cout << endl;

    cout << " --------------------- " << endl;

    for ( unsigned int n=0; n<5; n++ )
    {
        pair<string::iterator, string::iterator> fresult=
            find_nth( str1.begin(), str1.end(), str2.begin(), str2.end(), n );

        cout << string( fresult.first, str1.end() ) << endl;
        cout << string( str1.begin(), fresult.second ) << endl;
    
        cout << endl;
    }

    cout << " --------------------- " << endl;

    fresult=BOOST_STRING_NON_CONST_FUNCTION(find_first)( str1, str2 );
    cout << string( fresult.first, str1.end() ) << endl;
    cout << string( str1.begin(), fresult.second ) << endl;

    cout << endl;

    fresult=BOOST_STRING_NON_CONST_FUNCTION(find_first)( str1, str2 );
    cout << string( fresult.first, str1.end() ) << endl;
    cout << string( str1.begin(), fresult.second ) << endl;

    cout << endl;


}

void replace()
{
    string str1("123abcxxxabc321");
    const string str2("abc");
    const string str3("YYY");

    cout << endl;

    cout << replace_first_copy( string("abcabc"), string("abc"), string("YYY") ) << endl; 
    cout << replace_first_copy( string("abcabc"), string(""), string("YYY") ) << endl; 
    cout << replace_all_copy( string("abcabc"), string("abc"), string("YYY") ) << endl; 
    cout << replace_all_copy( string("abcabc"), string("abd"), string("YYY") ) << endl; 
    cout << replace_all_copy( str1, str2, string("") ) << endl;

    cout << erase_all_copy( string("abcabc"), string("abc") ) << endl; 
    cout << erase_first_copy( string("abcabc"), string("abc") ) << endl; 
    cout << erase_all_copy( str1, string("") ) << endl;

    cout << endl;

    replace_first( str1, string("x"), string("456") );
    cout << str1 << endl;
    replace_all( str1, string("abc"), string("X") );
    cout << str1 << endl;
    replace_all( str1, string("X"), string("mno") );
    cout << str1 << endl;
    replace_all( str1, string("mno"), string("ZZZ") );
    cout << str1 << endl;
    
    

    cout << endl;
}

int main()
{
//  trimtest();
//  convtest();
//  algo();
  substr();
//  replace();

    cout << "Done." << endl;
    cin.get();
}
