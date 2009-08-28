/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_CONTAINER_LIST_DETAIL_LIST_VALUE_AT_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/value_at.hpp>

namespace boost { namespace fusion { namespace extension
{
    //TODO doc
    template <typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<list_tag>
    {
        template <typename Seq, typename N>
        struct apply
          : result_of::value_at<
                typename detail::remove_reference<
                    Seq
                >::type::storage_type
              , N
            >
        {};
    };
}}}

#endif
