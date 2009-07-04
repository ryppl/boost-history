/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ITERATOR_RANGE_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_ITERATOR_RANGE_DETAIL_AT_IMPL_HPP

#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator/deref.hpp>

namespace boost { namespace fusion
{
    struct iterator_range_tag;

    namespace extension
    {
        template <typename Tag>
        struct at_impl;

        template <>
        struct at_impl<iterator_range_tag>
        {
            template <typename SeqRef, typename N>
            struct apply
            {
                typedef typename
                    detail::remove_reference<SeqRef>::type::begin_type
                begin_type;
                typedef typename result_of::advance<begin_type,N>::type pos;
                typedef typename result_of::deref<pos>::type type;

                static type
                call(SeqRef seq)
                {
                    return *advance<N>(seq.first);
                }
            };
        };
    }
}}

#endif
