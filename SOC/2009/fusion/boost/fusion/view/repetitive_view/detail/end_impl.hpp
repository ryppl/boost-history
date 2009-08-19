// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_END_IMPL_HPP
#define BOOST_FUSION_VIEW_REPETITIVE_VIEW_DETAIL_END_IMPL_HPP

#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/iterator/detail/advance.hpp>

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
            typename SeqRef
          , typename MaxIndex
          , bool /*IsEmptyOrForwardOnly*/=
                mpl::or_<
                    result_of::empty<SeqRef>
                  , mpl::not_<traits::is_bidirectional<SeqRef> >
                >::type/*false*/
        >
        struct get_real_end_it
        {
            typedef typename result_of::end<SeqRef>::type seq_end;
            typedef
                mpl::modulus<MaxIndex, result_of::size<SeqRef> >
            backwards_n;

            typedef typename
                mpl::eval_if<
                    traits::is_random_access<SeqRef>
                  , result_of::advance<seq_end, mpl::negate<backwards_n> >
                  , detail::backward<seq_end, backwards_n::value>
                >::type
            type;

            static type
            call_impl(SeqRef seq, mpl::true_ /*is_random_access*/)
            {
                return fusion::advance<mpl::negate<backwards_n> >(
                        fusion::end(seq));
            }

            static type
            call_impl(SeqRef seq, mpl::false_ /*is_random_access*/)
            {
                return detail::backward<seq_end, backwards_n::value>::call(
                        fusion::end(seq));
            }

            static type
            call(SeqRef seq)
            {
                return call_impl(seq,
                        typename traits::is_random_access<SeqRef>::type());
            }
        };

        template<typename SeqRef, typename MaxIndex>
        struct get_real_end_it<SeqRef, MaxIndex, /*IsEmptyOrForwardOnly*/true>
        {
            typedef typename
                result_of::end<
                    typename detail::remove_reference<SeqRef>::type::seq_type
                >
            type;

            static type
            call(SeqRef seq)
            {
                return fusion::end(seq);
            }
        };
    }

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<repetitive_view_tag>
        {
            template <typename SeqRef>
            struct apply
            {
                typedef typename detail::remove_reference<SeqRef>::type seq;
                typedef
                    detail::get_real_end_it<
                        typename seq::seq_type
                      , typename seq::size
                    >
                gen;

                typedef
                    repetitive_view_iterator<
                        typename seq::seq_type
                      , typename gen::type
                      , seq::size::value
                    >
                type;

                static type
                call(SeqRef seq)
                {
                    return type(gen::call(seq.seq.get()));
                }
            };
        };
    }
}}

#endif
