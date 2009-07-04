/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_ADVANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_ADVANCE_IMPL_HPP

#include <boost/fusion/iterator/advance.hpp>
#include <boost/mpl/negate.hpp>

namespace boost { namespace fusion {

    struct reverse_view_iterator_tag;

    template <typename Iterator>
    struct reverse_view_iterator;

    namespace extension
    {
        template<typename Tag>
        struct advance_impl;

        template<>
        struct advance_impl<reverse_view_iterator_tag>
        {
            template<typename ItRef, typename Dist>
            struct apply
            {
                typedef typename
                    detail::remove_reference<ItRef>::type::first_type
                first_type;
                typedef typename mpl::negate<Dist>::type negative_dist;
                typedef typename
                    result_of::advance<first_type, negative_dist>::type
                advanced_type;

                typedef reverse_view_iterator<advanced_type> type;

                static type
                call(ItRef it)
                {
                    return type(
                            boost::fusion::advance<negative_dist>(it.first));
                }
            };
        };
    }
}}

#endif
