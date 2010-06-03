/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_NEXT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct next_impl;

    template<>
    struct next_impl<cons_iterator_tag>
    {
        template<typename It>
        struct apply
        {
            typedef typename
                detail::remove_reference<It>::type::cons_type
            cons_type;
            typedef typename
                detail::forward_as<
                    cons_type
                  , typename detail::remove_reference<
                        cons_type
                    >::type::cdr_type
                >::type
            cdr_type;

            typedef cons_iterator<cdr_type> type;

            static type
            call(It it)
            {
                return type(static_cast<cdr_type>(it.cons->cdr),0);
            }
        };
    };
}}}

#endif
