/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_AT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template<>
    struct at_impl<single_view_tag>
    {
        template<typename Seq, typename>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    Seq
                  , typename detail::remove_reference<Seq>::type::value_type
                >::type
            type;

            static type
            call(Seq seq)
            {
                return seq.val;
            }
        };
    };
}}}

#endif
