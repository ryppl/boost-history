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

#ifndef BOOST_PTR_CONTAINER_PTR_LIST_HPP
#define BOOST_PTR_CONTAINER_PTR_LIST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
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
        
    //public: 
    //    BOOST_FORWARD_TYPEDEF( Base );
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
        
    private:
        class equal
        {
            const T& value_;
            
        public:
            
            equal( const T& v = T() ) : value_( v )
            { }
            
            bool operator()( const T& x ) const
            {
                return x == value_;
            }
            
            bool operator()( const T& l, const T& r ) const
            {
                return l == r;
            }
        };
        
        class less_than
        {
        public:
            bool operator()( const T& l, const T& r ) const
            {
                return l < r;
            }
            
            bool operator()( const T* l, const T* r ) const 
            {
                return *l < *r;
            }
        };
    
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
        
        void remove( const_reference value )                           
        {
            remove_if( equal( value ) ); 
        } 
        
        template< typename Predicate > 
        void remove_if( Predicate pred )                               
        { 
            iterator i = this->begin();
            iterator e = this->end();
            for(; i != e; ++i )
                if( pred( *i ) != false )
                    this->erase( i ); 
        }
        
        void unique()                                                  
        {
            unique( equal() );
        }
        
        template< typename BinaryPredicate >
        void unique( BinaryPredicate binary_pred )                     
        {
            iterator i = this->begin();
            iterator e = prior( this->end() );
            for(; i != e; ++i )
                if( binary_pred( *i, *next( i ) ) )
                    this->erase( i );    
        }
        
        void merge( ptr_list& x )                                 
        {
            merge( x, less_than() );
        }
    
        template< typename Compare > 
        void merge( ptr_list& x, Compare comp )                   
        {
            this->c__().merge( x.c__(), comp );
        }
        
        void sort()                                                    
        { 
            sort( less_than() ); 
        };
        
        template< typename Compare > 
        void sort( Compare comp )                             
        {
            this->c__().sort( comp );
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
        std::auto_ptr<ptr_list<T,A> > p( r.clone() );
        return p.release();
    }

}

#undef BOOST_FORWARD_TYPEDEF
#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

#endif
