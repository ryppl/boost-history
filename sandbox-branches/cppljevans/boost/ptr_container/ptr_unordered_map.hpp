/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003-2004
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

#ifndef BOOST_PTR_CONTAINER_PTR_UNORDERED_MAP_HPP
#define BOOST_PTR_CONTAINER_PTR_UNORDEERD_MAP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <map>
#include <boost/ptr_container/detail/associative_ptr_container.hpp>

namespace boost
{
    template< typename M, typename T >
    struct unordered_map_config
    {
        typedef M                                                                                                               container_type;
        typedef T                                                                                                               value_type;
        typedef typename ptr_container::detail::map_iterator<typename M::iterator, typename M::key_type, T>                     iterator;
        typedef typename ptr_container::detail::map_iterator<typename M::const_iterator, typename M::key_type, const T>         const_iterator;
        typedef typename ptr_container::detail::map_iterator<typename M::reverse_iterator, typename M::key_type, T>             reverse_iterator;
        typedef typename ptr_container::detail::map_iterator<typename M::const_reverse_iterator, typename M::key_type, const T> const_reverse_iterator;
        typedef std::pair<const typename M::key_type, T*>                                                                       object_type;
    };

    

    template< typename Key, typename T, typename Compare = std::less<Key>, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_unordered_map : public detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > >
    {
        typedef detail::associative_ptr_container< detail::map_config< std::map<Key,T*,Compare,Allocator>, T > > Base;
    
    public:    
        typedef T*                                              mapped_type;
        typedef BOOST_DEDUCED_TYPENAME Base::iterator           iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::const_iterator     const_iterator;           
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator       ptr_iterator;         
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_const_iterator ptr_const_iterator;         
        typedef BOOST_DEDUCED_TYPENAME Base::object_type        object_type;         
        typedef BOOST_DEDUCED_TYPENAME Base::size_type          size_type;

    private:
           
        void safe_insert( const Key& key, auto_ptr<T> ptr ) // strong
        {
            std::pair<ptr_iterator,bool> res = 
                this->c__().insert( std::make_pair( key, ptr.get() ) ); // strong, commit      
            if( res.second )                                            // nothrow     
                ptr.release();                                          // nothrow
        }

        template< typename II >                                               
        void map_basic_clone_and_insert( II first, II last )                  
        {                                                                     
            while( first != last )                                            
            {                                            
                if( this->find( first.key() ) != this->end() )
                {
                    const object_type& pair = *first.base();       // nothrow                    
                    auto_ptr<T> ptr( make_clone( *pair.second ) ); // strong 
                    this->safe_insert( pair.first, ptr );          // strong, commit
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
        
        //using Base::insert;
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last ) // basic
        {
            map_basic_clone_and_insert( first, last );
        }
        
        std::pair<iterator,bool> insert( Key& key, T* x ) // strong
        {
            std::auto_ptr<T> ptr( x );                                                         // nothrow
            if( x == 0 )
                throw bad_pointer( "Null pointer in 'ptr_map::insert()'" );
            
            std::pair<ptr_iterator,bool> res = this->c__().insert( std::make_pair( key, x ) ); // strong, commit      
            if( res.second )                                                                   // nothrow     
                ptr.release();                                                                 // nothrow
            return std::make_pair( iterator( res.first ), res.second );                        // nothrow   
        }
        
        std::pair<iterator,bool> insert( Key& key, const T& x ) // strong
        {
            return insert( key, make_clone( x ) );
        }

        BOOST_PTR_MAP_ALGORITHMS( Key, T );
    };
    


    template< typename Key, typename T, typename Compare = std::less<Key>, 
              typename Allocator = std::allocator< std::pair<const Key, T*> > >
    class ptr_unordered_multimap : public detail::associative_ptr_container< detail::map_config< std::multimap<Key,T*,Compare,Allocator>, T > >
    {
        typedef detail::associative_ptr_container< detail::map_config< std::multimap<Key,T*,Compare,Allocator>, T > > Base;
    
    public: // typedefs
        typedef T*                                              mapped_type;
        typedef BOOST_DEDUCED_TYPENAME Base::iterator           iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::const_iterator     const_iterator;           
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator       ptr_iterator;         
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_const_iterator ptr_const_iterator;         
        typedef BOOST_DEDUCED_TYPENAME Base::object_type        object_type;         
        typedef BOOST_DEDUCED_TYPENAME Base::size_type          size_type;

    private:
        
        void safe_insert( const Key& key, auto_ptr<T> ptr ) // strong
        {
            this->c__().insert( std::make_pair( key, ptr.get() ) ); // strong, commit      
            ptr.release();                                          // nothrow
        }

        template< typename II >                                               
        void map_basic_clone_and_insert( II first, II last )                  
        {                                                                     
            while( first != last )                                            
            {                                            
                const object_type& pair = *first.base();            // nothrow                     
                std::auto_ptr<T> ptr( make_clone( *pair.second ) ); // strong
                safe_insert( pair.first, ptr );                     // strong, commit
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
        
        //using Base::insert;
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last ) // basic
        {
            map_basic_clone_and_insert( first, last );
        }
        
        iterator insert( Key& key, T* x ) // strong
        {
            std::auto_ptr<T> ptr( x );                                         // nothrow
            if( x == 0 )
                throw bad_pointer( "Null pointer in 'ptr_multimap::insert()'" );

            ptr_iterator res = this->c__().insert( std::make_pair( key, x ) ); // strong, commit        
            ptr.release();                                                     // notrow
            return iterator( res );           
        }
        
        iterator insert( Key& key, const T& x ) // strong
        {
            return insert( key, make_clone( x ) );
        }
        
        BOOST_PTR_MAP_ALGORITHMS( Key, T );
    };

    //////////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename K, typename T, typename C, typename A >
    ptr_map<K,T,C,A>* make_clone( const ptr_map<K,T,C,A>& r )
    {
        return r.clone().release();
    }

    template< typename K, typename T, typename C, typename A >
    ptr_multimap<K,T,C,A>* make_clone( const ptr_multimap<K,T,C,A>& r )
    {
        return r.clone().release();
    }

}

#endif
