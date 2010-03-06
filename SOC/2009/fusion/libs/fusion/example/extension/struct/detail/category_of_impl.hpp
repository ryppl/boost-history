/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_CATEGORY_OF_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_CATEGORY_OF_IMPL_HPP

#include <boost/fusion/support/category_of.hpp>

namespace boost { namespace fusion {

    namespace extension
    {
        template<>
        struct category_of_impl<example::example_sequence_tag>
        {
            template<typename SeqRef>
            struct apply
            {
                struct type
                  : random_access_traversal_tag, associative_tag
                {};
            };
        };
    }
}}

#endif
