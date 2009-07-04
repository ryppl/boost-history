/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>

#include <boost/mpl/if.hpp>

namespace boost { namespace fusion
{
    struct joint_view_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<joint_view_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef typename
                    result_of::end<
                        typename detail::remove_reference<
                            SeqRef
                        >::type::seq2_type
                    >::type
                type;

                static type
                call(SeqRef seq)
                {
                    return fusion::end(seq.seq2.get());
                }
            };
        };
    }
}}

#endif
