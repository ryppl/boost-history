#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/config.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <vector>
#include <fstream>
#include <algorithm>

template< typename Container, typename T >
BOOST_DEDUCED_TYPENAME boost::container_iterator<Container>::type
find( Container& c,  T value )
{
    return std::find( boost::begin( c ), boost::end( c ), value );
}

template< typename Container, typename T >
BOOST_DEDUCED_TYPENAME boost::container_const_iterator<Container>::type
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
    
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_value_type<char_iterator_t>::type, 
                                   /*BOOST_DEDUCED_TYPENAME*/ detail::iterator_traits<char_iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_iterator<char_iterator_t>::type, char_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_const_iterator<char_iterator_t>::type, const char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_difference_type<char_iterator_t>::type,                           
                                   /*BOOST_DEDUCED_TYPENAME*/ detail::iterator_traits<char_iterator_t>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_size_type<char_iterator_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_result_iterator<char_iterator_t>::type, char_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_result_iterator<const char*>::type, const char* >::value ));
    //
    // note: why does is_same< /*BOOST_DEDUCED_TYPENAME*/ container_result_iterator<const char_iterator_t>::type, const char* >::value
    // fail?!?
    
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_value_type<char_array_t>::type, 
                                   /*BOOST_DEDUCED_TYPENAME*/ char>::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_iterator<char_array_t>::type, char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_const_iterator<char_array_t>::type, const char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_difference_type<char_array_t>::type,                           
                                   /*BOOST_DEDUCED_TYPENAME*/ ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_size_type<char_array_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_result_iterator<char_array_t>::type, char* >::value ));
    BOOST_STATIC_ASSERT(( is_same< /*BOOST_DEDUCED_TYPENAME*/ container_result_iterator<const char_array_t>::type, const char* >::value ));

    BOOST_CHECK( begin( char_s ) == char_s );
    BOOST_CHECK( end( char_s )   == (begin( char_s ) + size( char_s )) );
    BOOST_CHECK( empty( char_s ) == (char_s == 0 || char_s[0] == char()) );
    BOOST_CHECK( size( char_s )  == std::char_traits<char>::length( char_s ) );

    char to_search = 'n';
    BOOST_CHECK( find( char_s, to_search ) != end( char_s ) );
    BOOST_CHECK( find( my_string, to_search ) != end( my_string ) );
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
    
    BOOST_STATIC_ASSERT(( is_same< container_value_type<wchar_iterator_t>::type, 
                                   detail::iterator_traits<wchar_iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<wchar_iterator_t>::type, wchar_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<wchar_iterator_t>::type, const wchar_t* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<wchar_iterator_t>::type,                           
                                   detail::iterator_traits<wchar_iterator_t>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<wchar_iterator_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<wchar_iterator_t>::type, wchar_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const wchar_t*>::type, const wchar_t* >::value ));
    
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


