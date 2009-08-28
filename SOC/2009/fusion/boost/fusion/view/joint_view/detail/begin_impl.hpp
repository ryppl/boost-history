/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/equal_to.hpp>

#include <boost/mpl/if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct begin_impl;

    template <>
    struct begin_impl<joint_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;
            typedef typename
                result_of::begin<typename seq::seq1_type>::type
            begin_type;
            typedef typename
                result_of::end<typename seq::seq1_type>::type
            end_type;
            typedef typename
                result_of::begin<typename seq::seq2_type>::type
            concat_type;
            typedef typename
                result_of::equal_to<begin_type, end_type>::type
            equal_to;

            typedef typename
                mpl::if_<
                    equal_to
                  , concat_iterator<concat_type>
                  , joint_view_iterator<begin_type, end_type, concat_type>
                >::type
            type;

            static type
            call(Seq seq, mpl::true_)
            {
                return type(fusion::begin(seq.seq2.get()));
            }

            static type
            call(Seq seq, mpl::false_)
            {
                return type(
                        fusion::begin(seq.seq1.get())
                      , fusion::begin(seq.seq2.get()));
            }

            static type
            call(Seq seq)
            {
                return call(seq, equal_to());
            }
        };
    };
}}}

#endif
