/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_DISTANCE_HPP
#define BOOST_FUSION_ITERATOR_DISTANCE_HPP

#include <boost/fusion/iterator/detail/distance.hpp>
#include <boost/fusion/support/internal/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;
    struct iterator_facade_tag;

    namespace extension
    {
        template <typename Tag>
        struct distance_impl
        {
            template <typename FirstRef, typename LastRef>
            struct apply
              : distance_detail::linear_distance<FirstRef, LastRef>
            {};
        };

        template <>
        struct distance_impl<iterator_facade_tag>
        {
            template <typename FirstRef, typename LastRef>
            struct apply
              : detail::remove_reference<FirstRef>::type::
                  template distance<FirstRef, LastRef>
            {};
        };
    }

    namespace result_of
    {
        template <typename First, typename Last>
        struct distance
          : extension::distance_impl<typename traits::tag_of<First>::type>::
                template apply<
                    typename detail::add_lref<First>::type
                  , typename detail::add_lref<Last>::type
                >::type
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<First>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<Last>));
        };
    }

    template <typename First, typename Last>
    inline typename result_of::distance<First const&, Last const&>::type
    distance(First const& a, Last const& b)
    {
        return typename result_of::distance<First const&, Last const&>::type();
    }
}}

#endif
