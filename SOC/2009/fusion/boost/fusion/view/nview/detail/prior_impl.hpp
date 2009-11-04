/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_PRIOR_IMPL_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_PRIOR_IMPL_HPP

#include <boost/mpl/prior.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct prior_impl;

    template <>
    struct prior_impl<nview_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef
                nview_iterator<
                    typename it::seq_type,
                    typename mpl::prior<typename it::pos_type>::type
                >
            type;

            static type
            call(It it)
            {
                return type(it.seq,0);
            }
        };
    };
}}}

#endif
