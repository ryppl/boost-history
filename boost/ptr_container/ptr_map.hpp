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
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_map : public detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > >
    {
        typedef detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > > Base;
    
    public:
            // mapped_type ?
        
        typedef BOOST_DEDUCED_TYPENAME Base::iterator     iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator;         
        typedef BOOST_DEDUCED_TYPENAME Base::object_type  object_type;         

    private:
           
        template< typename II >                                               
        void map_basic_clone_and_insert( II first, II last )                  
        {                                                                     
            while( first != last )                                            
            {                                            
                if( this->find( first.key() ) != this->end() )
                {
                    const object_type& pair = *first.base();            // nothrow                     
                    std::auto_ptr<T> ptr( make_clone( *pair.second ) ); // strong
                    insert( pair.first, ptr.get() );                    // strong, commit
                    ptr.release();                                      // nothrow
                }
                ++first;                                                      
            }                                                                 
        }
        
    public:
        explicit ptr_map( const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc ) { }

        template< typename InputIterator >
        ptr_map( InputIterator first, InputIterator last, 
                 const Compare& comp = Compare(), const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc ) 
        {
            map_basic_clone_and_insert( first, last );
        }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_map );
        
        using Base::insert;
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last ) // basic
        {
            map_basic_clone_and_insert( first, last );
        }
        
        std::pair<iterator,bool> insert( const Key& key, T* x ) // strong
        {
            std::auto_ptr<T> ptr( x );                                                         // nothrow
            std::pair<ptr_iterator,bool> res = this->c__().insert( std::make_pair( key, x ) ); // strong, commit      
            if( res.second )                                                                   // nothrow     
                ptr.release();                                                                 // nothrow
            return std::make_pair( iterator( res.first ), res.second );                        // nothrow   
        }
        
        std::pair<iterator,bool> insert( const Key& key, const T& x ) // strong
        {
            return insert( key, make_clone( x ) );
        }

    };
    


    template< typename Key, typename T, typename Compare = std::less<Key>, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_multimap : public detail::associative_ptr_container< detail::map_config< std::multimap<Key,T*,Compare,Allocator>, T > >
    {
        typedef detail::associative_ptr_container< detail::map_config< std::multimap<Key,T*,Compare,Allocator>, T > > Base;
    
    public: // typedefs
        // mapped_type
        typedef BOOST_DEDUCED_TYPENAME Base::iterator     iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator;        
        typedef BOOST_DEDUCED_TYPENAME Base::object_type  object_type;          

    private:
        
        template< typename II >                                               
        void map_basic_clone_and_insert( II first, II last )                  
        {                                                                     
            while( first != last )                                            
            {                                            
                const object_type& pair = *first.base();            // nothrow                     
                std::auto_ptr<T> ptr( make_clone( *pair.second ) ); // strong
                insert( pair.first, ptr.get() );                    // strong, commit
                ptr.release();                                      // nothrow
                ++first;                                                      
            }                                                                 
        }
        
    public:
        explicit ptr_multimap( const Compare& comp = Compare(), 
                      const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc ) { }
        
        template< typename InputIterator >
        ptr_multimap( InputIterator first, InputIterator last,
                      const Compare& comp = Compare(), const Allocator& alloc = Allocator() )
        : Base( comp, alloc ) 
        {
            map_basic_clone_and_insert( first, last );
        }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_multimap );
        
        using Base::insert;
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last ) // basic
        {
            map_basic_clone_and_insert( first, last );
        }
        
        iterator insert( const Key& key, T* x ) // strong
        {
            std::auto_ptr<T> ptr( x );                                         // nothrow
            ptr_iterator res = this->c__().insert( std::make_pair( key, x ) ); // strong, commit        
            ptr.release();                                                     // notrow
            return iterator( res );           
        }
        
        iterator insert( const Key& key, const T& x ) // strong
        {
            return insert( key, make_clone( x ) );
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
