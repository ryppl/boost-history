/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_DEREF_DATA_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_DEREF_DATA_IMPL_HPP

#include "deref_impl.hpp"

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct deref_data_impl;

    template<>
    struct deref_data_impl<example::example_struct_iterator_tag>
      : deref_impl<example::example_struct_iterator_tag>
    {};
}}}

#endif
