#include <iostream>

using namespace std;

/*

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

*/

extern void conv_example();
extern void trim_example();
extern void predicate_example();
extern void find_example();
extern void rle_example();

int main()
{
    conv_example();
    trim_example();
    predicate_example();
    find_example();
    rle_example();

    cout << "Done." << endl;
    cin.get();

    return 0;
}
