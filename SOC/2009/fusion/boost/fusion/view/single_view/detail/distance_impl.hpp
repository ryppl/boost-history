/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_DISTANCE_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_DISTANCE_IMPL_HPP

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct distance_impl;

    template<>
    struct distance_impl<single_view_iterator_tag>
    {
        template<typename Begin, typename End>
        struct apply
          : mpl::int_<
                detail::remove_reference<Begin>::type::end::value==
                    detail::remove_reference<End>::type::end::value
                ? 0 : 1
            >
        {};
    };
}}}

#endif
