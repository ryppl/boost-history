#include <boost/container_traits/detail/iterator.hpp>
#include <boost/container_traits/detail/const_iterator.hpp>
#include <boost/container_traits/detail/result_iterator.hpp>
#include <boost/container_traits/detail/size_type.hpp>
#include <boost/container_traits/detail/value_type.hpp>
#include <boost/container_traits/detail/difference_type.hpp>

#include <boost/container_traits/detail/begin.hpp>
#include <boost/container_traits/detail/end.hpp>
#include <boost/container_traits/detail/size.hpp>
#include <boost/container_traits/detail/empty.hpp>
#include <boost/container_traits/detail/sfinae.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void check_sfinae()
{
    using namespace container_traits_detail;
    using type_traits::yes_type;
    using type_traits::no_type;

    //////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////
    char*          c_ptr;
    const char*    cc_ptr;
    wchar_t*       w_ptr;
    const wchar_t* cw_ptr;
        
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( c_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( cc_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( w_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( cw_ptr ) ) );
    
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_char_ptr_impl( c_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( no_type ) == sizeof( is_char_ptr_impl( cc_ptr ) ) );

    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_wchar_t_ptr_impl( w_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( no_type ) == sizeof( is_wchar_t_ptr_impl( cw_ptr ) ) );
    
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_char_ptr_impl( c_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_char_ptr_impl( cc_ptr ) ) );
    
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_wchar_t_ptr_impl( w_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_wchar_t_ptr_impl( cw_ptr ) ) );

/*
        //////////////////////////////////////////////////////////////////////
        // pair
        //////////////////////////////////////////////////////////////////////

        template< typename Iterator >
        yes_type is_pair_impl( const std::pair<Iterator,Iterator>* );
        no_type  is_pair_impl( ... );

        //////////////////////////////////////////////////////////////////////
        // iterator
        //////////////////////////////////////////////////////////////////////
        
#ifdef BOOST_MSVC_STD_ITERATOR
        template< typename T1, typename T2 >
        yes_type is_iterator_impl( const std::iterator<T2,T2>* );
        template< typename T1, typename T2 >
        yes_type is_iterator_impl( const std::istream_iterator<T1,T2>* );
#else        
#if BOOST_DINKUMWARE_STDLIB == 1
        template< typename C, typename T, typename D >
        yes_type is_iterator_impl( const std::iterator<C,T,D>* );
        template< typename C, typename T, typename D >
        yes_type is_iterator_impl( const std::istream_iterator<C,T,D>* );
#else
        template< typename C, typename T, typename D, typename P, typename R >
        yes_type is_iterator_impl( const std::iterator<C,T,D,P,R>* );
        template< typename C, typename T, typename D, typename P, typename R >
        yes_type is_iterator_impl( const std::istream_iterator<C,T,D,P>* );
#endif
#endif
        no_type  is_iterator_impl( ... ); 
        */
}



int test_main( int, char*[] )
{
    check_sfinae();

    return 0;
}


