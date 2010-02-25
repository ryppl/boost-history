/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct equal_to_impl;

    template<>
    struct equal_to_impl<nview_iterator_tag>
    {
        template<typename It1, typename It2>
        struct apply
        {
            typedef typename detail::remove_reference<It1>::type it1;
            typedef typename detail::remove_reference<It2>::type it2;

            typedef
                mpl::and_<
                    is_same<
                        typename detail::identity<typename it1::seq_type>::type
                      , typename detail::identity<typename it2::seq_type>::type
                    >
                  , is_same<
                        typename it1::pos_type
                      , typename it2::pos_type
                    >
                >
            type;
        };
    };
}}}

#endif
