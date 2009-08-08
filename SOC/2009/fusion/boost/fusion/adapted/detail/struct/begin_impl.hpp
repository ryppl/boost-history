/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_BEGIN_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_BEGIN_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<struct_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                basic_iterator<
                    struct_iterator_tag
                  , random_access_traversal_tag
                  , SeqRef
                  , 0
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(seq,0);
            }
        };
    };

    template <>
    struct begin_impl<assoc_struct_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                basic_iterator<
                    assoc_struct_iterator_tag
                  , assoc_struct_category
                  , SeqRef
                  , 0
                >
            type;

            static type
            call(SeqRef seq)
            {
                return type(seq,0);
            }
        };
    };
}}}

#endif
