/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_VALUE_AT_KEY_IMPL_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_VALUE_AT_KEY_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct value_at_key;

    template <>
    struct value_at_key<detail::filter_key_view_tag>
    {
        template <typename Seq,typename Key>
        struct apply
          : result_of::value_at_key<
                typename detail::remove_reference<Seq>::type_seq_type
              , Key
            >
        {};
    };
}}}

#endif
