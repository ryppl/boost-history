// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_MAP_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<map_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef typename
                result_of::begin<
                    typename detail::forward_as<
                        SeqRef
                      , typename detail::remove_reference<
                            SeqRef
                        >::type::storage_type
                    >::type
                >::type
            type;

            static type
            call(SeqRef seq)
            {
                return fusion::begin(seq.get_data());
            }
        };
    };
}}}

#endif
