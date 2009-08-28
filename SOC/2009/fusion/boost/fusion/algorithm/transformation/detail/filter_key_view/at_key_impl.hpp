/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_AT_KEY_IMPL_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_AT_KEY_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at_key.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct at_key_impl;

    template <>
    struct at_key_impl<detail::filter_key_view_tag>
    {
        template <typename Seq,typename Key>
        struct apply
        {
            typedef typename
                result_of::at_key<
                    typename detail::forward_as<
                        Seq
                      , typename detail::remove_reference<Seq>::type_seq_type
                    >::type
                  , Key
                >::type
            type;

            static type
            call(Seq seq)
            {
                return at_key<Key>(seq.seq.get());
            }
        };
    };
}}}

#endif
