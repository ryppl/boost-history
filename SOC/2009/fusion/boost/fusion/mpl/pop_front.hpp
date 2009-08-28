/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_POP_FRONT_HPP
#define BOOST_FUSION_MPL_POP_FRONT_HPP

#include <boost/fusion/sequence/convert.hpp>
#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct pop_front_impl;

    template <>
    struct pop_front_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
          : fusion::result_of::convert<
                typename fusion::traits::tag_of<Seq>::type
              , typename fusion::result_of::pop_front<Seq>::type
            >
        {};
    };
}}

#endif
