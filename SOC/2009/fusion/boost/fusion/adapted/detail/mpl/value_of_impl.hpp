/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_MPL_VALUE_OF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_MPL_VALUE_OF_IMPL_HPP

#include <boost/mpl/deref.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_impl;

    template <>
    struct value_of_impl<mpl_iterator_tag>
    {
        template <typename It>
        struct apply
          : mpl::deref<typename detail::identity<It>::type>
        {};
    };
}}}

#endif
