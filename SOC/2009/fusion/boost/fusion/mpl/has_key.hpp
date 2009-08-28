/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_HAS_KEY_HPP
#define BOOST_FUSION_MPL_HAS_KEY_HPP

#include <boost/fusion/sequence/intrinsic/has_key.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct has_key_impl;

    template <>
    struct has_key_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq, typename Key>
        struct apply
          : fusion::result_of::has_key<Seq, Key>
        {};
    };
}}

#endif
