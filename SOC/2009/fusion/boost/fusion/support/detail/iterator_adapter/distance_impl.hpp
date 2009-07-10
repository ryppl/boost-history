// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_DETAIL_ITERATOR_ADAPTER_DISTANCE_IMPL_HPP
#define BOOST_FUSION_SUPPORT_DETAIL_ITERATOR_ADAPTER_DISTANCE_IMPL_HPP

#include <boost/fusion/iterator/distance.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct distance_impl;

    template <>
    struct distance_impl<iterator_adapter_tag>
    {
        template <typename It1Ref, typename It2Ref>
        struct apply
        {
            //TODO cschmidt: category assert
            typedef typename
                result_of::distance<
                    typename detail::remove_reference<
                        It1Ref
                    >::type::iterator_type
                  , typename detail::remove_reference<
                        It2Ref
                    >::type::iterator_type
                >::type
            type;
        };
    };
}}}

#endif
