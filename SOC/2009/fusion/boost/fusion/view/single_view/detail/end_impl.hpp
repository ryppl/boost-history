/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_END_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct end_impl;

    template <>
    struct end_impl<single_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef
                single_view_iterator<
                    typename seq::value_type
                  , typename detail::forward_as<
                        Seq
                      , typename seq::value_type
                    >::type
                  , true
                >
            type;

            static type
            call(Seq seq)
            {
                return type(seq.val);
            }
        };
    };
}}}

#endif
