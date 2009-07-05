/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_DETAIL_VALUE_AT_KEY_IMPL_HPP
#define BOOST_FUSION_CONTAINER_SET_DETAIL_VALUE_AT_KEY_IMPL_HPP

namespace boost { namespace fusion
{
    struct set_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_at_key_impl;

        template <>
        struct value_at_key_impl<set_tag>
        {
            template <typename SeqRef, typename Key>
            struct apply
            {
                typedef typename
                    detail::remove_reference<SeqRef>::type::
                        template meta_at_impl<Key>::type
                type;
            };
        };
    }
}}

#endif
