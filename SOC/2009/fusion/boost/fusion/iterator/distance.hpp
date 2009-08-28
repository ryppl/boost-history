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
        template <typename>
        struct distance_impl
        {
            template <typename BeginRef, typename EndRef>
            struct apply
              : distance_detail::linear_distance<BeginRef, EndRef>
            {};
        };

        template <>
        struct distance_impl<iterator_facade_tag>
        {
            template <typename BeginRef, typename EndRef>
            struct apply
              : detail::remove_reference<BeginRef>::type::
                  template distance<BeginRef, EndRef>
            {};
        };
    }

    namespace result_of
    {
        template <typename Begin, typename End>
        struct distance
          : extension::distance_impl<typename traits::tag_of<Begin>::type>::
                template apply<Begin, End>::type
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<Begin>));
            BOOST_FUSION_MPL_ASSERT((traits::is_iterator<End>));
        };
    }

    template <typename Begin, typename End>
    inline typename result_of::distance<Begin const&, End const&>::type
    distance(Begin const& a, End const& b)
    {
        return typename result_of::distance<Begin const&, End const&>::type();
    }
}}

#endif
