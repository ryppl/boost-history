/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2005-2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_KEY_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_KEY_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct at_key_impl;

    template <>
    struct at_key_impl<struct_tag>
    {
        template <typename SeqRef, typename Key>
        struct apply
        {
            typedef typename detail::identity<SeqRef>::type seq;

            typedef typename
                detail::forward_as<
                    SeqRef
                  , typename struct_assoc_member<seq, Key>::type
                >::type
            type;

            static type
            call(SeqRef seq_)
            {
                return struct_assoc_member<seq, Key>::call(seq_);
            }
        };
    };
}}}

#endif
