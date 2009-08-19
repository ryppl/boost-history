// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<repetitive_view_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef typename detail::remove_reference<SeqRef>::type seq;

            typedef
                repetitive_view_iterator<
                    typename seq::seq_type
                  , typename result_of::begin<seq>::type
                  , 0
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(fusion::begin(seq.seq.get()));
            }
        };
    };
}}}

#endif
