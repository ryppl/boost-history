/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_DISTANCE_IMPL_HPP

#include <boost/fusion/iterator/distance.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct distance_impl;

    // Unary Version
    template<>
    struct distance_impl<transform_view_iterator_tag>
    {
        template<typename Begin, typename End>
        struct apply
          : result_of::distance<
                typename detail::remove_reference<Begin>::type::it_type
              , typename detail::remove_reference<End>::type::it_type
            >
        {};
    };

    // Binary Version
    template<>
    struct distance_impl<transform_view_iterator2_tag>
    {
        template<typename Begin, typename End>
        struct apply
          : result_of::distance<
                typename detail::remove_reference<Begin>::type::it1_type
              , typename detail::remove_reference<End>::type::it1_type
            >
        {};
    };
}}}

#endif
