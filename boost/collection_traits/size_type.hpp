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

#include <boost/collection_traits/config.hpp>

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/collection_traits/detail/size_type.hpp>
#else

#include <iterator>
#include <cstddef>
#include <utility>

namespace boost
{
    //////////////////////////////////////////////////////////////////////////
    // default
    //////////////////////////////////////////////////////////////////////////
    
    template< typename C >
    struct size_type_of
    {
        typedef BOOST_DEDUCED_TYPENAME C::size_type type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////

    template< typename Iterator >
    struct size_type_of< std::pair<Iterator,Iterator> >
    {
        typedef std::size_t type;
    };
    
    template< typename Iterator >
    struct size_type_of< const std::pair<Iterator,Iterator> >
    {
        typedef std::size_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // array
    //////////////////////////////////////////////////////////////////////////

    template< typename T, std::size_t sz >
    struct size_type_of< T[sz] >
    {
        typedef std::size_t type;
    };

    template< typename T, std::size_t sz >
    struct size_type_of< const T[sz] >
    {
        typedef std::size_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct size_type_of< char* >
    {
        typedef std::size_t type;
    };

    template<>
    struct size_type_of< wchar_t* >
    {
        typedef std::size_t type;
    };

    template<>
    struct size_type_of< const char* >
    {
        typedef std::size_t type;
    };

    template<>
    struct size_type_of< const wchar_t* >
    {
        typedef std::size_t type;
    };

    //////////////////////////////////////////////////////////////////////////
    // iterator
    //////////////////////////////////////////////////////////////////////////

    template< typename C, typename T, typename D, typename P >
    struct size_type_of< std::istream_iterator<C,T,D,P> >
    {
         typedef std::size_t type;
    };

    
    template< typename C, typename T, typename D, typename P >
    struct size_type_of< const std::istream_iterator<C,T,D,P> >
    {
        typedef std::size_t type;
    };

} // namespace boost

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif
