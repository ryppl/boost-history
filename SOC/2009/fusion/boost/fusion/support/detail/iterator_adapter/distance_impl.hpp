// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_DISTANCE_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_DISTANCE_IMPL_HPP

#include <boost/fusion/iterator/distance.hpp>

namespace boost { namespace fusion
{
    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct distance_impl;

        template <>
        struct distance_impl<iterator_adapter_tag>
        {
            template <typename I1, typename I2>
            struct apply
            {
                //TODO cschmidt: category assert

                typedef typename
                    detail::result_of_forward_as<I1
                      , typename detail::remove_reference<I1>::type::iterator_type
                    >::type
                iterator1_type;
                typedef typename
                    detail::result_of_forward_as<I2
                      , typename detail::remove_reference<I2>::type::iterator_type
                    >::type
                iterator2_type;

                typedef typename
                    result_of::distance<iterator1_type,iterator2_type>::type
                type;
            };
        };
    }
}}

#endif
