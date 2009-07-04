// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_LIST_LIST_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_LIST_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>

namespace boost { namespace fusion
{
    struct list_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_impl;

        template <>
        struct at_impl<list_tag>
        {
            template <typename SeqRef, typename N>
            struct apply
            {
                typedef typename
                    detail::result_of_forward_as<
                        SeqRef
                      , typename detail::remove_reference<SeqRef>::type::storage_type
                   >::type
                storage_type;

                typedef typename
                    result_of::at<storage_type,N>::type
                type;

                static type
                call(SeqRef seq)
                {
                    return fusion::at<N>(
                            static_cast<storage_type&>(seq.get_data()));
                }
            };
        };
    }
}}

#endif
