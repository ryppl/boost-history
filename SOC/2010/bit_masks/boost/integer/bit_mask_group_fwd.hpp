//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_FWD_HPP
#define BOOST_BIT_MASK_GROUP_FWD_HPP

/** Forward declaration of template bit_mask_group and the class used for
 *  associating a name/tag with a particular type within the bit_mask_group.
 */
namespace boost {

template <typename T, typename NameType> struct tagged;

template <  typename Mask0, typename Mask1, typename Mask2, typename Mask3,
            typename Mask4, typename Mask5, typename Mask6, typename Mask7,
            typename Mask8, typename Mask9
        >
struct bit_mask_group;

}

#endif
