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

#ifndef BOOST_PTR_CONTAINER_ARRAY_HPP
#define BOOST_PTR_CONTAINER_ARRAY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/array.hpp>
#include <boost/ptr_container/detail/reversible_ptr_container.hpp>

namespace boost
{

template< typename T, std::size_t size >
class ptr_array : public detail::reversible_ptr_container< ::boost::array<T*,size>, T >
{
    typedef detail::reversible_ptr_container< ::boost::array<T*,size>, T > Base;

public: // typedefs
    BOOST_FORWARD_TYPEDEF( Base );
    
public:

    ptr_array() : Base() {}
    ptr_array( std::auto_ptr<ptr_array> r )                            : Base( r )             {}
    ptr_array( typename Base::release_type r )                         : Base( r )             {}
    template< typename InputIterator >
    ptr_array( InputIterator first, InputIterator last )               : Base( first, last ) {}
    void operator=( std::auto_ptr<ptr_array> r )                       { Base::operator=( r );}

    BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_array );
};

}

#undef BOOST_FORWARD_TYPEDEF
#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

#endif
