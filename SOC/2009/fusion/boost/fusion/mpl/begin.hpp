/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_BEGIN_HPP
#define BOOST_FUSION_MPL_BEGIN_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/adapted/mpl/detail/begin_impl.hpp>
#include <boost/fusion/mpl/detail/fusion_iterator.hpp>

#include <boost/mpl/begin.hpp>

namespace boost { namespace mpl
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<fusion::fusion_sequence_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                fusion_iterator<
                    typename fusion::result_of::begin<SeqRef>::type
                >
            type;
        };
    };
}}

#endif
