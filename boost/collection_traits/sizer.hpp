// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_SIZER_HPP
#define BOOST_CONTAINER_TRAITS_SIZER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <cstddef>

namespace boost 
{
    namespace container_traits_detail
    {

       template< std::size_t sz >
       class container_traits_size
       {
           char give_size[sz];
       };
    }
    
    //////////////////////////////////////////////////////////////////////
    // constant array sise
    //////////////////////////////////////////////////////////////////////
    
    template< typename T, std::size_t sz >
    container_traits_detail::container_traits_size<sz>
    sizer( const T BOOST_ARRAY_REF[sz] );
    
    template< typename T, std::size_t sz >
    container_traits_detail::container_traits_size<sz>
    sizer( T BOOST_ARRAY_REF[sz] );

} // namespace 'boost'

#endif
