/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_AT_IMPL_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct at_impl;

    template <>
    struct at_impl<cons_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    Seq
                  , typename value_at_impl<cons_tag>::
                        template apply<Seq, N>::type
                >::type
            type;

            template <typename Cons, int N2>
            static type
            call(Cons& cons, mpl::int_<N2>)
            {
                return call(cons.cdr, mpl::int_<N2-1>());
            }

            template <typename Cons>
            static type
            call(Cons& cons, mpl::int_<0>)
            {
                return cons.car;
            }

            static type
            call(Seq seq)
            {
                return call(seq, mpl::int_<N::value>());
            }
        };
    };

    template <>
    struct at_impl<list_tag>
      : at_impl<cons_tag>
    {};
}}}

#endif
