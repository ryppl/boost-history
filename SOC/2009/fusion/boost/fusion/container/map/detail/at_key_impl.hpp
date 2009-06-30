/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_DETAIL_AT_KEY_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_DETAIL_AT_KEY_IMPL_HPP

namespace boost { namespace fusion
{
    struct map_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_key_impl;

        template <>
        struct at_key_impl<map_tag>
        {
            template <typename Sequence, typename Key>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Sequence>::type::
                        template meta_at_impl<Key>::type
                element;

                typedef typename
                    detail::result_of_forward_as<Sequence,element>::type
                type;

                static type
                call(Sequence s)
                {
                    return s.at_impl(mpl::identity<Key>());
                }
            };
        };
    }
}}

#endif
