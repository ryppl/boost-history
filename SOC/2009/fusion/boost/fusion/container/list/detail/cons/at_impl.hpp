/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_AT_IMPL_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/internal/workaround.hpp>
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename Cons, int I>
        struct cons_advance
        {
            typedef typename
                cons_advance<
                    typename cons_advance<Cons, 4>::type
                  , I-4
                >::type::cdr_type
            type;
        };

        template <typename Cons>
        struct cons_advance<Cons, 0>
        {
            typedef Cons type;
        };

        template <typename Cons>
        struct cons_advance<Cons, 1>
        {
            typedef typename Cons::cdr_type type;
        };

        template <typename Cons>
        struct cons_advance<Cons, 2>
        {
#if BOOST_WORKAROUND(BOOST_MSVC, > 1400) // VC8 and above
            typedef typename Cons::cdr_type::cdr_type type;
#else
            typedef typename Cons::cdr_type _a;
            typedef typename _a::cdr_type type;
#endif
        };

        template <typename Cons>
        struct cons_advance<Cons, 3>
        {
#if BOOST_WORKAROUND(BOOST_MSVC, > 1400) // VC8 and above
            typedef typename Cons::cdr_type::cdr_type::cdr_type type;
#else
            typedef typename Cons::cdr_type _a;
            typedef typename _a::cdr_type _b;
            typedef typename _b::cdr_type type;
#endif
        };

        template <typename Cons>
        struct cons_advance<Cons, 4>
        {
#if BOOST_WORKAROUND(BOOST_MSVC, > 1400) // VC8 and above
            typedef typename Cons::cdr_type::cdr_type::cdr_type::cdr_type type;
#else
            typedef typename Cons::cdr_type _a;
            typedef typename _a::cdr_type _b;
            typedef typename _b::cdr_type _c;
            typedef typename _c::cdr_type type;
#endif
        };
    }

    namespace extension
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
                      , typename detail::cons_advance<Seq, N::value>::type
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
    }
}}

#endif
