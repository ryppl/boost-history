/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_AT_IMPL_HPP

#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost { namespace fusion
{
    struct vector_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_impl;

        template <>
        struct at_impl<vector_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                typedef typename
                    mpl::at<
                        typename detail::remove_reference<Sequence>::type::types
                      , N
                    >::type
                element;

                typedef typename
                    detail::result_of_forward_as<Sequence,element>::type
                type;

                static type
                call(Sequence v)
                {
                    return v.at_impl(N());
                }
            };
        };
    }
}}

#endif
