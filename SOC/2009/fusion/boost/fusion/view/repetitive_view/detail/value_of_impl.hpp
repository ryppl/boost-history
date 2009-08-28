/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_VALUE_OF_IMPL_HPP

#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/prior.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    template <>
    struct value_of_impl<repetitive_view_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename
                result_of::value_of<
                    typename result_of::prior<
                        typename detail::remove_reference<
                            It
                        >::type::it_type
                    >::type
                >::type
            type;
        };
    };
}}}

#endif
