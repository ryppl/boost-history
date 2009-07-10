/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_SIZE_IMPL_HPP
#define BOOST_FUSION_VIEW_FILTER_VIEW_DETAIL_SIZE_IMPL_HPP

#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct size_impl;

    template <>
    struct size_impl<filter_view_tag>
    {
        template <typename SeqRef>
        struct apply
          : result_of::distance<
                typename result_of::begin<SeqRef>::type
              , typename result_of::end<SeqRef>::type
            >
        {};
    };
}}}

#endif
