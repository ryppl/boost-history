// (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CONTAINER_TRAITS_RESULT_ITERATOR_HPP
#define BOOST_CONTAINER_TRAITS_RESULT_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
#include <boost/container_traits/detail/result_iterator.hpp>
#else

#include <boost/container_traits/config.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <iterator>
#include <cstddef>
#include <utility>

namespace boost
{
    //////////////////////////////////////////////////////////////////////////
    // default
    //////////////////////////////////////////////////////////////////////////
    
    template< typename C >
    struct container_result_iterator
    {
        typedef BOOST_CT_DEDUCED_TYPENAME mpl::if_< BOOST_DEDUCED_TYPENAME is_const<C>::type, 
                                                    BOOST_DEDUCED_TYPENAME C::const_iterator,
                                                    BOOST_DEDUCED_TYPENAME C::iterator >::type type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // default
    //////////////////////////////////////////////////////////////////////////

    template< typename Iterator >
    struct container_result_iterator< std::pair<Iterator,Iterator> >
    {
        typedef Iterator type;
    };
    
    template< typename Iterator >
    struct container_result_iterator< const std::pair<Iterator,Iterator> >
    {
        typedef Iterator type;
    };

    //////////////////////////////////////////////////////////////////////////
    // array
    //////////////////////////////////////////////////////////////////////////

    template< typename T, std::size_t sz >
    struct container_result_iterator< T[sz] >
    {
        typedef T* type;
    };

    template< typename T, std::size_t sz >
    struct container_result_iterator< const T[sz] >
    {
        typedef const T* type;
    };

    //////////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////////

    template<>
    struct container_result_iterator< char* >
    {
        typedef char* type;
    };

    template<>
    struct container_result_iterator< wchar_t* >
    {
        typedef wchar_t* type;
    };

    template<>
    struct container_result_iterator< const char* >
    {
        typedef const char* type;
    };

    template<>
    struct container_result_iterator< const wchar_t* >
    {
        typedef const wchar_t* type;
    };
    
    //////////////////////////////////////////////////////////////////////////
    // iterator
    //////////////////////////////////////////////////////////////////////////

    template< typename C, typename T, typename D, typename P >
    struct container_result_iterator< std::istream_iterator<C,T,D,P> >
    {
        typedef std::istream_iterator<C,T,D,P> type;
    };

    template< typename C, typename T, typename D, typename P >
    struct container_result_iterator< const std::istream_iterator<C,T,D,P> >
    {
        typedef std::istream_iterator<C,T,D,P> type;
    };

} // namespace boost

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif
