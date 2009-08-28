/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_KEY_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_KEY_OF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct key_of_impl;

    template <>
    struct key_of_impl<assoc_struct_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                extension::struct_assoc_key<
                    typename detail::identity<typename it::seq_type>::type
                  , it::index::value
                >::type
            type;
        };
    };
}}}

#endif
