//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASK_GROUP_TAG_OF_HPP
#define BOOST_BIT_MASK_GROUP_TAG_OF_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <cstddef>

namespace boost { namespace fusion {

    struct bit_mask_group_tag;
    struct bit_mask_group_iterator_tag;
    
    namespace traits {

    /** bit_mask_group specilization. */
    template <  typename Extra>
    template <  typename Mask0, typename Mask1, typename Mask2, typename Mask3,
                typename Mask4, typename Mask5, typename Mask6, typename Mask7,
                typename Mask8, typename Mask9 >
    struct tag_of< boost::bit_mask_group< Mask0, Mask1, Mask2, Mask3, Mask4,
                                          Mask5, Mask6, Mask7, Mask8, Mask9 >,
                                          Extra >
    {
        typedef bit_mask_group_tag type;
    };


    /** Iterator tag_of specilization. */
    template <typename Extra>
    template <typename MaskGroup, std::size_t Pos>
    struct tag_of< boost::fusion::bit_mask_group_iterator< MaskGroup, Pos >, Extra> {
        typedef bit_mask_group_iterator_tag type;
    };

}}} // end boost::fusion::traits

#endif
