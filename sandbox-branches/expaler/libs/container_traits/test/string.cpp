#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/config.hpp>
#include <vector>
#include <fstream>
#include <algorithm>

template< typename Container, typename T >
BOOST_DEDUCED_TYPENAME boost::iterator_of<Container>::type
find( Container& c,  T value )
{
    return std::find( boost::begin( c ), boost::end( c ), value );
}

template< typename Container, typename T >
BOOST_DEDUCED_TYPENAME boost::const_iterator_of<Container>::type
find( const Container& c, T value )
{
    return std::find( boost::begin( c ), boost::end( c ), value );
}

std::vector<char> 
check_rvalue_return()
{
    return std::vector<char>( 10, 'm' ); 
}

using namespace boost;


void check_char()
{
    typedef char*                  char_iterator_t;
    typedef char                   char_array_t[10];
    const char*      char_s       = "a string";
    char             my_string[]  = "another string";

    
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ value_type_of<char_iterator_t>::type, 
                                   /*BOOST_DEDUCED_TYPENAME*/ detail::iterator_traits<char_iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ iterator_of<char_iterator_t>::type, char_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ const_iterator_of<char_iterator_t>::type, const char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ difference_type_of<char_iterator_t>::type,                           
		                           /*BOOST_DEDUCED_TYPENAME*/ ::std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ size_type_of<char_iterator_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ result_iterator_of<char_iterator_t>::type, char_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ result_iterator_of<const char*>::type, const char* >::value ));
    //
    // note: why does is_same< /*BOOST_DEDUCED_TYPENAME*/ result_iterator<const char_iterator_t>::type, const char* >::value
    // fail?!?
    
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ value_type_of<char_array_t>::type, 
                                   /*BOOST_DEDUCED_TYPENAME*/ char>::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ iterator_of<char_array_t>::type, char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ const_iterator_of<char_array_t>::type, const char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ difference_type_of<char_array_t>::type,                           
		                           /*BOOST_DEDUCED_TYPENAME*/ ::std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ size_type_of<char_array_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ result_iterator_of<char_array_t>::type, char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ result_iterator_of<const char_array_t>::type, const char* >::value ));

    BOOST_CHECK( begin( char_s ) == char_s );
	const char* end = begin( char_s ) + size( char_s );
    BOOST_CHECK( boost::end( char_s ) == end );
    BOOST_CHECK( empty( char_s ) == (char_s == 0 || char_s[0] == char()) );
    BOOST_CHECK( size( char_s )  == std::char_traits<char>::length( char_s ) );
    
	BOOST_CHECK( begin( my_string ) == my_string );
	const char* end2 = begin( my_string ) + size( my_string );
    BOOST_CHECK( boost::end( my_string ) == end2 );
    BOOST_CHECK( empty( my_string ) == (my_string == 0 || my_string[0] == char()) );
    BOOST_CHECK( size( my_string )  == std::char_traits<char>::length( my_string ) );


    char to_search = 'n';
    BOOST_CHECK( find( char_s, to_search ) != boost::end( char_s ) );
    BOOST_CHECK( find( my_string, to_search ) != boost::end( my_string ) );
}

void check_string()
{
    check_char();
//    check_char<volatile char>();
//    check_char<const char>();
//    check_char<const volatile char>();
    
#ifndef BOOST_NO_INTRINSIC_WCHAR_T  
    typedef wchar_t*               wchar_iterator_t;          
    const wchar_t*  char_ws      = L"a wide string";
    wchar_t         my_wstring[] = L"another wide string";
    
    BOOST_STATIC_ASSERT(( is_same< value_type_of<wchar_iterator_t>::type, 
                                   detail::iterator_traits<wchar_iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< iterator_of<wchar_iterator_t>::type, wchar_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< const_iterator_of<wchar_iterator_t>::type, const wchar_t* >::value ));
    BOOST_STATIC_ASSERT(( is_same< difference_type_of<wchar_iterator_t>::type,                           
                                   detail::iterator_traits<wchar_iterator_t>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< size_type_of<wchar_iterator_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< result_iterator_of<wchar_iterator_t>::type, wchar_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< result_iterator_of<const wchar_t*>::type, const wchar_t* >::value ));
    
    BOOST_CHECK( begin( char_ws ) == char_ws );
    BOOST_CHECK( end( char_ws )   == (begin( char_ws ) + size( char_ws )) );
    BOOST_CHECK( empty( char_ws ) == (char_ws == 0 || char_ws[0] == wchar_t()) );
    BOOST_CHECK( size( char_ws )  == std::char_traits<wchar_t>::length( char_ws ) );
        
    wchar_t to_search = L'n';
    BOOST_CHECK( find( char_ws, to_search ) != end( char_ws ) );    
    BOOST_CHECK( find( my_wstring, to_search ) != end( my_wstring ) );
#endif  
    
    find( check_rvalue_return(), 'n' );
	
}



int test_main( int, char*[] )
{
    check_string();

    return 0;
}


