/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_EQUAL_TO_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_EQUAL_TO_IMPL_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct equal_to_impl;

    template<>
    struct equal_to_impl<single_view_iterator_tag>
    {
        template<typename It1, typename It2>
        struct apply
        {
            typedef typename detail::remove_reference<It1>::type it1;
            typedef typename detail::remove_reference<It2>::type it2;

            typedef
                mpl::and_<
                    is_same<typename it1::value_type, typename it2::value_type>
                  , mpl::equal_to<typename it1::end, typename it2::end>
                >
            type;
        };
    };
}}}

#endif
