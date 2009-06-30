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

namespace boost { namespace fusion
{
    struct list_tag;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<list_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef typename
                    detail::result_of_forward_as<Sequence
                      , typename detail::remove_reference<Sequence>::type::storage_type
                    >::type
                storage_type;

                typedef
                    iterator_adapter<
                        typename result_of::begin<storage_type>::type
                      , forward_traversal_tag
                    >
                type;

                static type
                call(Sequence s)
                {
                    return fusion::begin(
                            static_cast<storage_type>(s.get_data()));
                }
            };
        };
    }
}}

#endif
