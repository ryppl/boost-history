/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/advance_c.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <boost/mpl/modulus.hpp>
#include <boost/mpl/negate.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<
            typename Seq
          , typename MaxIndex
          , bool /*IsEmptyOrForwardOnly*/=
                mpl::or_<
                    result_of::empty<Seq>
                  , mpl::not_<traits::is_bidirectional<Seq> >
                >::value/*false*/
        >
        struct get_real_end_it
        {
            typedef
                mpl::negate<mpl::modulus<MaxIndex, result_of::size<Seq> > >
            backwards_n;

            typedef typename
                result_of::advance<
                    typename result_of::end<Seq>::type
                  , typename backwards_n::type
                >::type
            type;

            static type
            call(Seq seq)
            {
                return fusion::advance<backwards_n>(fusion::end(seq));
            }
        };

        template<typename Seq, typename MaxIndex>
        struct get_real_end_it<Seq, MaxIndex, /*IsEmptyOrForwardOnly*/true>
        {
            typedef typename
                result_of::end<
                    typename detail::remove_reference<Seq>::type::seq_type
                >
            type;

            static type
            call(Seq seq)
            {
                return type(seq,fusion::end(seq));
            }
        };
    }

    namespace extension
    {
        template <typename>
        struct end_impl;

        template <>
        struct end_impl<repetitive_view_tag>
        {
            template <typename Seq>
            struct apply
            {
                typedef typename detail::remove_reference<Seq>::type seq;
                typedef
                    detail::get_real_end_it<
                        typename seq::seq_type
                      , typename seq::size
                    >
                gen;

                typedef
                    repetitive_view_iterator<
                        typename detail::forward_as<
                            Seq
                          , typename seq::seq_type
                        >::type
                      , typename gen::type
                      , seq::size::value
                    >
                type;

                static type
                call(Seq seq)
                {
                    return type(seq.seq.get(),gen::call(seq.seq.get()));
                }
            };
        };
    }
}}

#endif
