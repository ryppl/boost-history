/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_NEXT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_NEXT_IMPL_HPP

namespace boost { namespace fusion
{
    struct cons_iterator_tag;

    template <typename Cons>
    struct cons_iterator;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<cons_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Iterator>::type::cons_type
                cons_type;
                typedef typename
                    detail::remove_reference<cons_type>::type::cdr_type
                cdr_type;

                typedef
                    cons_iterator<
                        typename detail::result_of_forward_as<
                            cons_type
                          , cdr_type
                        >::type
                    >
                type;

                static type
                call(Iterator i)
                {
                    return type(i.cons.cdr);
                }
            };
        };
    }
}}

#endif
