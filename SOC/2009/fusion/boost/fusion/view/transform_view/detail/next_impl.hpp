/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct next_impl;

    // Unary Version
    template <>
    struct next_impl<transform_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef
                transform_view_iterator<
                    typename result_of::next<
                        typename it::it_type
                    >::type
                  , typename it::transform_type
                >
            type;

            static type
            call(It it)
            {
                return type(fusion::next(it.it), *it.f);
            }
        };
    };

    // Binary Version
    template <>
    struct next_impl<transform_view_iterator2_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef
                transform_view_iterator2<
                    typename result_of::next<
                        typename it::it1_type
                    >::type
                  , typename result_of::next<
                        typename it::it2_type
                    >::type
                  , typename it::transform_type
                >
            type;

            static type
            call(It it)
            {
                return type(fusion::next(it.it1),
                            fusion::next(it.it2),
                            *it.f);
            }
        };
    };
}}}

#endif
