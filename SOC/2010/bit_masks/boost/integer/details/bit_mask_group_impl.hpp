//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_IMPL_HPP

/** This is used for creating an extention into the boost fusion library for 
 *  the bit_mask_group.
 */
namespace boost {
namespace details {
/** Sequence tag.
 *  This is used for the boost.fusion extention.
 *  Used for dispatching to function overloads.
 *  The only thing needed for this is the declaration but just in case I have
 *  provided an empty definition.
 */
struct bit_mask_group_tag { };

}} // end boost::details



/** Next step in sequence extention.
 *  Adding dispatch look up mechanism.
 */
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/include/tag_of_fwd.hpp>

namespace boost { namespace fusion { namespace traits {

    template<>
    template <  typename Mask0, typename Mask1, typename Mask2, typename Mask3,
                typename Mask4, typename Mask5, typename Mask6, typename Mask7,
                typename Mask8,typename Mask9 >
    struct tag_of< boost::bit_mask_group<Mask0, Mask1, Mask2, Mask3, Mask4,
                           Mask5, Mask6, Mask7, Mask8, Mask9 > >
    {
        typedef example::bit_mask_group_tag type;
    };
}}} // end boost::fusion::traits




#endif
