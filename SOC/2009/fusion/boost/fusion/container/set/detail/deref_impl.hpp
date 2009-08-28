/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_CONTAINER_SET_DETAIL_DEREF_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<set_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                result_of::at<
                    typename detail::forward_as<
                        typename it::seq_type
                      , typename detail::remove_reference<
                            typename it::seq_type
                        >::type::storage_type
                    >::type
                  , typename it::index
                >::type
            type;

            static type
            call(It it_)
            {
                return at<typename it::index>(it_.seq->get_data());
            }
        };
    };
}}}

#endif
