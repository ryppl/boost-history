/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_CONTAINER_SET_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/iterator/basic_iterator.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct begin_impl;

    template <>
    struct begin_impl<set_tag>
    {
        template <typename SeqRef>
        struct apply
        {
            typedef
                basic_iterator<
                    set_iterator_tag
                  , set_category
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
