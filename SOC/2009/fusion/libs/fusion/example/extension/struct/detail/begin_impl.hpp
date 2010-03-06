/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_BEGIN_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_BEGIN_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct begin_impl;

    template<>
    struct begin_impl<example::example_sequence_tag>
    {
        template<typename SeqRef>
        struct apply
        {
            typedef example::example_struct_iterator<SeqRef, 0> type;

            static type
            call(SeqRef seq)
            {
                return type(seq);
            }
        };
    };
}}}

#endif
