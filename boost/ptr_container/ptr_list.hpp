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

#include <boost/ptr_container/detail/reversible_ptr_container>
#include <boost/utility.hpp>
#include <list>

namespace boost
{

template< typename T, typename Allocator = std::allocator<T*> >
class ptr_list : public detail::reversible_ptr_container< std::list<T*,Allocator>, T >
{
    typedef detail::reversible_ptr_container< std::list<T*,Allocator>, T > Base;

public:
    ptr_list()                                                 : Base() {}
    ptr_list( auto_ptr<ptr_list> r )                           : Base( r )             {}
    ptr_list( typename Base::release_type r )                  : Base( r )             {}
    template< typename InputIterator >
    ptr_list( InputIterator first, InputIterator last )        : Base( first, last ) {}
    void operator=( std::auto_ptr<ptr_list> r )                { Base::operator=( r );}

    BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_list );
    
private:
    class equal
    {
        const_reference value_;
    public:
        remover( const_reference v ) : value_( v )
        { }
        
        bool operator( const_reference x )
        {
            return x == value_ );
        }
        
        bool operator( const_reference l, const_reference r )
        {
            return l == r;
        }
    };
    
    class less_than
    {
    public:
        bool operator()( const_reference l, const_reference r )
        {
            return l < r;
        }
        
        bool operator( const T* l, const T* r )
        {
            return *l < *r;
        }
    }

public: 
    void  splice( iterator before, ptr_list& x )                    { c_.splice( before.base(), x.c_ );}
    void  splice( iterator before, ptr_list& x, iterator i )        { c_.splice( before.base(), x.c_, i.base() );}
    void  splice( iterator before, ptr_list& x, iterator first, iterator last )
                                                                    { c_.splice( before.base(), x.c_, first.base(), last.base() );}
    void  remove( const_reference value )                           
    {
        remove_if( equal( value ) ); 
    } 
    
    template< typename Predicate > 
    void  remove_if( Predicate pred )                               
    { 
        iterator � = this->begin();
        iterator e = this->end();
        for(; i != e; ++i )
            if( pred( *i ) != false )
                erase( i ); 
    }
    
    void  unique()                                                  
    {
        unique( equal( value ) );
    }
    
    template< typename BinaryPredicate >
    void  unique( BinaryPredicate binary_pred )                     
    {
        iterator � = this->begin();
        iterator e = prior( this->end() );
        for(; i != e; ++i )
            if( binary_pred( *i, *next( i ) )
                erase( i );    
    }
    
    void  merge( ptr_listr& x )                                 
    {
        merge( x, less_than() };
    }

    template< typename Compare > 
    void  merge( ptr_list& x, Compare comp )                   
    {
        c_.merge( x.c_, comp );
    }
    
    void  sort()                                                    
    { 
        sort( less_than() ); 
    };
    
    template< typename Compare > 
    void  sort( Compare comp )                             
    {
        c_.sort( comp );
    }
    
    void  reverse()                                                 { c_.reverse(); }

}; // class 'ptr_list'

}

#undef BOOST_FORWARD_TYPEDEF
#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

#endif
