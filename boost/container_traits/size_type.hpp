// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_SIZE_TYPE_HPP
#define BOOST_CONTAINER_TRAITS_SIZE_TYPE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <iterator>
#include <cstddef>
#include <utility>

namespace boost
{
    //////////////////////////////////////////////////////////////////////////
    // default
    //////////////////////////////////////////////////////////////////////////
    
    template< typename C >
    struct container_size_type
    {
        typedef BOOST_DEDUCED_TYPENAME C::size_type type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////

    template< typename Iterator >
    struct container_size_type< std::pair<Iterator,Iterator> >
    {
        typedef std::size_t type;
    };
    
    template< typename Iterator >
    struct container_size_type< const std::pair<Iterator,Iterator> >
    {
        typedef std::size_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // array
    //////////////////////////////////////////////////////////////////////////

    template< typename T, std::size_t sz >
    struct container_size_type< T[sz] >
    {
        typedef std::size_t type;
    };

    template< typename T, std::size_t sz >
    struct container_size_type< const T[sz] >
    {
        typedef std::size_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct container_size_type< char* >
    {
        typedef std::size_t type;
    };

    template<>
    struct container_size_type< wchar_t* >
    {
        typedef std::size_t type;
    };

    template<>
    struct container_size_type< const char* >
    {
        typedef std::size_t type;
    };

    template<>
    struct container_size_type< const wchar_t* >
    {
        typedef std::size_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // iterator
    //////////////////////////////////////////////////////////////////////////

    template< typename C, typename T, typename D, typename P >
    struct container_size_type< std::istream_iterator<C,T,D,P> >
    {
         typedef std::size_t type;
    };

    
    template< typename C, typename T, typename D, typename P >
    struct container_size_type< const std::istream_iterator<C,T,D,P> >
    {
        typedef std::size_t type;
    };

} // namespace boost

#endif
