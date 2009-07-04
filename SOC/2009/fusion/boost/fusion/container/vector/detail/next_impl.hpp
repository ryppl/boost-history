/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_NEXT_IMPL_HPP

namespace boost { namespace fusion
{
    struct vector_iterator_tag;

    template <typename Vector, int N>
    struct vector_iterator;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<vector_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Iterator>::type::vector
                vector;
                typedef typename
                    detail::remove_reference<Iterator>::type::index
                index;
                typedef vector_iterator<vector, index::value+1> type;

                static type
                call(Iterator i)
                {
                    return type(*i.vec,0);
                }
            };
        };
    }
}}

#endif
