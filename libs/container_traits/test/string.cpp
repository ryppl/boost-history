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

#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

template< typename Container, typename T >
BOOST_DEDUCED_TYPENAME boost::container_traits<Container>::iterator
find( Container& c,  T value )
{
    return std::find( boost::begin( c ), boost::end( c ), value );
}

template< typename Container, typename T >
BOOST_DEDUCE_TYPENAME boost::container_traits<Container>::const_iterator
find( const Container& c, T value )
{
    return std::find( boost::begin( c ), boost::end( c ), value );
}

#else

//
// On a conforming compiler it is much easier...
//
template< typename Container, typename T >
BOOST_DEDUCED_TYPENAME boost::container_traits<Container>::result_iterator
find( Container& c, T value )
{
    return std::find( boost::begin( c ), boost::end( c ), value );
}

#endif

using namespace boost;
using boost::unit_test_framework::test_suite;

template< typename charT >
inline void check_char()
{
    typedef charT*                  char_iterator_t;
    const charT*     char_s       = "a string";
    charT            my_string[]  = "another string";
    
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<char_iterator_t>::value_type, 
                          BOOST_DEDUCED_TYPENAME detail::iterator_traits<char_iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<char_iterator_t>::iterator, char_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<char_iterator_t>::const_iterator, const charT* >::value ));
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<char_iterator_t>::difference_type,                           
                          BOOST_DEDUCED_TYPENAME detail::iterator_traits<char_iterator_t>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<char_iterator_t>::size_type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<char_iterator_t>::result_iterator, char_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< BOOST_DEDUCED_TYPENAME container_traits<const char_iterator_t>::result_iterator, const charT* >::value ));
    
    BOOST_CHECK( begin( char_s ) == char_s );
    BOOST_CHECK( end( char_s )   == (begin( char_s ) + size( char_s )) );
    BOOST_CHECK( empty( char_s ) == (char_s == 0 || char_s[0] == charT()) );
    BOOST_CHECK( size( char_s )  == std::char_traits<charT>::length( char_s ) );

    charT to_search = 'n';
    BOOST_CHECK( find( char_s, to_search ) != end( char_s ) );
    BOOST_CHECK( find( my_string, to_search ) != end( my_string ) );
}

void check_string()
{
    check_char<char>();
//    check_char<volatile char>();
//    check_char<const char>();
//    check_char<const volatile char>();
    
#ifndef BOOST_NO_INTRINSIC_WCHAR_T  
    typedef wchar_t*               wchar_iterator_t;          
    const wchar_t*  char_ws      = L"a wide string";
    wchar_t         my_wstring[] = L"another wide string";
    
    BOOST_STATIC_ASSERT(( is_same< container_traits<wchar_iterator_t>::value_type, 
                          detail::iterator_traits<wchar_iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<wchar_iterator_t>::iterator, wchar_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<wchar_iterator_t>::const_iterator, const wchar_t* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<wchar_iterator_t>::difference_type,                           
                          detail::iterator_traits<wchar_iterator_t>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<wchar_iterator_t>::size_type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<wchar_iterator_t>::result_iterator, wchar_iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<const wchar_iterator_t>::result_iterator, const wchar_t* >::value ));
    
    BOOST_CHECK( begin( char_ws ) == char_ws );
    BOOST_CHECK( end( char_ws )   == (begin( char_ws ) + size( char_ws )) );
    BOOST_CHECK( empty( char_ws ) == (char_ws == 0 || char_ws[0] == wchar_t()) );
    BOOST_CHECK( size( char_ws )  == std::char_traits<wchar_t>::length( char_ws ) );
        
    wchar_t to_search = L'n';
    BOOST_CHECK( find( char_ws, to_search ) != end( char_ws ) );    
    BOOST_CHECK( find( my_wstring, to_search ) != end( my_wstring ) );
#endif  

}



int test_main( int, char*[] )
{
    check_string();

    return 0;
}


