/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_VALUE_OF_IMPL_HPP

#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/support/internal/result_of.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    // Unary Version
    template <>
    struct value_of_impl<transform_view_iterator_tag>
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
                    typename result_of::value_of<typename it::it_type>::type)
                >::type
            type;
        };
    };

    // Binary Version
    template <>
    struct value_of_impl<transform_view_iterator2_tag>
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
                   typename result_of::value_of<typename it::it1_type>::type
                 , typename result_of::value_of<typename it::it2_type>::type)
                >::type
            type;
        };
    };
}}}

#endif
