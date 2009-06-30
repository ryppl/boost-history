/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_AT_IMPL_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_DETAIL_AT_IMPL_HPP

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/support/assert.hpp>

#include <boost/type_traits/is_reference.hpp>
#include <boost/fusion/support/unused.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    struct zip_view_tag;

    namespace detail
    {
        template<typename N>
        struct poly_at
        {
            template<typename T>
            struct result;

            template<typename N1, typename SeqRef>
            struct result<poly_at<N1>(SeqRef)>
              : mpl::eval_if<is_same<SeqRef, unused_type const&>
                           , mpl::identity<unused_type const&>
                           , result_of::at<SeqRef, N>
                >
            {
                //BOOST_MPL_ASSERT((is_reference<SeqRef>));
            };

            template<typename Seq>
            typename result<poly_at(BOOST_FUSION_R_ELSE_LREF(Seq))>::type
            operator()(BOOST_FUSION_R_ELSE_LREF(Seq) seq) const
            {
                return fusion::at<N>(BOOST_FUSION_FORWARD(Seq,seq));
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
        template<typename Tag>
        struct at_impl;

        template<>
        struct at_impl<zip_view_tag>
        {
            template<typename SeqRef, typename N>
            struct apply
            {
                typedef typename
                    result_of::as_vector<
                        typename result_of::transform<
                            typename detail::remove_reference<
                                SeqRef
                            >::type::seqs_type
                          , detail::poly_at<N>
                        >::type
                    >::type
                type;

                static type
                call(SeqRef seq)
                {
                    return type(sequence_assign(fusion::transform(
                            seq.seqs, detail::poly_at<N>())));
                }
            };
        };
    }
}}

#endif
