/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>

namespace boost { namespace fusion { namespace extension
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
                    typename result_of::end<typename seq::sequence_type>::type
                  , typename detail::forward_as<
                        SeqRef
                      , typename seq::transform_type
                    >::type
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(fusion::end(seq.seq.get()), seq.f);
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
                  , typename detail::forward_as<
                        SeqRef
                      , typename seq::transform_type
                    >::type
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(
                        fusion::end(seq.seq1.get())
                      , fusion::end(seq.seq2.get())
                      , seq.f);
            }
        };
    };
}}}

#endif
