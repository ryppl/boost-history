// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_DETAIL_ITERATOR_ADAPTER_NEXT_IMPL_HPP
#define BOOST_FUSION_SUPPORT_DETAIL_ITERATOR_ADAPTER_NEXT_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct next_impl;

    template <>
    struct next_impl<iterator_adapter_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename
                detail::remove_reference<ItRef>::type
            it;

            typedef
                iterator_adapter<
                    typename result_of::next<
                        typename it::iterator_type
                    >::type
                  , typename it::category
                >
            type;

            static type
            call(ItRef it)
            {
                return type(fusion::next(it.it),0);
            }
        };
    };
}}}

#endif
