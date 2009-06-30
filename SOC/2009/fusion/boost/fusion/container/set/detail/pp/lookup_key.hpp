/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_SET_DETAIL_PP_LOOKUP_KEY_HPP
#define BOOST_FUSION_CONTAINER_SET_DETAIL_PP_LOOKUP_KEY_HPP

#include <boost/fusion/support/detail/unknown_key.hpp>

namespace boost { namespace fusion { namespace detail
{
    template <typename T, int index>
    struct set_key_type
    {
        typedef T type;
    };

    template <int index>
    struct set_key_type<void_, index>
    {
        typedef unknown_key<index> type;
    };
}}}

#endif
