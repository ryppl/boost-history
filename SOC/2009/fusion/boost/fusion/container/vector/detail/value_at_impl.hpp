/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_VECTOR_DETAIL_VALUE_AT_IMPL_HPP

#ifdef BOOST_FUSION_PREFER_MPL
#   include <boost/mpl/at.hpp>
#endif

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<vector_tag>
    {
        template <typename Seq, typename N>
        struct apply
#ifdef BOOST_FUSION_PREFER_MPL
          : mpl::at<
                typename detail::remove_reference<Seq>::type::types
              , N
            >
#else
          : vector_meta_value_at<
                typename detail::remove_reference<Seq>::type
              , N::value
            >
#endif
        {};
    };
}}}

#endif
