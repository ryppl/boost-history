//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_FUSION_EXT_IS_VIEW_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_FUSION_EXT_IS_VIEW_IMPL_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion {

    struct bit_mask_group;

    namespace extension {

    template<typename Tag>
    struct is_view_impl;

    template <>
    struct is_view_impl< bit_mask_group_tag > {
        template <typename>
        struct apply : mpl::false_ { };
    };

}}} // end boost::fusion::extension

#endif
