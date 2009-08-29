/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/algorithm/query/detail/find_if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct next_impl;

    template <>
    struct next_impl<filter_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef
                detail::static_find_if<
                    typename result_of::next<typename it::begin_type>::type
                  , typename it::end_type
                  , mpl::bind1<
                        typename mpl::lambda<typename it::pred_type>::type
                      , mpl::bind1<mpl::quote1<result_of::value_of>,mpl::_1>
                    >
                >
            filter;

            typedef
                filter_iterator<
                    typename filter::type
                  , typename it::end_type
                  , typename it::pred_type
                >
            type;

            static type
            call(It it)
            {
                return type(filter::call(it.first),0);
            }
        };
    };
}}}

#endif
