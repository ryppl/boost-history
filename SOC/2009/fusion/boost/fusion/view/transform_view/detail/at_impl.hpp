/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/support/internal/result_of.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template<>
    struct at_impl<transform_view_tag>
    {
        template<typename Seq, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef typename
                boost::result_of<
                    typename detail::get_func_base<
                        typename seq::transform_type
                    >::type(
                    typename result_of::at<typename seq::seq_type, N>::type
                    )
                >::type
            type;

            static type
            call(Seq seq)
            {
                return seq.f(fusion::at<N>(seq.seq.get()));
            }
        };
    };

    template<>
    struct at_impl<transform_view2_tag>
    {
        template<typename Seq, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef typename
                boost::result_of<
                    typename detail::get_func_base<
                        typename seq::transform_type
                    >::type(
                   typename fusion::result_of::at<
                       typename seq::seq1_type
                      , N
                    >::type
                  , typename fusion::result_of::at<
                        typename seq::seq2_type
                      , N
                    >::type)
                >::type
            type;

            static type call(Seq seq)
            {
                return seq.f(
                        fusion::at<N>(seq.seq1.get()),
                        fusion::at<N>(seq.seq2.get()));
            }
        };
    };
}}}

#endif
