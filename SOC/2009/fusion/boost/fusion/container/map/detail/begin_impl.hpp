// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_MAP_BEGIN_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>

namespace boost { namespace fusion
{
    struct map_tag;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<map_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef typename
                    detail::result_of_forward_as<Sequence
                      , typename detail::remove_reference<Sequence>::type::storage_type
                    >::type
                storage_type;

                typedef typename
                    result_of::begin<storage_type>::type
                type;

                static type
                call(Sequence s)
                {
                    return fusion::begin(
                            static_cast<storage_type>(s.get_data()));
                }
            };
        };
    }
}}

#endif
