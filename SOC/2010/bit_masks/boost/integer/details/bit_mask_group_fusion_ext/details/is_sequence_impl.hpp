//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_FUSION_EXT_IS_SEQUENCE_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_FUSION_EXT_IS_SEQUENCE_IMPL_HPP

namespace boost { namespace fusion {

    struct bit_mask_group_tag;
    
    namespace extension {

    template <typename>
    struct is_sequence_impl;

    template<>
    struct is_sequence_impl< bit_mask_group_tag > {
        template<typename T>
        struct apply : mpl::true_ { };
    };

}}} // end boost::fusion::extension

#endif
