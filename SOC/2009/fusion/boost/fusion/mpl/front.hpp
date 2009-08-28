/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_FRONT_HPP
#define BOOST_FUSION_MPL_FRONT_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/value_of.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct front_impl;

    template <>
    struct front_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
          : fusion::result_of::value_of<
                typename fusion::result_of::begin<Seq>::type
            >
        {};
    };
}}

#endif
