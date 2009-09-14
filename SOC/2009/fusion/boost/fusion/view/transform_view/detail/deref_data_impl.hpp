/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_DEREF_DATA_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_DEREF_DATA_IMPL_HPP

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/support/internal/result_of.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_data_impl;

    // Unary Version
    template <>
    struct deref_data_impl<transform_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename
                boost::result_of<
                    typename detail::get_func_base<
                        typename it::transform_type
                    >::type(
                    typename result_of::deref<typename it::it_type>::type)
                >::type
            result_pair;

            typedef typename
                detail::forward_as<
                    result_pair
                  , typename detail::remove_reference<
                        result_pair
                    >::type::second_type
                >::type
            type;

            static type
            call(It it)
            {
                return (*it.f)(fusion::deref(it.it)).second;
            }
        };
    };

    // Binary Version
    template <>
    struct deref_data_impl<transform_view_iterator2_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename
                boost::result_of<
                    typename detail::get_func_base<
                        typename it::transform_type
                    >::type(
                    typename result_of::deref<typename it::it1_type>::type
                  , typename result_of::deref<typename it::it2_type>::type)
                >::type
            result_pair;

            typedef typename
                detail::forward_as<
                    result_pair
                  , typename detail::remove_reference<result_pair>::second_type
                >::type
            type;

            static type
            call(It it)
            {
                return (*it.f)
                    (fusion::deref(it.it1), fusion::deref(it.it2)).second;
            }
        };
    };
}}}

#endif

