/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>

#include <boost/mpl/apply.hpp>

#include <boost/fusion/view/transform_view/detail/apply_transform_result.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct at_impl;

    template<>
    struct at_impl<transform_view_tag>
    {
        template<typename SeqRef, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<SeqRef>::type seq;

            typedef typename
                mpl::apply<
                    detail::apply_transform_result<
                        typename seq::transform_type
                    >
                  , typename result_of::at<
                        typename seq::seq_type
                      , N
                    >::type
                >::type
            type;

            static type
            call(SeqRef seq)
            {
                return seq.f(boost::fusion::at<N>(seq.seq.get()));
            }
        };
    };

    template<>
    struct at_impl<transform_view2_tag>
    {
        template<typename SeqRef, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<SeqRef>::type seq;

            typedef typename
                mpl::apply<
                    detail::apply_transform_result<
                        typename seq::transform_type
                    >
                  , typename boost::fusion::result_of::at<
                        typename seq::seq1_type
                      , N
                    >::type
                  , typename boost::fusion::result_of::at<
                        typename seq::seq2_type
                      , N
                    >::type
                >::type
            type;

            static type call(SeqRef seq)
            {
                return seq.f(
                        boost::fusion::at<N>(seq.seq1.get()),
                        boost::fusion::at<N>(seq.seq2.get()));
            }
        };
    };
}}}

#endif
