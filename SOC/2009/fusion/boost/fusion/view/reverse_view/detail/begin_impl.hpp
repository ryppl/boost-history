/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>

namespace boost { namespace fusion
{
    struct reverse_view_tag;

    template <typename First>
    struct reverse_view_iterator;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<reverse_view_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef
                    reverse_view_iterator<
                        typename result_of::end<
                            typename detail::remove_reference<
                                SeqRef
                            >::type::seq_type
                        >::type
                    >
                type;

                static type
                call(SeqRef seq)
                {
                    return type(fusion::end(seq.seq.get()));
                }
            };
        };
    }
}}

#endif
