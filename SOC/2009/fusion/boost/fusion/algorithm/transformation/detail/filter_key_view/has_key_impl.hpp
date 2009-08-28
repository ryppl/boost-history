/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_HAS_KEY_IMPL_HPP
#define BOOST_FUSION_ALGORITHM_TRANSFORMATION_DETAIL_FILTER_KEY_VIEW_HAS_KEY_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/has_key.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion { namespace extension
{
    template <typename>
    struct has_key;

    template <>
    struct has_key<detail::filter_key_view_tag>
    {
        template <typename Seq,typename Key>
        struct apply
        {
            typedef typename detail::remove_reference<Seq>::type seq;

            typedef typename
                mpl::eval_if<
                    mpl::apply1<typename seq::pred_type,Key>
                  , mpl::identity<mpl::false_>
                  , result_of::has_key<typename seq::storage_type,Key>
                >::type
            type;
        };
    };
}}}

#endif
