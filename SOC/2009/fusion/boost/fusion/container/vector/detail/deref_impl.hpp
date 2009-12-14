/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DEREF_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_DEREF_IMPL_HPP

#ifdef BOOST_FUSION_PREFER_MPL
#   include <boost/mpl/at.hpp>
#endif

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<vector_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;
            typedef typename it::seq_type vector;

            typedef
                typename detail::forward_as<
                    vector
#ifdef BOOST_FUSION_TAGGED_VECTOR
                  , decltype(
                        detail::at_type_helper<it::index::value>(*static_cast<
                            typename detail::remove_reference<vector>::type*
                        >(0))
                    )
#elif defined(BOOST_FUSION_PREFER_MPL)
                  , typename mpl::at<
                        typename detail::remove_reference<
                            vector
                        >::type::types
                      , typename it::index
                    >::type
#else
                  , typename vector_meta_value_at<
                        typename detail::remove_reference<vector>::type
                      , it::index::value
                    >::type
#endif
                >::type
            type;

            static type
            call(It it)
            {
#ifdef BOOST_FUSION_TAGGED_VECTOR
                return detail::at_helper<it::index::value>(*it.seq);
#else
                return it.seq->at_impl(typename it::index());
#endif
            }
        };
    };
}}}

#endif
