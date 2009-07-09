/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_DETAIL_AT_HPP
#define BOOST_FUSION_MPL_DETAIL_AT_HPP

#include <boost/mpl/at.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>

namespace boost {
    namespace fusion
    {
        struct fusion_sequence_tag;
    }

    namespace mpl
    {
        template <typename Tag>
        struct at_impl;

        template <>
        struct at_impl<fusion::fusion_sequence_tag>
        {
            template <typename SeqRef, typename N>
            struct apply
              : fusion::result_of::value_at<SeqRef, N>
            {};
        };
    }
}

#endif
