/*==============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct begin_impl;

    // Unary Version
    template<>
    struct begin_impl<transform_view_tag>
    {
        template<typename Seq>
        struct apply
        {
            typedef typename
                detail::remove_reference<Seq>::type
            seq;
            typedef typename
                detail::forward_as_lref<
                    Seq
                  , typename seq::transform_type
                >::type
            transform_type;

            typedef
                transform_view_iterator<
                    typename result_of::begin<
                        typename detail::forward_as_lref<
                            Seq
                          , typename seq::seq_type
                        >::type
                    >::type
                  , typename detail::forward_as_lref<
                        Seq
                      , typename seq::transform_type
                    >::type
                  , typename seq::is_associative
                >
            type;

            static type
            call(Seq seq)
            {
                return type(
                    fusion::begin(seq.seq.template get<Seq>()),
                    static_cast<transform_type>(seq.f));
            }
        };
    };

    // Binary Version
    template<>
    struct begin_impl<transform_view2_tag>
    {
        template<typename Seq>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;
            typedef typename
                detail::forward_as_lref<
                    Seq
                  , typename seq::transform_type
                >::type
            transform_type;

            typedef
                transform_view_iterator2<
                    typename result_of::begin<
                        typename detail::forward_as_lref<
                            Seq
                          , typename seq::seq1_type
                        >::type
                    >::type
                  , typename result_of::begin<
                        typename detail::forward_as_lref<
                            Seq
                          , typename seq::seq2_type
                        >::type
                    >::type
                  , transform_type
                  , typename seq::is_associative
                >
            type;

            static type
            call(Seq seq)
            {
                return type(
                    fusion::begin(seq.seq1.template get<Seq>()),
                    fusion::begin(seq.seq2.template get<Seq>()),
                    static_cast<transform_type>(seq.f));
            }
        };
    };
}}}

#endif
