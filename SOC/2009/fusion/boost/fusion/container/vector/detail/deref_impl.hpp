/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DEREF_IMPL_HPP

#include <boost/mpl/at.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct deref_impl;

    template <>
    struct deref_impl<vector_iterator_tag>
    {
        template <typename ItRef>
        struct apply
        {
            typedef typename detail::remove_reference<ItRef>::type it;
            typedef typename it::seq_type vector;

            typedef
                typename detail::forward_as<
                    vector
                  , typename mpl::at<
                        typename detail::remove_reference<
                            vector
                        >::type::types
                      , typename it::index
                    >::type
                >::type
            type;

            static type
            call(ItRef it)
            {
                return it.seq->at_impl(typename it::index());
            }
        };
    };
}}}

#endif
