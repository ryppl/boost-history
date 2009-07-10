/*=============================================================================
    Copyright (c) 2005 Joel de Guzman
    Copyright (c) 2005 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_BEGIN_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_BEGIN_IMPL_HPP

#include <boost/fusion/support/iterator_adapter.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<list_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                iterator_adapter<
                    typename result_of::begin<
                        typename detail::forward_as<
                            SeqRef
                          , typename detail::remove_reference<
                                SeqRef
                            >::type::storage_type
                        >::type
                    >::type
                  , forward_traversal_tag
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(fusion::begin(seq.get_data()),0);
            }
        };
    };
}}}

#endif
