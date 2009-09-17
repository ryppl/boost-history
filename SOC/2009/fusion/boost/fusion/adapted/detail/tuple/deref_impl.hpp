/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_TUPLE_DEREF_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_TUPLE_DEREF_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct deref_impl;

    template <>
    struct deref_impl<BOOST_FUSION_ADAPTED_TUPLE_ITERATOR_TAG>
    {
        template <typename It>
        struct apply
        {
            typedef typename detail::remove_reference<It>::type it;

            typedef typename
                detail::forward_as<
                    typename it::seq_type
                  , typename BOOST_FUSION_ADAPTED_TUPLE_NAMESPACE(element)<
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
                return
#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
#   ifdef BOOST_NO_0X_HDR_TUPLE
                    std::tr1::get<it::index::value>(
#   else
                    std::get<it::index::value>(
#   endif
#else
                    tuples::get<it::index::value>(
#endif
                        *it_.seq);
            }
        };
    };
}}}

#endif
