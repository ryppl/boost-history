/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_VALUE_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/result_of.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct value_at_impl;

    template<>
    struct value_at_impl<transform_view_tag>
    {
        template<typename SeqRef, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<SeqRef>::type seq;

            typedef typename
                support::result_of<
                    typename support::get_func_base<
                        typename seq::transform_type
                    >::type(
                    typename boost::fusion::result_of::value_at<
                        typename seq::sequence_type
                      , N
                    >::type)
                >::type
            type;
        };
    };

    template<>
    struct value_at_impl<transform_view2_tag>
    {
        template<typename SeqRef, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<SeqRef>::type seq;

            typedef typename
                support::result_of<
                    typename support::get_func_base<
                        typename seq::transform_type
                    >::type(
                    typename boost::fusion::result_of::value_at<
                        typename seq::seq1_type, N
                    >::type
                  , typename boost::fusion::result_of::value_at<
                        typename seq::seq2_type, N
                    >::type)
                >::type
            type;
        };
    };
}}}

#endif
