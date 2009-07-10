/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DISTANCE_IMPL_HPP

#include <boost/mpl/minus.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct distance_impl;

    template <>
    struct distance_impl<vector_iterator_tag>
    {
        template <typename First, typename Last>
        struct apply
        {
            typedef typename
                mpl::minus<
                    typename detail::remove_reference<Last>::type::index
                  , typename detail::remove_reference<First>::type::index
                >::type
            type;
        };
    };
}}}

#endif
