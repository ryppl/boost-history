/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_PRIOR_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_PRIOR_IMPL_HPP

#include <boost/fusion/iterator/next.hpp>

namespace boost { namespace fusion
{
    struct reverse_view_iterator_tag;

    template <typename Iterator>
    struct reverse_view_iterator;

    namespace extension
    {
        template <>
        struct prior_impl<reverse_view_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Iterator>::type::first_type
                first_type;

                typedef
                    reverse_view_iterator<
                        typename result_of::next<first_type>::type
                    >
                type;

                static type
                call(Iterator i)
                {
                    return type(next(i.first));
                }
            };
        };
    }
}}

#endif
