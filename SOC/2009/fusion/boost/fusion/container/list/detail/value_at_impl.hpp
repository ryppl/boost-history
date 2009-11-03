/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_VALUE_AT_IMPL_HPP

#include <boost/mpl/eval_if.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<cons_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::remove_reference<Seq>::type
            seq;

            typedef typename
                mpl::eval_if<
                    N
                  , apply<typename seq::cdr_type, mpl::int_<N::value-1> >
                  , mpl::identity<typename seq::car_type>
                >::type
            type;
        };
    };

    template <>
    struct value_at_impl<list_tag>
      : value_at_impl<cons_tag>
    {};
}}}

#endif
