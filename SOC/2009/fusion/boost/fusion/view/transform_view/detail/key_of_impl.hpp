/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_KEY_OF_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_KEY_OF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct key_of_impl;

    template <>
    struct key_of_impl<transform_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename
                value_of_impl<transform_view_iterator_tag>::
                    template apply<It>::type::first_type
            type;
        };
    };

    template <>
    struct key_of_impl<transform_view_iterator2_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename
                value_of_impl<transform_view_iterator2_tag>::
                    template apply<It>::first_type
            type;
        };
    };
}}}

#endif
