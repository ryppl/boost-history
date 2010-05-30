//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASK_GROUP_HPP
#define BOOST_BIT_MASK_GROUP_HPP


#include <boost/integer/compound_mask.hpp>
// #include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>




namespace boost {

// TODO: move this into a sperate file
namespace details {
struct null_mask { };
typedef null_mask unused_parameter;

template <typename T, typename TypeVector>
struct add_to_vector{
    template <typename NewT, typename NewTypeVector>
    struct rebind {
    };
};
}
template <  typename Mask0,
            typename Mask1  = details::unused_parameter,
            typename Mask2  = details::unused_parameter,
            typename Mask3  = details::unused_parameter,
            typename Mask4  = details::unused_parameter,
            typename Mask5  = details::unused_parameter,
            typename Mask6  = details::unused_parameter,
            typename Mask7  = details::unused_parameter,
            typename Mask8  = details::unused_parameter,
            typename Mask9  = details::unused_parameter
        >
struct bit_mask_group
    // if they are the same then do nothing? if they arn't the same 
    // that means that I located a mask or something to be part of
    // the mask group.
{
    // typedef typename mpl::if_<
    //    is_same<Mask1,typename details::unused_parameter>,
    //    Mask1,
};


} // namespace

#endif
