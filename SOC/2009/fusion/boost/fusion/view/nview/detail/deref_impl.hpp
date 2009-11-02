/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_DEREF_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/mpl/deref.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct deref_impl;

    template<>
    struct deref_impl<nview_iterator_tag>
    {
        template<typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename mpl::deref<typename it::pos_type>::type index;

            typedef typename
                result_of::at<typename it::seq_type, index>::type
            type;

            static type
            call(It it)
            {
                return fusion::at<index>(it.seq);
            }
        };
    };
}}}

#endif

