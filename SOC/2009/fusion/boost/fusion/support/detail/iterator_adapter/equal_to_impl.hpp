// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_EQUAL_TO_IMPL_HPP

#include <boost/fusion/iterator/equal_to.hpp>

namespace boost { namespace fusion
{
    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct equal_to_impl;

        template <>
        struct equal_to_impl<iterator_adapter_tag>
        {
            template <typename I1, typename I2>
            struct apply
              : result_of::equal_to<
                    typename detail::result_of_forward_as<I1
                      , typename detail::remove_reference<I1>::type::iterator_type
                    >::type
                  , typename detail::result_of_forward_as<I2
                      , typename detail::remove_reference<I2>::type::iterator_type
                    >::type
                >
            {
            };
        };
    }
}}

#endif
