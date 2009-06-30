// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_SUPPORT_ITERATOR_ADAPTER_VALUE_OF_IMPL_HPP

#include <boost/fusion/iterator/value_of.hpp>

namespace boost { namespace fusion
{
    struct iterator_adapter_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_of_impl;

        template <>
        struct value_of_impl<iterator_adapter_tag>
        {
            template <typename Iterator>
            struct apply
              : result_of::value_of<
                    typename detail::result_of_forward_as<Iterator
                      , typename detail::remove_reference<Iterator>::type::iterator_type
                    >::type
                >
            {
            };
        };
    }
}}

#endif
