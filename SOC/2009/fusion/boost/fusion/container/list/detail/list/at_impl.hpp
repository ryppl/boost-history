/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct at_impl;

    template <>
    struct at_impl<list_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                result_of::at<
                    typename detail::forward_as<
                        Seq
                      , typename detail::remove_reference<
                            Seq
                        >::type::storage_type
                    >::type
                  , N
                >::type
            type;

            static type
            call(Seq seq)
            {
                return fusion::at<N>(seq.get_data());
            }
        };
    };
}}}

#endif
