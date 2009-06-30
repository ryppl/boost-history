/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>
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
        struct end_impl;

        // Unary Version
        template <>
        struct end_impl<transform_view_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef typename detail::remove_reference<SeqRef>::type seq;

                typedef
                    transform_view_iterator<
                        typename result_of::end<typename seq::seq_type>::type
                      , typename detail::result_of_forward_as<
                            SeqRef
                          , typename seq::transform_type
                        >::type
                    >
                type;

                static type
                call(SeqRef s)
                {
                    return type(fusion::end(s.seq), s.f);
                }
            };
        };

        // Binary Version
        template <>
        struct end_impl<transform_view2_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef typename detail::remove_reference<SeqRef>::type seq;

                typedef
                    transform_view_iterator2<
                        typename result_of::end<typename seq::seq1_type>::type
                      , typename result_of::end<typename seq::seq2_type>::type
                      , typename detail::result_of_forward_as<
                            SeqRef
                          , typename seq::transform_type
                        >::type
                    >
                type;

                static type
                call(SeqRef s)
                {
                    return type(fusion::end(s.seq1), fusion::end(s.seq2), s.f);
                }
            };
        };
    }
}}

#endif
