/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_BEGIN_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_BEGIN_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/support/unused.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        struct poly_begin
        {
            template<typename T>
            struct result;

            template<typename Self, typename SeqRef>
            struct result<Self(SeqRef)>
              : mpl::eval_if<
                    is_same<SeqRef, unused_type const&>
                  , mpl::identity<unused_type const&>
                  , result_of::begin<SeqRef>
                >
            {};

            template<typename Seq>
            typename result<poly_begin(BOOST_FUSION_R_ELSE_LREF(Seq))>::type
            operator()(BOOST_FUSION_R_ELSE_LREF(Seq) seq) const
            {
                return fusion::begin(BOOST_FUSION_FORWARD(Seq,seq));
            }

            unused_type const&
            operator()(unused_type const& unused) const
            {
                return unused;
            }
        };
    }

    namespace extension
    {
        template<typename>
        struct begin_impl;

        template<>
        struct begin_impl<zip_view_tag>
        {
            template<typename Seq>
            struct apply
            {
                typedef typename
                    detail::remove_reference<Seq>::type
                seq;

                typedef
                    zip_view_iterator<
                        typename result_of::transform<
                            typename seq::seqs_type
                          , detail::poly_begin
                        >::type
                      , typename seq::category
                    >
                type;

                static type
                call(Seq seq)
                {
                    return type(
                        fusion::transform(seq.seqs, detail::poly_begin()),
                        0);
                }
            };
        };
    }
}}

#endif
