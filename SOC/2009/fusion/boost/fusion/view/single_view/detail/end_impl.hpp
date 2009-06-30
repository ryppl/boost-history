/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_DETAIL_END_IMPL_HPP

namespace boost { namespace fusion
{
    struct single_view_tag;

    template <typename T>
    struct single_view_iterator_end;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<single_view_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef single_view_iterator_end<Sequence> type;

                static type
                call(Sequence)
                {
                    return type();
                }
            };
        };
    }
}}

#endif
