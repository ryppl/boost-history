/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_HAS_KEY_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_HAS_KEY_IMPL_HPP

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct has_key_impl;

    template<>
    struct has_key_impl<example::example_sequence_tag>
    {
        template<typename SeqRef, typename Key>
        struct apply
          : mpl::or_<
                is_same<Key, fields::name>
              , is_same<Key, fields::age>
            >
        {};
    };
}}}

#endif
