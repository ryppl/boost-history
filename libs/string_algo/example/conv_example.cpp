#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <boost/string_algo/conv.hpp>

using namespace std;
using namespace boost;

void conv_example()
{  
    cout << "* Case Conversion Example *" << endl << endl;

    string str1("AbCdEfG");
    vector<char> vec1( str1.begin(), str1.end() );

    cout << "lower-cased copy of vec1: ";
    tolower_copy( ostream_iterator<char>(cout), vec1.begin(), vec1.end() );
    cout << endl;

    cout << "upper-cased copy of str1: " << toupper_copy( str1 ) << endl;

    tolower( str1 );
    cout << "lower-cased str1: " << str1 << endl;

    cout << endl;
}
