/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_AT_IMPL_HPP

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct at_impl;

    template <>
    struct at_impl<struct_tag>
    {
        template <typename Seq, typename N>
        struct apply
        {
            typedef typename
                detail::forward_as<
                    Seq
                  , typename struct_member<
                        typename detail::identity<Seq>::type
                      , N::value
                    >::type
                >::type
            type;

            static type
            call(Seq seq)
            {
                return
                    struct_member<
                        typename detail::identity<Seq>::type
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
