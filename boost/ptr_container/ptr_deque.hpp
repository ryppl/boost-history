/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */

#ifndef BOOST_PTR_CONTAINER_PTR_DEQUE_HPP
#define BOOST_PTR_CONTAINER_PTR_DEQUE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <deque>
#include <boost/ptr_container/detail/reversible_ptr_container.hpp>

namespace boost
{

template< typename T, typename Allocator = std::allocator<T*> >
class ptr_deque : public detail::reversible_ptr_container< std::deque<T*,Allocator>, T >
{
    typedef detail::reversible_ptr_container< std::deque<T*,Allocator>, T > Base;

public: 

    ptr_deque() : Base() {}
    ptr_deque( std::auto_ptr<ptr_deque> r )              : Base( r )             {}
    ptr_deque( typename Base::release_type r )           : Base( r )                {}
    template< typename InputIterator >
    ptr_deque( InputIterator first, InputIterator last ) : Base( first, last ) {}
    void operator=( auto_ptr<ptr_deque> r )              { Base::operator=( r ); }

    BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_deque );
};


}

#undef BOOST_FORWARD_TYPEDEF
#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

#endif
