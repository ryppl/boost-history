/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_AT_IMPL_HPP

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct at_impl;

    template <>
    struct at_impl<boost_tuple_tag>
    {
        template <typename SeqRef, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    SeqRef
                  , typename tuples::element<
                        N::value
                      , typename detail::identity<SeqRef>::type
                    >::type
                >::type
            type;

            static type
            call(SeqRef seq)
            {
                return tuples::get<N::value>(
                        BOOST_FUSION_FORWARD(SeqRef,seq));
            }
        };
    };
}}}

#endif
