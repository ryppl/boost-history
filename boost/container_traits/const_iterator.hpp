// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_CONST_ITERATOR_HPP
#define BOOST_CONTAINER_TRAITS_CONST_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/container_traits/detail/const_iterator.hpp>
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
    struct container_const_iterator_
    {
        typedef BOOST_DEDUCED_TYPENAME C::const_iterator type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////

    template< typename Iterator >
    struct container_const_iterator< std::pair<Iterator,Iterator> >
    {
        typedef Iterator type;
    };
    
    template< typename Iterator >
    struct container_const_iterator< const std::pair<Iterator,Iterator> >
    {
        typedef Iterator type;
    };

    //////////////////////////////////////////////////////////////////////////
    // array
    //////////////////////////////////////////////////////////////////////////

    template< typename T, std::size_t sz >
    struct container_const_iterator< T[sz] >
    {
        typedef const T* type;
    };

    template< typename T, std::size_t sz >
    struct container_const_iterator< const T[sz] >
    {
        typedef const T* type;
    };

    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct container_const_iterator< char* >
    {
        typedef const char* type;
    };

    template<>
    struct container_const_iterator< wchar_t* >
    {
        typedef const wchar_t* type;
    };

    template<>
    struct container_const_iterator< const char* >
    {
        typedef const char* type;
    };

    template<>
    struct container_const_iterator< const wchar_t* >
    {
        typedef const wchar_t* type;
    };

    //////////////////////////////////////////////////////////////////////////
    // iterator
    //////////////////////////////////////////////////////////////////////////

    template< typename C, typename T, typename D, typename P >
    struct container_const_iterator< std::istream_iterator<C,T,D,P> >
    {
        typedef std::istream_iterator<C,T,D,P> type;
    };

    template< typename C, typename T, typename D, typename P >
    struct container_const_iterator< const std::istream_iterator<C,T,D,P> >
    {
        typedef std::istream_iterator<C,T,D,P> type;
    };

} // namespace boost

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif
