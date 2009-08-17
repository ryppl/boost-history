// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_CONTAINER_MAP_DETAIL_VALUE_OF_DATA_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_DETAIL_VALUE_OF_DATA_IMPL_HPP

#include <boost/fusion/container/map/detail/value_of_impl.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct value_of_data_impl;

    template <>
    struct value_of_data_impl<map_iterator_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename
                value_of_impl<map_iterator_tag>::
                    template apply<ItRef>::type::second_type
            type;
        };
    };
}}}

#endif
