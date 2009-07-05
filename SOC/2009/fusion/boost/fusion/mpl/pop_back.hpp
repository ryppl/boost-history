/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_POP_BACK_HPP
#define BOOST_FUSION_MPL_POP_BACK_HPP

#include <boost/mpl/pop_back.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/algorithm/transformation/pop_back.hpp>
#include <boost/fusion/sequence/convert.hpp>

namespace boost { namespace mpl
{
    template <typename Tag>
    struct pop_back_impl;

    template <>
    struct pop_back_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef typename
                fusion::result_of::pop_back<Seq>::type
            result;

            typedef typename
                fusion::result_of::convert<
                    typename fusion::traits::tag_of<Seq>::type, result
                >::type
            type;
        };
    };
}}

#endif
