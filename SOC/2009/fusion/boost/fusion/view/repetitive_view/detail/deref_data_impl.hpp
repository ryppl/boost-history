// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_DEREF_DATA_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_DEREF_DATA_IMPL_HPP

#include <boost/fusion/iterator/deref_data.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct deref_data_impl;

    template <>
    struct deref_data_impl<repetitive_view_iterator_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename
                result_of::deref_data<
                    typename detail::remove_reference<ItRef>::type::it_type
                >::type
            type;

            static type
            call(ItRef it)
            {
                return deref_data(it.first);
            }
        };
    };
}}}

#endif
