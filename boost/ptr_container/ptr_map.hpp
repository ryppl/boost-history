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
    //////////////////////////////////////////////////////////////////////////////
    // clonability of map values
    
    namespace foo
    {

        template< typename K, typename T, typename C, typename A >
            typename std::map<K,T,C,A>::value_type make_clone( const typename std::map<K,T,C,A>::value_type& pair )
        {
            typename std::map<K,T,C,A>::value_type( pair.first, make_clone( *pair.second ) );
        }
        
        template< typename K, typename T, typename C, typename A >
            typename std::multimap<K,T,C,A>::value_type make_clone( const typename std::multimap<K,T,C,A>::value_type& pair )
        {
            typename std::multimap<K,T,C,A>::value_type( pair.first, make_clone( *pair.second ) );
        }
    }
    
    
    //////////////////////////////////////////////////////////////////////////////
    // ptr_less specializarion
    /*
    template< typename 
    struct map_ptr_less
    {
        bool operator()( const P& l, const P& r ) const
        {
            return *l.second < *r.second;
        }
    };
    */
    

    template< typename Key, typename T, typename Compare = std::less<Key>, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_map : public detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > >
    {
        typedef detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > > Base;
    
    public:
        // mapped_type
               
        typedef BOOST_DEDUCED_TYPENAME Base::iterator     iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator;         

    public:
        explicit ptr_map( const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc ) { }

        template< typename InputIterator >
        ptr_map( InputIterator first, InputIterator last, 
                 const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) 
        : Base( first, last, comp, alloc ) { }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_map );
        
        using Base::insert;
        
        /*
        std::pair<iterator,bool> insert( const T& t ) // strong
        {
            return insert( make_clone( t ) );
        }
        */
        
        std::pair<iterator,bool> insert( std::pair<const Key, T*>& p ) // strong
        {
            std::auto_ptr<T> ptr( p.second );
            std::pair<ptr_iterator,bool> res = this->c__().insert( p );       
            if( res.second )                                                 
                ptr.release();                                                  
            return std::make_pair( iterator( res.first ), res.second );
        }
    };
    


    template< typename Key, typename T, typename Compare = std::less<Key>, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_multimap : public detail::associative_ptr_container< detail::map_config< std::multimap<Key,T*,Compare,Allocator>, T > >
    {
        typedef detail::associative_ptr_container< detail::map_config< std::multimap<Key,T*,Compare,Allocator>, T > > Base;
    
    public: // typedefs
        typedef BOOST_DEDUCED_TYPENAME Base::iterator     iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator;         

    public:
        explicit ptr_multimap( const Compare& comp = Compare(), 
                      const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc ) { }
        
        template< typename InputIterator >
        ptr_multimap( InputIterator first, InputIterator last,
                      const Compare& comp = Compare(), const Allocator& alloc = Allocator() )
        : Base( first, last, comp, alloc ) { }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_multimap );
        
        using Base::insert;
        /*
        iterator insert( const std::pair<const Key,T*>& t ) // strong
        {
            return insert( make_clone( t ) );
        }
        */
        iterator insert( const std::pair<const Key, T*>& p ) // strong
        {
            std::auto_ptr<T> ptr( p.second );
            ptr_iterator res = this->c__().insert( p );       
            ptr.release();                                                  
            return iterator( res );
        }
    };

    //////////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename K, typename T, typename C, typename A >
    ptr_map<K,T,C,A>* make_clone( const ptr_map<K,T,C,A>& r )
    {
        std::auto_ptr<ptr_map<K,T,C,A> > p( r.clone() );
        return p.release();
    }

    template< typename K, typename T, typename C, typename A >
    ptr_multimap<K,T,C,A>* make_clone( const ptr_multimap<K,T,C,A>& r )
    {
        std::auto_ptr<ptr_multimap<K,T,C,A> > p( r.clone() );
        return p.release();
    }

}

#endif
