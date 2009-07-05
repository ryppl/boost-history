/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct vector_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct equal_to_impl;

        template <>
        struct equal_to_impl<vector_iterator_tag>
        {
            template <typename It1Ref, typename It2Ref>
            struct apply
              : is_same<
                    typename detail::remove_reference<It1Ref>::type::identity
                  , typename detail::remove_reference<It2Ref>::type::identity
                >
            {
            };
        };
    }
}}

#endif
