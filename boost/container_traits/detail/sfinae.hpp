// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_SFINAE_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_SFINAE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

#include <iterator>
#include <utility>


namespace boost 
{
    namespace container_traits_detail
    {          
        using type_traits::yes_type;
        using type_traits::no_type;

        //////////////////////////////////////////////////////////////////////
        // string
        //////////////////////////////////////////////////////////////////////
        
        yes_type is_string_impl( const char* );
        yes_type is_string_impl( const wchar_t* );
        no_type  is_string_impl( ... );
        
        yes_type is_char_ptr_impl( char* );
        no_type  is_char_ptr_impl( ... );
        
        yes_type is_const_char_ptr_impl( const char* );
        no_type  is_const_char_ptr_impl( ... );

        yes_type is_wchar_t_ptr_impl( wchar_t* );
        no_type  is_wchar_t_ptr_impl( ... );
        
        yes_type is_const_wchar_t_ptr_impl( const wchar_t* );
        no_type  is_const_wchar_t_ptr_impl( ... );
        
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
#else        
#if BOOST_DINKUMWARE_STDLIB == 1
        template< typename C, typename T, typename D >
        yes_type is_iterator_impl( const std::iterator<C,T,D>* );
#else
        template< typename C, typename T, typename D, typename P, typename R >
        yes_type is_iterator_impl( const std::iterator<C,T,D,P,R>* );
#endif
#endif
        no_type  is_iterator_impl( ... ); 
        
    } // namespace 'container_traits_detail'
    
} // namespace 'boost'

#endif
