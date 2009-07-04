/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_END_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_END_IMPL_HPP

namespace boost { namespace fusion
{
    struct nil;

    struct cons_tag;

    template <typename Car, typename Cdr>
    struct cons;

    template <typename Cons>
    struct cons_iterator;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<cons_tag>
        {
            template <typename Seq>
            struct apply
            {
                typedef
                    cons_iterator<
                        typename detail::result_of_forward_as<Seq,nil>::type
                    >
                type;

                static type
                call(Seq)
                {
                    return type();
                }
            };
        };
    }
}}

#endif
