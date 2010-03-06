/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_VALUE_AT_KEY_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_VALUE_AT_KEY_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct value_at_key_impl;

    template<>
    struct value_at_key_impl<example::example_sequence_tag>
    {
        template<typename Sequence, typename N>
        struct apply;

        template<typename Sequence>
        struct apply<Sequence, fields::name>
        {
            typedef std::string type;
        };

        template<typename Sequence>
        struct apply<Sequence, fields::age>
        {
            typedef int type;
        };
    };
}}}

#endif
