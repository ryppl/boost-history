/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_VALUE_AT_IMPL_HPP

#include <boost/mpl/eval_if.hpp>

namespace boost { namespace fusion
{
    struct cons_tag;

    namespace extension
    {
        template <typename Tag>
        struct value_at_impl;

        template <>
        struct value_at_impl<cons_tag>
        {
            template <typename Seq, typename N>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Seq>::type
                identity_sequence;

                typedef typename
                    mpl::eval_if<
                        N
                      , mpl::identity<typename identity_sequence::car_type>
                        //cschmidt: qualifiers could be moved here, though
                        //this is not necessary!
                      , apply<typename identity_sequence::cdr_type
                            , mpl::int_<N::value-1>
                        >
                    >::type
                type;
            };
        };
    }
}}

#endif
