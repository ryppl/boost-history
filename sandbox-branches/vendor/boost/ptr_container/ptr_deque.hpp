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

#ifndef BOOST_PTR_CONTAINER_PTR_DEQUE_HPP
#define BOOST_PTR_CONTAINER_PTR_DEQUE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <deque>
#include <boost/ptr_container/detail/reversible_ptr_container.hpp>

namespace boost
{
    
    template< typename T, typename Allocator = std::allocator<T*> >
    class ptr_deque : public detail::reversible_ptr_container< detail::default_config< std::deque<T*,Allocator>, T > >
    {
        typedef detail::reversible_ptr_container< detail::default_config< std::deque<T*,Allocator>, T > > Base;
        
    public:
        typedef BOOST_DEDUCED_TYPENAME Base::size_type        size_type;
        typedef BOOST_DEDUCED_TYPENAME Base::const_reference  const_reference;
        
    public: 
    
        explicit ptr_deque( const Allocator& alloc = Allocator() ) 
        : Base( alloc ) { }
        
        ptr_deque( size_type n, const_reference x, const Allocator& alloc = Allocator() )  
        : Base( n, x, alloc ) { }
        
        template< typename InputIterator >
        ptr_deque( InputIterator first, InputIterator last )
        : Base( first, last ) { }
        
        BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( ptr_deque );
    };

    //////////////////////////////////////////////////////////////////////////////
    // clonability
    
    template< typename T, typename A >
    ptr_deque<T,A>* make_clone( const ptr_deque<T,A>& r )
    {
        return r.clone().release();
    }

}

//#undef BOOST_FORWARD_TYPEDEF
//#undef BOOST_PTR_CONTAINER_RELEASE_AND_CLONE

#endif
