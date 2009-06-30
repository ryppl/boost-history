/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/view/transform_view/transform_view_fwd.hpp>

namespace boost { namespace fusion
{
    template <typename First, typename F>
    struct transform_view_iterator;

    template <typename First1, typename First2, typename F>
    struct transform_view_iterator2;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        // Unary Version
        template <>
        struct begin_impl<transform_view_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef typename
                    detail::remove_reference<SeqRef>::type
                seq;

                typedef
                    transform_view_iterator<
                        typename result_of::begin<typename seq::seq_type>::type
                      , typename detail::result_of_forward_as<
                            SeqRef
                          , typename seq::transform_type
                        >::type
                    >
                type;

                static type
                call(SeqRef s)
                {
                    return type(fusion::begin(s.seq), s.f);
                }
            };
        };

        // Binary Version
        template <>
        struct begin_impl<transform_view2_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef typename detail::remove_reference<SeqRef>::type seq;

                typedef
                    transform_view_iterator2<
                        typename result_of::begin<typename seq::seq1_type>::type
                      , typename result_of::begin<typename seq::seq2_type>::type
                      , typename detail::result_of_forward_as<
                            SeqRef
                          , typename seq::transform_type
                        >::type
                    >
                type;

                static type
                call(SeqRef s)
                {
                    return type(
                            fusion::begin(s.seq1),
                            fusion::begin(s.seq2),
                            s.f);
                }
            };
        };
    }
}}

#endif
