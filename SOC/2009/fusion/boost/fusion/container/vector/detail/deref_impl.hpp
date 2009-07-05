/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DEREF_IMPL_HPP

#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost { namespace fusion
{
    struct vector_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<vector_iterator_tag>
        {
            template <typename ItRef>
            struct apply
            {
                typedef typename detail::remove_reference<ItRef>::type it;
                typedef typename it::vector vector;
                typedef typename
                    mpl::at<
                        typename detail::remove_reference<vector>::type::types
                      , typename it::index
                    >::type
                element;

                typedef typename
                    detail::result_of_forward_as<vector,element>::type
                type;

                static type
                call(ItRef it)
                {
                    return it.vec->at_impl(typename it::index());
                }
            };
        };
    }
}}

#endif
