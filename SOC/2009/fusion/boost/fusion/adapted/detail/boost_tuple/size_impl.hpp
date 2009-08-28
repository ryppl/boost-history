/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_SIZE_IMPL_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_BOOST_TUPLE_SIZE_IMPL_HPP

#include <boost/mpl/int.hpp>

namespace boost { namespace fusion { namespace extension
{
    template<typename>
    struct size_impl;

    template <>
    struct size_impl<boost_tuple_tag>
    {
        template <typename Seq>
        struct apply
          : mpl::int_<
                tuples::length<typename detail::identity<Seq>::type>::value
            >
        {};
    };
}}}

#endif
