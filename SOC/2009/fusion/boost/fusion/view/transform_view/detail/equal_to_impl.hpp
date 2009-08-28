/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/fusion/iterator/equal_to.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct equal_to_impl;

    template<>
    struct equal_to_impl<transform_view_iterator_tag>
    {
        template<typename It1, typename It2>
        struct apply
          : result_of::equal_to<
                typename detail::remove_reference<It1>::type::it_type
              , typename detail::remove_reference<It2>::type::it_type
            >
        {};
    };

    template<>
    struct equal_to_impl<transform_view_iterator2_tag>
    {
        template<typename It1, typename It2>
        struct apply
          : result_of::equal_to<
                typename detail::remove_reference<It1>::type::it1_type
              , typename detail::remove_reference<It2>::type::it1_type
            >
        {};
    };
}}}

#endif
