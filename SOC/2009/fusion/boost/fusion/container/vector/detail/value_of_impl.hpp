/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VALUE_OF_IMPL_HPP

#ifdef BOOST_FUSION_PREFER_MPL
#   include <boost/mpl/at.hpp>
#endif

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct value_of_impl;

    template <>
    struct value_of_impl<vector_iterator_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename detail::remove_reference<ItRef>::type it;

            typedef typename
#ifdef BOOST_FUSION_PREFER_MPL
                mpl::at<
                    typename detail::remove_reference<
                        typename it::seq_type
                    >::type::types
                  , typename it::index
                >::type
#else
                vector_meta_value_at<
                    typename detail::remove_reference<
                        typename it::seq_type
                    >::type
                  , it::index::value
                >::type
#endif
            type;
        };
    };
}}}

#endif
