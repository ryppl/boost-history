//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASK_GROUP_HPP
#define BOOST_BIT_MASK_GROUP_HPP


#include <boost/integer/compound_mask.hpp>
// #include <boost/mpl/list.hpp>



namespace boost {

template <  typename Mask0,
            typename Mask1  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask2  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask3  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask4  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask5  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask6  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask7  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask8  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask9  = integral_mask< typename Mask0::value_type, 0 >
        >
struct t;


} // namespace

#endif
