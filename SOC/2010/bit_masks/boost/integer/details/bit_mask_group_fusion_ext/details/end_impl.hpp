//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_FUSION_EXT_END_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_FUSION_EXT_END_IMPL_HPP

namespace boost { namespace fusion {

    struct bit_mask_group_tag;

    namespace extension {

    template <typename> struct end_impl;

    template <>
    struct end_impl< bit_mask_group_tag > {
        template <typename MaskGroup>
        struct apply {
            typedef bit_mask_group_iterator<
                MaskGroup,
                mpl::size<
                    typename MaskGroup::mask_vector
                 >::value
            > type;

            static type call(MaskGroup&) {
                return type();
            }
        };
    };

}}} // end boost::fusion::extension

#endif
