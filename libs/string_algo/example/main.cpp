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

    cout << "*" << ltrim( str1 ) << "*" << endl;
    cout << "*" << rtrim( str1 ) << "*" << endl;
    cout << "*" << trim( str1 ) << "*" << endl;

    cout << "----" << endl;

    cout << "*" << ltrim( str3 ) << "*" << endl;
    cout << "*" << rtrim( str3 ) << "*" << endl;
    cout << "*" << trim( str3 ) << "*" << endl;

    cout << "----" << endl;

    rtrim_in( str1 );
    cout << "*" << str1 << "*" << endl;
    ltrim_in( str1 );
    cout << "*" << str1 << "*" << endl;
    trim_in( str2 );
    cout << "*" << str2 << "*" << endl;

    cout << "----" << endl;

    rtrim_in( str3 );
    cout << "*" << str3 << "*" << endl;
    ltrim_in( str3 );
    cout << "*" << str3 << "*" << endl;
    trim_in( str3 );
    cout << "*" << str3 << "*" << endl;
}

void convtest()
{
    string str1("AbCdEfG");
    string str2("aaaaBBBB");
    string str3("");

    cout << "*" << tolower( str1 ) << "*" << endl;
    cout << "*" << toupper( str1 ) << "*" << endl;

    cout << "----" << endl;

    tolower_in( str1 );
    cout << "*" << str1 << "*" << endl;
    toupper_in( str1 );
    cout << "*" << str1 << "*" << endl;
}

void algo()
{
    string str1("123xxx321");
    string str2("abc");
    string str3("");

    cout << (start_with( str1, string("123") )?"true":"false") << endl; 
    cout << (start_with( str1, string("1234") )?"true":"false") << endl; 
    cout << (end_with( str1, string("321") )?"true":"false") << endl; 
    cout << (end_with( str1, string("123") )?"true":"false") << endl; 

    cout << (start_with( str2, string("abc") )?"true":"false") << endl; 
    cout << (end_with( str2, string("abc") )?"true":"false") << endl; 

    cout << (start_with( str2, string("abcd") )?"true":"false") << endl; 
    cout << (end_with( str2, string("abcd") )?"true":"false") << endl; 

    cout << (contains( str1, string("xxx") )?"true":"false") << endl; 
    cout << (contains( str1, string("") )?"true":"false") << endl; 
    cout << (contains( str1, string("abc") )?"true":"false") << endl; 


}

void substr()
{
    string str1("123abcxxxabc321");
    const string str2("abc");

	pair<string::iterator, string::iterator> fresult=
		find( str1.begin(), str1.end(), str2.begin(), str2.end() );

	cout << string( fresult.first, str1.end() ) << endl;
	cout << string( str1.begin(), fresult.second ) << endl;

	cout << endl;

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

	cout << replace_first( string("abcabc"), string("abc"), string("YYY") ) << endl; 
	cout << replace_first( string("abcabc"), string(""), string("YYY") ) << endl; 
	cout << replace_all( string("abcabc"), string("abc"), string("YYY") ) << endl; 
	cout << replace_all( string("abcabc"), string("abd"), string("YYY") ) << endl; 
	cout << replace_all( str1, str2, string("") ) << endl;

	cout << erase_all( string("abcabc"), string("abc") ) << endl; 
	cout << erase_first( string("abcabc"), string("abc") ) << endl; 
	cout << erase_all( str1, string("") ) << endl;

	cout << endl;

	replace_first_in( str1, string("x"), string("456") );
	cout << str1 << endl;
	replace_all_in( str1, string("abc"), string("X") );
	cout << str1 << endl;
	replace_all_in( str1, string("X"), string("mno") );
	cout << str1 << endl;
	replace_all_in( str1, string("mno"), string("ZZZ") );
	cout << str1 << endl;
	
	

	cout << endl;
}

int main()
{
	trimtest();
	convtest();
	algo();
	substr();
	replace();

    cout << "Done." << endl;
    cin.get();
}
