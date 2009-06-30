/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_ADVANCE_IMPL_HPP

#include <boost/fusion/iterator/advance.hpp>

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
        template<typename Tag>
        struct advance_impl;

        // Unary Version
        template<>
        struct advance_impl<transform_view_iterator_tag>
        {
            template<typename ItRef, typename Dist>
            struct apply
            {
                typedef typename
                    detail::remove_reference<ItRef>::type
                it;

                typedef
                    transform_view_iterator<
                        typename result_of::advance<
                            typename it::first_type
                          , Dist
                        >::type
                      , typename it::transform_type
                    >
                type;

                static type
                call(ItRef i)
                {
                    return type(boost::fusion::advance<Dist>(i.first), i.f);
                }
            };
        };

        // Binary Version
        template<>
        struct advance_impl<transform_view_iterator2_tag>
        {
            template<typename ItRef, typename Dist>
            struct apply
            {
                typedef typename
                    detail::remove_reference<ItRef>::type
                it;

                typedef
                    transform_view_iterator2<
                        typename result_of::advance<
                            typename it::first1_type
                          , Dist
                        >::type
                      , typename result_of::advance<
                            typename it::first2_type
                          , Dist
                        >::type
                      , typename it::transform_type
                    >
                type;

                static type
                call(ItRef i)
                {
                    return type(boost::fusion::advance<Dist>(i.first1)
                              , boost::fusion::advance<Dist>(i.first2)
                              , i.f);
                }
            };
        };
    }
}}

#endif
