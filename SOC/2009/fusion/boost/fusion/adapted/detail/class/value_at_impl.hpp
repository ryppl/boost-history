/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_CLASS_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_CLASS_VALUE_AT_IMPL_HPP

#include <boost/fusion/adapted/detail/struct/value_at_impl.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<class_tag>
      : value_at_impl<struct_tag>
    {};

    template <>
    struct value_at_impl<assoc_class_tag>
      : value_at_impl<assoc_struct_tag>
    {};
}}}

#endif
