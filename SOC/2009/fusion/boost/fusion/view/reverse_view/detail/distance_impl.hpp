/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_DISTANCE_IMPL_HPP

#include <boost/fusion/iterator/distance.hpp>

namespace boost { namespace fusion {

    struct reverse_view_iterator_tag;

    template <typename First>
    struct reverse_view_iterator;

    namespace extension
    {
        template<typename Tag>
        struct distance_impl;

        template<>
        struct distance_impl<reverse_view_iterator_tag>
        {
            template<typename First, typename Last>
            struct apply
            {
                typedef typename
                    detail::remove_reference<First>::type::first_type
                first_type;
                typedef typename
                    detail::remove_reference<Last>::type::first_type
                last_type;

                typedef typename
                    result_of::distance<last_type, first_type>::type
                type;
            };
        };
    }
}}

#endif
