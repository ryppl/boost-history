/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_EMPTY_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_CONS_EMPTY_IMPL_HPP

#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct empty_impl;

    template <>
    struct empty_impl<cons_tag>
    {
        template <typename Seq>
        struct apply
          : boost::is_same<typename detail::identity<Seq>::type, nil>
        {};
    };
}}}

#endif
