/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_NVIEW_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_NVIEW_DETAIL_DISTANCE_IMPL_HPP

#include <boost/mpl/distance.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct distance_impl;

    template<>
    struct distance_impl<nview_iterator_tag>
    {
        template<typename It1, typename It2>
        struct apply
          : mpl::distance<
                typename detail::remove_reference<It1>::type::pos_type
              , typename detail::remove_reference<It2>::type::pos_type
            >
        {};
    };
}}}

#endif
