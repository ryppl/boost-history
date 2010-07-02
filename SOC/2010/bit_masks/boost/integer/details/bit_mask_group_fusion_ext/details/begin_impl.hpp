//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_FUSION_EXT_BEGIN_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_FUSION_EXT_BEGIN_IMPL_HPP
#include "../bit_mask_group_iterator.hpp"

namespace boost { namespace fusion {

    struct bit_mask_group_tag;

    namespace extension {

    template<typename> struct begin_impl;

    template <>
    struct begin_impl< bit_mask_group_tag > {
        template<typename MaskGroup>
        struct apply {
            typedef bit_mask_group_iterator<MaskGroup, 0> type;
        
            static type call(MaskGroup&) {
                return type();
            }
        };
    };

}}} // end boost::fusion::extension

#endif