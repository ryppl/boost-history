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

#ifndef BOOST_PTR_CONTAINER_PTR_VECTOR_HPP
#define BOOST_PTR_CONTAINER_PTR_VECTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <vector>
#include <boost/ptr_container/detail/reversible_ptr_container.hpp>

namespace boost
{

template< typename T, typename Allocator = std::allocator<T*> >
class ptr_vector : public detail::reversible_ptr_container< detail::default_config< std::vector<T*,Allocator>, T > >
{
    typedef detail::reversible_ptr_container< detail::default_config< std::vector<T*,Allocator>, T > > Base;

public: 
    //BOOST_FORWARD_TYPEDEF( Base );
    typedef typename Base::size_type size_type;

public:
    explicit ptr_vector( const Allocator& alloc = Allocator() )               
    : Base( alloc ) {}
    
    ptr_vector( size_type n, const T& x, 
                const Allocator& alloc = Allocator() )  
    : Base( n, x, alloc ) {}
    
    ptr_vector( std::auto_ptr<ptr_vector> r )                                          
    : Base( r ) {}
    
    template< typename InputIterator >
    ptr_vector( InputIterator first, InputIterator last, 
                const Allocator& alloc = Allocator() ) 
    : Base( first, last, alloc ) {}
    
    void operator=( std::auto_ptr<ptr_vector> r )                                   
    {
        Base::operator=( r );
    }
    
    BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_vector );
    
public: // vector interface
    
    size_type capacity() const
    {
        return this->c__().capacity();
    }
    
    void reserve( size_type n )
    {
        this->c__().reserve( n ); 
    }

};

#undef BOOST_FORWARD_TYPEDEF
#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

}

#endif
