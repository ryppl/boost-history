//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_VALUE_TYPE_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_VALUE_TYPE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/container_traits/detail/common.hpp>
#include <boost/detail/iterator.hpp>

//////////////////////////////////////////////////////////////////////////////
// missing partial specialization  workaround.
//////////////////////////////////////////////////////////////////////////////

namespace boost 
{
    namespace container_traits_detail 
    {        
        template< typename T >
        struct container_value_type_;

        template<>
        struct container_value_type_<std_container_>
        {
            template< typename C >
            struct pts
            {
                typedef BOOST_DEDUCED_TYPENAME C::value_type type;
            };
        };

        template<>
        struct container_value_type_<std_pair_>
        {
            template< typename P >
            struct pts
            {
                typedef BOOST_CT_DEDUCED_TYPENAME detail::iterator_traits< BOOST_DEDUCED_TYPENAME P::first_type >::value_type type;
            };
        };

        template<>
        struct container_value_type_<array_>; // give up

        template<>
        struct container_value_type_<char_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef char type; 
            };         
        };
        
        template<>
        struct container_value_type_<const_char_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef const char type;
            };         
        };

        template<>
        struct container_value_type_<wchar_t_ptr_>
        {
            template< typename S >
            struct pts
            {
                typedef wchar_t type;
            };         
        };

        template<>
        struct container_value_type_<const_wchar_t_ptr_>
        {
             template< typename S >
             struct pts
             {
                 typedef const wchar_t type;
             };         
         };

        template<>
        struct container_value_type_<iterator_>
        {
            template< typename I >
            struct pts
            {
                typedef BOOST_CT_DEDUCED_TYPENAME detail::iterator_traits<I>::value_type type;
            };         
        };
    } 
    
    template< typename C >
    class value_type_of
    {
        typedef BOOST_DEDUCED_TYPENAME container_traits_detail::container<C>::type c_type;
    public:
        typedef BOOST_DEDUCED_TYPENAME container_traits_detail::container_value_type_<c_type>::BOOST_NESTED_TEMPLATE pts<C>::type type; 
    };

}

#endif

