/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_EMPTY_HPP
#define BOOST_FUSION_MPL_EMPTY_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct empty_impl;

    template <>
    struct empty_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq>
        struct apply
          : fusion::result_of::empty<Seq>
        {};
    };
}}

#endif
