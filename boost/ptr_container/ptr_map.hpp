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

#ifndef BOOST_PTR_CONTAINER_PTR_MAP_HPP
#define BOOST_PTR_CONTAINER_PTR_MAP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <map>
#include <boost/ptr_container/detail/associative_ptr_container.hpp>

namespace boost
{
    

template< typename Key, typename T, typename Compare = std::less<Key>, 
          typename Allocator = std::allocator<T*> >
class ptr_map : public detail::associative_ptr_container< std::map<Key,T*,Compare,Allocator>, T >
{
    typedef detail::associative_ptr_container< std::map<Key,T*,Compare,Allocator>, T > Base;

public: // typedefs
    BOOST_FORWARD_ASSOC_TYPEDEF( Base );
        
public:
    explicit ptr_map( const Compare& comp = Compare(), 
             const allocator_type& alloc = allocator_type() ) : Base( comp, alloc ) ) {}
    explicit ptr_map( std::auto_ptr<ptr_map> r ) : Base( r ) {}
    template< typename InputIterator >
    ptr_map( InputIterator first, InputIterator last ) : Base( first, last ) {}
    void operator=( std::auto_ptr<ptr_map> r ) { Base::operator=( r );}

};



template< typename Key, typename T, typename Compare = std::less<Key>, 
          typename Allocator = std::allocator<T*> >
class ptr_multimap : public detail::associative_ptr_container< std::map<Key,T*,Compare,Allocator>, T >
{
    typedef detail::associative_ptr_container< std::map<Key,T*,Compare,Allocator>, T > Base;

public: // typedefs
    BOOST_FORWARD_ASSOC_TYPEDEF( Base );
        
public:
    ptr_multimap( const Compare& comp = Compare(), 
                  const allocator_type& alloc = allocator_type() ) : Base( comp, alloc ) ) {}
    ptr_multimap( std::auto_ptr<ptr_multimap> r ) : Base( r )             {}
    
    template< typename InputIterator >
    ptr_multimap( InputIterator first, InputIterator last ) : Base( first, last ) {}
    void operator=( std::auto_ptr<ptr_multimap> r ) { Base::operator=( r );}

};

}

#endif
