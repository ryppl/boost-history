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
    

template< typename Key, typename T, typename Compare = ptr_less<Key>, 
          typename Allocator = std::allocator<T*> >
class ptr_map : public detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > >
{
    typedef detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > > Base;

public:
    explicit ptr_map( const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) 
    : Base( comp, alloc ) { }
    
    ptr_map( std::auto_ptr<ptr_map> r ) 
    : Base( r ) { }
    
    template< typename InputIterator >
    ptr_map( InputIterator first, InputIterator last, 
             const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) 
    : Base( first, last, comp, alloc ) { }
    
    void operator=( std::auto_ptr<ptr_map> r ) 
    {
        Base::operator=( r );
    }
    
    BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_map );
};



template< typename Key, typename T, typename Compare = ptr_less<Key>, 
          typename Allocator = std::allocator<T*> >
class ptr_multimap : public detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > >
{
    typedef detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > > Base;

public:
    explicit ptr_multimap( const Compare& comp = Compare(), 
                  const Allocator& alloc = Allocator() ) 
    : Base( comp, alloc ) { }
    
    ptr_multimap( std::auto_ptr<ptr_multimap> r ) 
    : Base( r ) { }
    
    template< typename InputIterator >
    ptr_multimap( InputIterator first, InputIterator last,
                   const Compare& comp = Compare(), const Allocator& alloc = Allocator() )
    : Base( first, last, comp, alloc ) { }
    
    void operator=( std::auto_ptr<ptr_multimap> r ) 
    {
        Base::operator=( r );
    }

    BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_multimap );
};

}

#endif
