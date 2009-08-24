/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_TRANSFORM_VIEW_DETAIL_ADVANCE_IMPL_HPP

#include <boost/fusion/iterator/advance.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct advance_impl;

    // Unary Version
    template<>
    struct advance_impl<transform_view_iterator_tag>
    {
        template<typename ItRef, typename N>
        struct apply
        {
            typedef typename
                detail::remove_reference<ItRef>::type
            it;

            typedef
                transform_view_iterator<
                    typename result_of::advance<typename it::it_type, N>::type
                  , typename it::transform_type
                >
            type;

            static type
            call(ItRef it)
            {
                return type(fusion::advance<N>(it.it), *it.f);
            }
        };
    };

    // Binary Version
    template<>
    struct advance_impl<transform_view_iterator2_tag>
    {
        template<typename ItRef, typename N>
        struct apply
        {
            typedef typename
                detail::remove_reference<ItRef>::type
            it;

            typedef
                transform_view_iterator2<
                    typename result_of::advance<typename it::it1_type, N>::type
                  , typename result_of::advance<typename it::it2_type, N>::type
                  , typename it::transform_type
                >
            type;

            static type
            call(ItRef it)
            {
                return type(fusion::advance<N>(it.it1),
                            fusion::advance<N>(it.it2),
                            it.f);
            }
        };
    };
}}}

#endif
