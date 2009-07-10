/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2005-2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_HAS_KEY_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_HAS_KEY_IMPL_HPP

#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion { namespace extension
{
    struct no_such_member;

    template<typename Tag>
    struct has_key_impl;

    template<>
    struct has_key_impl<struct_tag>
    {
        template<typename SeqRef, typename Key>
        struct apply
          : mpl::not_<
                is_same<
                    no_such_member
                  , typename struct_assoc_member<
                        typename detail::identity<SeqRef>::type
                      , Key
                    >::type
                >
            >
        {};
    };
}}}

#endif
