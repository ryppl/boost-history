//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_MEMBER_IMPL_HPP
#define BOOST_BITFIELD_MEMBER_IMPL_HPP
#include "bitfield_vector_base.hpp"


namespace boost { namespace detail {

/** bitfield_vector_member_impl
 *  This implements the members basic member functions so that they can be
 *  tested outside of the bitfield_vector it self and the used to implement
 *  the rest of the functionalility of the bitfield_vector with out as much
 *  trouble.
 *  
 *  
 *  This level of the data structures deals with constructing the object 
 *  correctly and the implementing a lot of details related to the
 *  bitfeild_vector which would other wise be private.
 *  
 */
template <typename T, typename Alloc>
struct bitfield_vector_member_impl
    :bitfield_vector_base<T,Alloc>
{
    
    /** Base class of the bitfield_vector_impl. */
    typedef bitfield_vector_base<T,Alloc> _base;

    /** Default Constructor. */
    bitfield_vector_member_impl()
        :_base()
    { }

    /** Constructor over an allocator. */
    bitfield_vector_member_impl(typename _base::allocator const& alloc)
        :_base(alloc)
    { }

    /** Array Constructor. */
    bitfield_vector_member_impl(std::size_t n)
        :_base(n)
    { }

    /** Array + Allocator Constructor. */
    bitfield_vector_member_impl(std::size_t n,
        typename _base::allocator const& alloc)
        :_base(n,alloc)
    { }

};

}} // end boost::detail


#endif
