// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_DISTANCE_IMPL_HPP

#include <boost/mpl/minus.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct distance_impl;

    template<>
    struct distance_impl<repetitive_view_iterator_tag>
    {
        template<typename BeginRef, typename EndRef>
        struct apply
        {
            typedef
                mpl::minus<
                    typename detail::remove_reference<
                        BeginRef
                    >::type::index
                  , typename detail::remove_reference<
                        EndRef
                    >::type::index
                >
            type;
        };
    };
}}}

#endif
