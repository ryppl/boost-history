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

#ifndef BOOST_PTR_CONTAINER_PTR_SET_HPP
#define BOOST_PTR_CONTAINER_PTR_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <set>
#include <boost/ptr_container/detail/associative_ptr_container.hpp>

namespace boost
{
    
    template< typename Key, typename Compare = ptr_less<Key>, 
              typename Allocator = std::allocator<Key*> >
    class ptr_set : public detail::associative_ptr_container< detail::default_config< std::set<Key*,Compare,Allocator>, Key > >
    {
        typedef detail::associative_ptr_container< detail::default_config< std::set<Key*,Compare,Allocator>, Key > > Base;
    
    public: // typedefs
        //BOOST_FORWARD_ASSOC_TYPEDEF( Base );
        
    public:
        explicit ptr_set( const Compare& comp = Compare(), 
                          const Allocator& alloc = Allocator() ) : Base( comp, alloc ) 
        { }
        
        ptr_set( std::auto_ptr<ptr_set> r ) : Base( r )                
        { }

        template< typename InputIterator >
        ptr_set( InputIterator first, InputIterator last, const Compare& comp = Compare(), 
                 const Allocator& alloc = Allocator() ) 
        : Base( first, last, comp, alloc )
        { }
           
        void operator=( std::auto_ptr<ptr_set> r ) 
        {
            Base::operator=( r );
        }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_set );
    };
        
        
        
    template< typename Key, typename Compare = ptr_less<Key>, 
              typename Allocator = std::allocator<Key*> >
    class ptr_multiset : public detail::associative_ptr_container< detail::default_config< std::multiset<Key*,Compare,Allocator>, Key > >
    {
        typedef detail::associative_ptr_container< detail::default_config< std::multiset<Key*,Compare,Allocator>, Key > > Base;
        
    public: // typedefs
        //BOOST_FORWARD_ASSOC_TYPEDEF( Base );
        
    public:
        explicit ptr_multiset( const Compare& comp = Compare(), 
                               const Allocator& alloc = Allocator() )
        : Base( comp, alloc ) 
        { }

        
        ptr_multiset( std::auto_ptr<ptr_multiset> r ) : Base( r )                
        { }
        
        template< typename InputIterator >
        ptr_multiset( InputIterator first, InputIterator last,
                      const Compare& comp = Compare(), 
                      const Allocator& alloc = Allocator() ) 
        : Base( first, last, comp, alloc ) { }
        
        void operator=( std::auto_ptr<ptr_multiset> r ) 
        {
            Base::operator=( r );
        }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_multiset );
    };

}

#endif
