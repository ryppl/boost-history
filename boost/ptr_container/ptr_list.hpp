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

#ifndef BOOST_PTR_CONTAINER_PTR_LIST_HPP
#define BOOST_PTR_CONTAINER_PTR_LIST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/ptr_container/detail/reversible_ptr_container.hpp>
#include <boost/utility.hpp>
#include <list>

namespace boost
{

    template< typename T, typename Allocator = std::allocator<T*> >
    class ptr_list : public detail::reversible_ptr_container< detail::default_config< std::list<T*,Allocator>, T > >
    {
        typedef detail::reversible_ptr_container< detail::default_config< std::list<T*,Allocator>, T > > Base;
        
    public:
        typedef BOOST_DEDUCED_TYPENAME Base::iterator        iterator;
        typedef BOOST_DEDUCED_TYPENAME Base::size_type       size_type;
        typedef BOOST_DEDUCED_TYPENAME Base::const_reference const_reference;
        
    
    public:
        explicit ptr_list( const Allocator& alloc = Allocator() ) 
        : Base( alloc ) { }
        
        ptr_list( size_type n, const_reference x, const Allocator& alloc = Allocator() )
        : Base( n, x, alloc ) { }
       
        template< typename InputIterator >
        ptr_list( InputIterator first, InputIterator last )      
        : Base( first, last ) { }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_list );
          
    public: 
        void splice( iterator before, ptr_list& x )                    
        {
            this->c__().splice( before.base(), x.c__() );
        }
        
        void splice( iterator before, ptr_list& x, iterator i ) 
        {
            this->c__().splice( before.base(), x.c__(), i.base() );
        }
        
        void splice( iterator before, ptr_list& x, iterator first, iterator last )
        {
            this->c__().splice( before.base(), x.c__(), first.base(), last.base() );
        }
        
        void merge( ptr_list& x )                                 
        {
            merge( x, ptr_container::detail::less_than<T>() );
        }
    
        template< typename Compare > 
        void merge( ptr_list& x, Compare comp )                   
        {
            this->c__().merge( x.c__(), comp );
        }
        
        void sort()                                                    
        { 
            sort( std::less<T>() ); 
        };
        
        template< typename Compare > 
        void sort( Compare comp )                             
        {
            this->c__().sort( indirected2<Compare>( comp ) );
        }
        
        void reverse()
        {
            this->c__().reverse(); 
        }
    
    }; // class 'ptr_list'

    //////////////////////////////////////////////////////////////////////////////
    // clonability

    template< typename T, typename A >
    ptr_list<T,A>* make_clone( const ptr_list<T,A>& r )
    {
        return r.clone().release();
    }

}

//#undef BOOST_FORWARD_TYPEDEF
//#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

#endif
