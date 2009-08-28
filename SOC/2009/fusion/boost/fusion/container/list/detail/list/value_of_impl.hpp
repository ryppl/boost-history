/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_VALUE_OF_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/value_at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    template <>
    struct value_of_impl<list_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                result_of::value_at<
                    typename detail::remove_reference<
                        typename it::seq_type
                    >::type::storage_type
                  , typename it::index
                >::type
            type;
        };
    };
}}}

#endif
