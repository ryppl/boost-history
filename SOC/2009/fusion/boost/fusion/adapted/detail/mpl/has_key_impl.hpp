/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_HAS_KEY_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_HAS_KEY_IMPL_HPP

#include <boost/mpl/has_key.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct has_key_impl;

    template <>
    struct has_key_impl<mpl_sequence_tag>
    {
        template <typename Seq, typename Key>
        struct apply
          : mpl::has_key<typename detail::identity<Seq>::type, Key>
        {};
    };
}}}

#endif
