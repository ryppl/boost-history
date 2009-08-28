/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEREF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_DEREF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<struct_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename
                detail::identity<typename it::seq_type>::type
            identity_struct;

            typedef typename
                detail::forward_as<
                    typename it::seq_type
                  , typename extension::struct_member<
                        identity_struct
                      , it::index::value
                    >::type
                >::type
            type;

            static type
            call(It it)
            {
                return
                    extension::struct_member<
                        identity_struct
                      , it::index::value
                    >::call(*it.seq);
            }
        };
    };

    template <>
    struct deref_impl<assoc_struct_iterator_tag>
       : deref_impl<struct_iterator_tag>
    {};
}}}

#endif
