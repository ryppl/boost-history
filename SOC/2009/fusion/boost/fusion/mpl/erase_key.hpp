/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_MPL_ERASE_KEY_HPP
#define BOOST_FUSION_MPL_ERASE_KEY_HPP

#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/algorithm/transformation/erase_key.hpp>
#include <boost/fusion/sequence/convert.hpp>

namespace boost { namespace mpl
{
    template <typename>
    struct erase_key_impl;

    template <>
    struct erase_key_impl<fusion::fusion_sequence_tag>
    {
        template <typename Seq, typename Key>
        struct apply
          : fusion::result_of::convert<
                typename fusion::traits::tag_of<Seq>::type
              , typename fusion::result_of::erase_key<Seq, Key>::type
            >::type
        {};
    };
}}

#endif
