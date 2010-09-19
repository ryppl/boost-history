/*==============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_NEXT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_NEXT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/addressof.hpp>

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
            typedef typename result_of::empty<cdr_type>::type empty;

            typedef typename
                mpl::if_c<
                    empty::value
                  , nil_iterator
                  , cons_iterator<cdr_type>
                >::type
            type;

            static type
            call(It, mpl::true_)
            {
                return type();
            }

            static type
            call(It it, mpl::false_)
            {
                return type(boost::addressof(it.cons->cdr),0);
            }

            static type
            call(It it)
            {
                return call(it, empty());
            }
        };
    };
}}}

#endif
