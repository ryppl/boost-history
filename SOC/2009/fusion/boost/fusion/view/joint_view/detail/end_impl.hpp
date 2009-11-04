/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>

#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct end_impl;

    template <>
    struct end_impl<joint_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef
                concat_iterator<
                    typename seq::category
                  , typename result_of::end<
                        typename detail::forward_as<
                            Seq
                          , typename seq::seq2_type
                        >::type
                    >::type
                >
            type;

            static type
            call(Seq seq)
            {
                return type(fusion::end(seq.seq2.get()),0);
            }
        };
    };
}}}

#endif
