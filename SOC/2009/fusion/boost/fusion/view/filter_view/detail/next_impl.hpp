/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct next_impl;

    template<>
    struct next_impl<filter_view_iterator_tag>
    {
        template<typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef
                filter_view_iterator<
                    typename it::category
                  , typename result_of::next<typename it::begin_type>::type
                  , typename it::end_type
                  , typename it::pred_type
                  , typename it::pred_is_metafunction
                >
            type;

            static type
            call(It it)
            {
                return type(fusion::next(it.first),0);
            }
        };
    };
}}}

#endif
