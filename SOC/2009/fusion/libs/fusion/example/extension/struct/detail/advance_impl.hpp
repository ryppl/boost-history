/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef EXTENSION_STRUCT_DETAIL_ADVANCE_IMPL_HPP
#define EXTENSION_STRUCT_DETAIL_ADVANCE_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct advance_impl;

    template<>
    struct advance_impl<example::example_struct_iterator_tag>
    {
        template<typename ItRef, typename N>
        struct apply
        {
            typedef typename detail::remove_reference<ItRef>::type it;

            typedef
                example::example_struct_iterator<
                    typename it::struct_type
                  , it::index::value + N::value
                >
            type;

            static type
            call(ItRef it)
            {
                return type(it.struct_);
            }
        };
    };
}}}

#endif
