/*==============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/mpl/at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template<>
    struct at_impl<nview_tag>
    {
        template<typename Seq, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;
            typedef typename mpl::at<typename seq::indices, N>::type index;

            typedef typename
                result_of::at<typename seq::seq_type,index>::type
            type;

            static type
            call(Seq seq)
            {
                return fusion::at<index>(seq.seq.get());
            }
        };
    };
}}}

#endif
