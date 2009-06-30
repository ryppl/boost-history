/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion
{
    struct transform_view_iterator_tag;
    struct transform_view_iterator2_tag;

    template<typename First, typename F>
    struct transform_view_iterator;

    template <typename First1, typename First2, typename F>
    struct transform_view_iterator2;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        // Unary Version
        template <>
        struct next_impl<transform_view_iterator_tag>
        {
            template <typename ItRef>
            struct apply
            {
                typedef typename detail::remove_reference<ItRef>::type it;

                typedef
                    transform_view_iterator<
                        typename result_of::next<
                            typename it::first_type
                        >::type
                      , typename it::transform_type
                    >
                type;

                static type
                call(ItRef i)
                {
                    return type(fusion::next(i.first), i.f);
                }
            };
        };

        // Binary Version
        template <>
        struct next_impl<transform_view_iterator2_tag>
        {
            template <typename ItRef>
            struct apply
            {
                typedef typename detail::remove_reference<ItRef>::type it;

                typedef
                    transform_view_iterator2<
                        typename result_of::next<
                            typename it::first1_type
                        >::type
                      , typename result_of::next<
                            typename it::first2_type
                        >::type
                      , typename it::transform_type
                    >
                type;

                static type
                call(ItRef i)
                {
                    return type(fusion::next(i.first1),
                            fusion::next(i.first2),
                            i.f);
                }
            };
        };
    }
}}

#endif
