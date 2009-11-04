/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>

#include <boost/mpl/min.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename Seq, typename M>
        struct get_endpoint
          : result_of::advance<typename result_of::begin<Seq>::type, M>
        {};

        template<typename M>
        struct endpoints
        {
            template<typename T>
            struct result;

            template<typename Self, typename SeqRef>
            struct result<Self(SeqRef)>
              : mpl::eval_if<
                    is_same<SeqRef, unused_type const&>
                  , mpl::identity<unused_type const&>
                  , get_endpoint<SeqRef, M>
                >
            {};

            template<typename Seq>
            typename result<endpoints(BOOST_FUSION_R_ELSE_LREF(Seq))>::type
            operator()(BOOST_FUSION_R_ELSE_LREF(Seq) seq) const
            {
                return fusion::advance<M>(
                        fusion::begin(BOOST_FUSION_FORWARD(Seq,seq)));
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
        struct end_impl;

        template<>
        struct end_impl<zip_view_tag>
        {
            template<typename Seq>
            struct apply
            {
                typedef typename detail::remove_reference<Seq>::type seq;

                typedef
                    zip_view_iterator<
                        typename result_of::transform<
                            typename seq::seqs_type
                          , detail::endpoints<typename seq::size>
                        >::type
                      , typename seq::category
                    >
                type;

                static type
                call(Seq seq)
                {
                    return type(
                            fusion::transform(
                                seq.seqs,
                                detail::endpoints<typename seq::size>()
                            ),
                            0);
                }
            };
        };
    }
}}

#endif
