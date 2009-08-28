/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_VALUE_OF_IMPL_HPP

#include <boost/fusion/iterator/value_of.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    template <>
    struct value_of_impl<joint_view_iterator_tag>
    {
        template <typename It>
        struct apply
          : result_of::value_of<
                typename detail::remove_reference<It>::type::begin_type
            >
        {};
    };

    template <>
    struct value_of_impl<concat_iterator_tag>
      : value_of_impl<joint_view_iterator_tag>
    {};
}}}

#endif
