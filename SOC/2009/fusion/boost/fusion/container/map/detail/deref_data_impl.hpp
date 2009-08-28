/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_MAP_DETAIL_DEREF_DATA_IMPL_HPP
#define BOOST_FUSION_CONTAINER_MAP_DETAIL_DEREF_DATA_IMPL_HPP

#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_data_impl;

    template <>
    struct deref_data_impl<map_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                detail::forward_as<
                    typename it::seq_type
                  , typename result_of::value_of<It>::type::second_type
                >::type
            type;

            static type
            call(It it)
            {
                return deref(it).second;
            }
        };
    };
}}}

#endif
