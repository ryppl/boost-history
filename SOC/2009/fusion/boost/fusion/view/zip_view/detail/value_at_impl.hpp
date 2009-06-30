/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_VALUE_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_VALUE_AT_IMPL_HPP

#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/support/unused.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion {

    struct zip_view_tag;

    namespace detail
    {
        template<typename N>
        struct poly_value_at
        {
            template<typename T>
            struct result;

            template<typename N1, typename Seq>
            struct result<poly_value_at<N1>(Seq)>
              : mpl::eval_if<
                    is_same<Seq, unused_type const&>
                  , mpl::identity<unused_type const&>
                  , result_of::value_at<Seq, N>
                >
            {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct value_at_impl;

        template<>
        struct value_at_impl<zip_view_tag>
        {
            template<typename SeqRef, typename N>
            struct apply
            {
                typedef typename
                    result_of::transform<
                        typename detail::remove_reference<
                            SeqRef
                        >::type::seqs_type
                      , detail::poly_value_at<N>
                    >::type
                values;

                typedef typename result_of::as_vector<values>::type type;
            };
        };
    }
}}

#endif
