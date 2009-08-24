/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_AT_IMPL_HPP

#ifdef BOOST_FUSION_PREFER_MPL
#   include <boost/mpl/at.hpp>
#endif
#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename Tag>
    struct at_impl;

    template <>
    struct at_impl<vector_tag>
    {
        template <typename SeqRef, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    SeqRef
#ifdef BOOST_FUSION_PREFER_MPL
                  , typename mpl::at<
                        typename detail::remove_reference<
                            SeqRef
                        >::type::types
                      , N
                    >::type
#else
                  , typename vector_meta_value_at<
                        typename detail::remove_reference<SeqRef>::type
                      , N::value
                    >::type
#endif
                >::type
            type;

            static type
            call(SeqRef seq)
            {
                return seq.at_impl(mpl::int_<N::value>());
            }
        };
    };
}}}

#endif
