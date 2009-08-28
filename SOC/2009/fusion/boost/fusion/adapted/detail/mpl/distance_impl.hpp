/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_DISTANCE_IMPL_HPP

#include <boost/fusion/adapted/detail/mpl/get_mpl_it.hpp>

#include <boost/mpl/distance.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct distance_to_impl;

    template <>
    struct distance_to_impl<mpl_iterator_tag>
    {
        template <typename It1, typename It2>
        struct apply
          : mpl::distance<
                typename detail::identity<It1>::type
              , typename detail::get_mpl_it<
                    typename detail::identity<It2>::type
                >::type
            >
        {};
    };
}}}

#endif
