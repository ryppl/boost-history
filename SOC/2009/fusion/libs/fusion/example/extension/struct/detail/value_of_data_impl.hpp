/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_VALUE_OF_DATA_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_VALUE_OF_DATA_IMPL_HPP

#include "value_of_impl.hpp"

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct value_of_data_impl;

    template<>
    struct value_of_data_impl<example::example_struct_iterator_tag>
      : value_of_impl<example::example_struct_iterator_tag>
    {};
}}}

#endif
