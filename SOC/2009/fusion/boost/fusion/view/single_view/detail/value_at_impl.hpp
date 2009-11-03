/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_VALUE_AT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct value_at_impl;

    template<>
    struct value_at_impl<single_view_tag>
    {
        template<typename Seq, typename>
        struct apply
        {
            typedef typename
                detail::remove_reference<Seq>::type::value_type
            type;
        };
    };
}}}

#endif
