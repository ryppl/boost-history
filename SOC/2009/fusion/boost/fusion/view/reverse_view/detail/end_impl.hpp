/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct end_impl;

    template <>
    struct end_impl<reverse_view_tag>
    {
        template <typename Seq>
        struct apply
        {
            typedef
                reverse_view_iterator<
                    typename result_of::begin<
                        typename detail::forward_as<
                            Seq
                          , typename detail::remove_reference<
                                Seq
                            >::type::seq_type
                        >::type
                    >::type
                 >
            type;

            static type
            call(Seq seq)
            {
                return type(fusion::begin(seq.seq.get()),0);
            }
        };
    };
}}}

#endif
