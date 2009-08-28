/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct begin_impl;

    template <>
    struct begin_impl<filter_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef
                filter_iterator<
                    typename result_of::begin<typename seq::seq_type>::type
                  , typename result_of::end<typename seq::seq_type>::type
                  , typename seq::pred_type
                >
            type;

            static type
            call(Seq seq)
            {
                return type(fusion::begin(seq.seq.get()),0);
            }
        };
    };
}}}

#endif
