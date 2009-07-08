/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_END_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_END_IMPL_HPP

#include <boost/fusion/adapted/struct/struct_iterator.hpp>

namespace boost { namespace fusion
{
    struct struct_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <typename Struct>
        struct struct_size;

        template <>
        struct end_impl<struct_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef
                    struct_iterator<
                        Sequence
                      , struct_size<Sequence>::value
                    >
                type;

                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif
