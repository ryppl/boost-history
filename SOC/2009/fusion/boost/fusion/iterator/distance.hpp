/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_DISTANCE_HPP
#define BOOST_FUSION_ITERATOR_DISTANCE_HPP

#include <boost/fusion/iterator/detail/distance.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    // Special tags:
    struct iterator_facade_tag; // iterator facade tag

    namespace extension
    {
        template <typename Tag>
        struct distance_impl
        {
            // default implementation
            template <typename First, typename Last>
            struct apply
              : distance_detail::linear_distance<First, Last>::type
            {
                BOOST_FUSION_MPL_ASSERT_NOT(
                        traits::is_random_access<First>,
      "default implementation not available for random access iterators");
                BOOST_FUSION_MPL_ASSERT_NOT(
                        traits::is_random_access<Last>,
      "default implementation not available for random access iterators");
            };
        };

        template <>
        struct distance_impl<iterator_facade_tag>
        {
            template <typename First, typename Last>
            struct apply
              : detail::remove_reference<First>::type::
                    template distance<First, Last>::type
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
