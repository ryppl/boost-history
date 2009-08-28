/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_DEREF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_DEREF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<boost_tuple_iterator_tag>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                detail::forward_as<
                    typename it::seq_type
                  , typename tuples::element<
                        it::index::value
                      , typename detail::remove_reference<
                            typename it::seq_type
                        >::type
                    >::type
                >::type
            type;

            static type
            call(It it_)
            {
                return get<it::index::value>(*it_.seq);
            }
        };
    };
}}}

#endif
