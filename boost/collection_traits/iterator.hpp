// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef BOOST_CONTAINER_TRAITS_ITERATOR_HPP
#define BOOST_CONTAINER_TRAITS_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/collection_traits/config.hpp>

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/collection_traits/detail/iterator.hpp>
#else

#include <boost/iterator/iterator_traits.hpp>
#include <cstddef>
#include <utility>

namespace boost
{
    //////////////////////////////////////////////////////////////////////////
    // default
    //////////////////////////////////////////////////////////////////////////
    
    template< typename C >
    struct iterator_of
    {
        typedef BOOST_DEDUCED_TYPENAME C::iterator type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // pair
    //////////////////////////////////////////////////////////////////////////

    template< typename Iterator >
    struct iterator_of< std::pair<Iterator,Iterator> >
    {
        typedef Iterator type;
    };
    
    template< typename Iterator >
    struct iterator_of< const std::pair<Iterator,Iterator> >
    {
        typedef Iterator type;
    };

    //////////////////////////////////////////////////////////////////////////
    // array
    //////////////////////////////////////////////////////////////////////////

    template< typename T, std::size_t sz >
    struct iterator_of< T[sz] >
    {
        typedef T* type;
    };

    template< typename T, std::size_t sz >
    struct iterator_of< const T[sz] >
    {
        // remark: is this correct? I think T* should be it!
        typedef const T* type;
    };

    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct iterator_of< char* >
    {
        typedef char* type;
    };

    template<>
    struct iterator_of< wchar_t* >
    {
        typedef wchar_t* type;
    };

    template<>
    struct iterator_of< const char* >
    {
        typedef const char* type;
    };

    template<>
    struct iterator_of< const wchar_t* >
    {
        typedef const wchar_t* type;
    };

} // namespace boost

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif
