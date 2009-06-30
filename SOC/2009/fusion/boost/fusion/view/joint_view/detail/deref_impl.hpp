/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_VIEW_JOINT_VIEW_DETAIL_DEREF_IMPL_HPP

#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion
{
    struct joint_view_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<joint_view_iterator_tag>
        {
            template<typename ItRef>
            struct apply
            {
                typedef typename
                    result_of::deref<
                        typename detail::remove_reference<
                            ItRef
                        >::type::first_type
                    >::type
                type;

                static type
                call(ItRef i)
                {
                    return fusion::deref(i.first);
                }
            };
        };
    }
}}

#endif
