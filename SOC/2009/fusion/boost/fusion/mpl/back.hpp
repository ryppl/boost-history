/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_BACK_HPP
#define BOOST_FUSION_MPL_BACK_HPP

#include <boost/fusion/iterator/prior.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct back_impl;

    template <>
    struct back_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
          : fusion::result_of::value_of<
                typename fusion::result_of::prior<
                    typename fusion::result_of::end<Seq>::type
                >::type
            >
        {};
    };
}}

#endif
