/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_BEGIN_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_BEGIN_IMPL_HPP

namespace boost { namespace fusion
{
    struct cons_tag;

    template <typename Cons>
    struct cons_iterator;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<cons_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef cons_iterator<Sequence> type;

                static type
                call(Sequence t)
                {
                    return type(t);
                }
            };
        };
    }
}}

#endif
