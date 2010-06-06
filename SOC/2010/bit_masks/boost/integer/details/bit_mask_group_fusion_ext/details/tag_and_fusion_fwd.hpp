//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_FUSION_EXT_TAG_AND_FUSION_FWD_HPP
#define BOOST_BIT_MASK_GROUP_FUSION_EXT_TAG_AND_FUSION_FWD_HPP

namespace boost {
/** Forward declaration of the iterator, for the fusion sequence extention
 *  bit_mask_group
 */
template<typename MaskGroup, unsigned int Pos> struct bit_mask_group_iterator;

namespace details {
/** bit_mask_group's Sequence tag.
 *  Used for dispatching to overloads.
 */
struct bit_mask_group_tag;

/** bit_mask_group's iterator tag. */
struct bit_mask_group_iterator_tag;

} // end details


namespace fusion { namespace extension {

/** All Forward decalations for all of the impl funcitons used by the fusion
 *  extension process.
 */
template <typename> struct value_at_impl;
template <typename> struct deref_impl;
template <typename> struct next_impl;
template <typename> struct prior_impl;
template <typename> struct advance_impl;
template <typename> struct distance_impl;
template <typename> struct equal_to_impl;
template <typename> struct category_of_impl;
template <typename> struct is_sequence_impl;
template <typename> struct is_view_impl;
template <typename> struct begin_impl;
template <typename> struct end_impl;
template <typename> struct size_impl;
template <typename> struct value_at_impl;
template <typename> struct at_impl;
template <typename> struct at_key_impl;

/** Forward declaration of all spcilizations. */
// iterator overloads.
template <> struct value_at_impl< boost::details::bit_mask_group_iterator_tag >;
template <> struct deref_impl< boost::details::bit_mask_group_iterator_tag >;
template <> struct next_impl< boost::details::bit_mask_group_iterator_tag >;
template <> struct prior_impl< boost::details::bit_mask_group_iterator_tag >;
template <> struct advance_impl< boost::details::bit_mask_group_iterator_tag >;
template <> struct distance_impl< boost::details::bit_mask_group_iterator_tag >;
template <> struct equal_to_impl< boost::details::bit_mask_group_iterator_tag >;

// sequence overloads.
template <> struct category_of_impl< boost::details::bit_mask_group_tag >;
template <> struct begin_impl< boost::details::bit_mask_group_tag >;
template <> struct end_impl< boost::details::bit_mask_group_tag >;
template <> struct size_impl< boost::details::bit_mask_group_tag >;
template <> struct value_at_impl< boost::details::bit_mask_group_tag >;
template <> struct at_impl< boost::details::bit_mask_group_tag >;
template <> struct at_key_impl< boost::details::bit_mask_group_tag >;

}}} // end boost::fusion::extension

#endif
