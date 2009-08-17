/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_IMPL_HPP

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename Tag>
    struct at_impl;

    template <>
    struct at_impl<struct_tag>
    {
        template <typename SeqRef, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    SeqRef
                  , typename struct_member<
                        typename detail::identity<SeqRef>::type
                      , N::value
                    >::type
                >::type
            type;

            static type
            call(SeqRef seq)
            {
                return
                    struct_member<
                        typename detail::identity<SeqRef>::type
                      , N::value
                    >::call(seq);
            }
        };
    };

    template <>
    struct at_impl<assoc_struct_tag>
      : at_impl<struct_tag>
    {};
}}}

#endif
