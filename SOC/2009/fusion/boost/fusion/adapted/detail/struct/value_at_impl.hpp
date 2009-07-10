/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_VALUE_AT_IMPL_HPP

#include <boost/fusion/support/assert.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct value_at_impl;

    template <>
    struct value_at_impl<struct_tag>
    {
        template <typename SeqRef, typename N>
        struct apply
          : struct_member<typename detail::identity<SeqRef>::type, N::value>
        {
            //BOOST_FUSION_INDEX_CHECK(N::value, struct_size<Sequence>::value);
        };
    };
}}}

#endif
