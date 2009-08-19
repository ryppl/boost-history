// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_VALUE_OF_DATA_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_VALUE_OF_DATA_IMPL_HPP

#include <boost/fusion/iterator/value_of_data.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct value_of_data_impl;

    template <>
    struct value_of_data_impl<reverse_view_iterator_tag>
    {
        template <typename ItRef>
        struct apply
          : result_of::value_of_data<
                typename detail::remove_reference<ItRef>::type::it_type
            >
        {};
    };
}}}

#endif
