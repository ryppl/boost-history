/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_PUSH_FRONT_HPP
#define BOOST_FUSION_MPL_PUSH_FRONT_HPP

#include <boost/fusion/algorithm/transformation/push_front.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct push_front_impl;

    template <>
    struct push_front_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq, typename T>
        struct apply
          : fusion::result_of::push_front<Seq, T>
        {};
    };
}}

#endif
