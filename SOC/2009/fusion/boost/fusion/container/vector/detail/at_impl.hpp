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
    template <typename>
    struct at_impl;

    template <>
    struct at_impl<vector_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    Seq
#ifdef BOOST_FUSION_TAGGED_VECTOR
                  , decltype(
                        detail::at_type_helper<N::value>(*static_cast<
                            typename detail::remove_reference<Seq>::type*
                        >(0))
                    )
#elif defined(BOOST_FUSION_PREFER_MPL)
                  , typename mpl::at<
                        typename detail::remove_reference<Seq>::type::types
                      , N
                    >::type
#else
                  , typename vector_meta_value_at<
                        typename detail::remove_reference<Seq>::type
                      , N::value
                    >::type
#endif
                >::type
            type;

            static type
            call(Seq seq)
            {
#ifdef BOOST_FUSION_TAGGED_VECTOR
                return detail::at_helper<N::value>(seq);
#else
                return seq.at_impl(mpl::int_<N::value>());
#endif
            }
        };
    };
}}}

#endif
