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

int main()
{
	trimtest();
	cout << endl << "%%" << endl << endl;
	convtest();

	cout << "Done." << endl;
	cin.get();
}
