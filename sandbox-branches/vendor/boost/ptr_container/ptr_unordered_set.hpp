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

#ifndef BOOST_PTR_CONTAINER_PTR_UNORDERED_SET_HPP
#define BOOST_PTR_CONTAINER_PTR_UNORDERED_SET_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <set>
#include <boost/ptr_container/detail/associative_ptr_container.hpp>

namespace boost
{
    
    template< typename Key, typename Compare = ptr_less<Key>, 
              typename Allocator = std::allocator<Key*> >
    class ptr_unordered_set : public detail::associative_ptr_container< detail::default_config< std::set<Key*,Compare,Allocator>, Key > >
    {
        typedef detail::associative_ptr_container< detail::default_config< std::set<Key*,Compare,Allocator>, Key > > Base;
    
    public: // typedefs
       
        typedef BOOST_DEDUCED_TYPENAME Base::iterator     iterator;                 
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator;   
        typedef BOOST_DEDUCED_TYPENAME Base::size_type    size_type;         

    private:
        
        template< typename II >                                               
        void set_basic_clone_and_insert( II first, II last ) // basic                 
        {                                                                     
            while( first != last )                                            
            {           
                if( find( *first ) != this->end() )
                    insert( make_clone( *first ) ); // strong, commit
                ++first;                                                      
            }                                                                 
        }                         

    public:
        explicit ptr_set( const Compare& comp = Compare(), 
                          const Allocator& alloc = Allocator() ) : Base( comp, alloc ) 
        { }
        
        template< typename InputIterator >
        ptr_set( InputIterator first, InputIterator last, const Compare& comp = Compare(), 
                 const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc )
        {
            set_basic_clone_and_insert( first, last );
        }

        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_set );
        
        using Base::insert;                                                    
        
        std::pair<iterator,bool> insert( const Key& x ) // strong  
        {  
            return this->insert( make_clone ( x ) );
        }                                                   

        std::pair<iterator,bool> insert( Key* x ) // strong                      
        {       
            if( x == 0 )
                throw bad_pointer( "Null pointer in 'set::insert()'" );
            
            std::auto_ptr<Key> ptr( x );                                
            std::pair<ptr_iterator,bool> res = this->c__().insert( x );       
            if( res.second )                                                 
                ptr.release();                                                  
            return std::make_pair( iterator( res.first ), res.second );     
        }

        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last ) // basic
        {
            set_basic_clone_and_insert( first, last );
        }
        
        BOOST_PTR_SET_ALGORITHMS( Key );      
    };
        
        
        
    template< typename Key, typename Compare = ptr_less<Key>, 
              typename Allocator = std::allocator<Key*> >
    class ptr_unordered_multiset : public detail::associative_ptr_container< detail::default_config< std::multiset<Key*,Compare,Allocator>, Key > >
    {
        typedef detail::associative_ptr_container< detail::default_config< std::multiset<Key*,Compare,Allocator>, Key > > Base;
        
    public: // typedefs
        
        typedef BOOST_DEDUCED_TYPENAME Base::iterator     iterator;              
        typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator;  
        typedef BOOST_DEDUCED_TYPENAME Base::size_type    size_type;         
         
    private:
        template< typename II >                                               
        void set_basic_clone_and_insert( II first, II last ) // basic                 
        {                                                                     
            while( first != last )                                            
            {           
                insert( make_clone( *first ) ); // strong, commit                              
                ++first;                                                      
            }                                                                 
        }                         

    public:
        explicit ptr_multiset( const Compare& comp = Compare(), 
                               const Allocator& alloc = Allocator() )
        : Base( comp, alloc ) 
        { }
        
        template< typename InputIterator >
        ptr_multiset( InputIterator first, InputIterator last,
                      const Compare& comp = Compare(), 
                      const Allocator& alloc = Allocator() ) 
        : Base( comp, alloc ) 
        {
            set_basic_clone_and_insert( first, last );
        }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_multiset );
                                                                            
        using Base::insert;                                                     
        
        iterator insert( const Key& x ) // strong                                
        {                                                                       
            return this->insert( make_clone ( x ) );                            
        }                                                                       
        
        iterator insert( Key* x ) // strong                                      
        {   
            if( x == 0 )
                throw bad_pointer( "Null pointer in 'multiset::insert()'" );

            std::auto_ptr<Key> ptr( x );                                
            ptr_iterator res = this->c__().insert( x );                         
            ptr.release();                                                      
            return iterator( res );                                             
        }
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last ) // basic
        {
            set_basic_clone_and_insert( first, last );
        }
        
        BOOST_PTR_SET_ALGORITHMS( Key )      
    };

    //////////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename K, typename C, typename A >
    ptr_set<K,C,A>* make_clone( const ptr_set<K,C,A>& r )
    {
        return r.clone().release();
    }

    template< typename K, typename C, typename A >
    ptr_multiset<K,C,A>* make_clone( const ptr_multiset<K,C,A>& r )
    {
        r.clone().release();
    }

}

#endif
