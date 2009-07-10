// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_VALUE_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/value_at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct value_at_impl;

    template <>
    struct value_at_impl<list_tag>
    {
        template <typename SeqRef, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    SeqRef
                  , typename detail::remove_reference<SeqRef>::type::storage_type
               >::type
            storage_type;

            typedef typename
                result_of::value_at<storage_type,N>::type
            type;
        };
    };
}}}

#endif
