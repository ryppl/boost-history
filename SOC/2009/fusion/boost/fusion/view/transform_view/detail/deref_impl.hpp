/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_DEREF_IMPL_HPP

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/support/internal/result_of.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct deref_impl;

    // Unary Version
    template <>
    struct deref_impl<transform_view_iterator_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename
                detail::remove_reference<ItRef>::type
            it;

            typedef typename
                detail::internal::result_of<
                    typename detail::get_func_base<
                        typename it::transform_type
                    >::type(
                    typename result_of::deref<typename it::first_type>::type
                    )
                >::type
            type;

            static type
            call(ItRef it)
            {
                return (*it.f)(fusion::deref(it.first));
            }
        };
    };

    // Binary Version
    template <>
    struct deref_impl<transform_view_iterator2_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename detail::remove_reference<ItRef>::type it;

            typedef typename
                detail::internal::result_of<
                    typename detail::get_func_base<
                        typename it::transform_type
                    >::type(
                    typename result_of::deref<typename it::first1_type>::type
                  , typename result_of::deref<typename it::first2_type>::type)
                >::type
            type;

            static type
            call(ItRef it)
            {
                return (*it.f)(
                        fusion::deref(it.first1)
                      , fusion::deref(it.first2));
            }
        };
    };
}}}

#endif
