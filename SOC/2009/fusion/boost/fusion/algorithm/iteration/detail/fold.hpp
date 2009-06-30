/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_ITERATION_DETAIL_FOLD_HPP
#define BOOST_FUSION_ALGORITHM_ITERATION_DETAIL_FOLD_HPP

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/distance.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Seq, typename State, typename F>
        struct fold;
    }

    namespace detail
    {
        template <typename It, typename State, typename F>
        struct fold_apply
        {
            typedef typename identity<F>::type identity_F;

            typedef typename
                identity_F::template result<
                    identity_F(
                        typename result_of::deref<It>::type
                      , State
                    )
                >::type
            type;
        };

        template <typename First, typename Last, typename State, typename F>
        struct static_fold;

        template <typename First, typename Last, typename State, typename F>
        struct next_result_of_fold
          : static_fold<
                typename result_of::next<First>::type
              , Last
              , typename fold_apply<First, State, F>::type
              , F
            >
        {
        };

        template <typename First, typename Last, typename State, typename F>
        struct static_fold
        {
            typedef typename
                mpl::eval_if<
                    result_of::equal_to<First, Last>
                  , mpl::identity<State>
                  , next_result_of_fold<First, Last, State, F>
                >::type
            type;
        };

        template<typename I0, typename State, typename F, int N>
        struct result_of_unrolled_fold;

        template<int N>
        struct unrolled_fold
        {
            template<typename I0, typename State, typename F>
            static typename result_of_unrolled_fold<I0, State, F, N>::type
            call(I0 const& i0,
                    BOOST_FUSION_R_ELSE_LREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                typedef typename result_of::next<I0>::type I1;
                I1 i1 = fusion::next(i0);
                typedef typename result_of::next<I1>::type I2;
                I2 i2 = fusion::next(i1);
                typedef typename result_of::next<I2>::type I3;
                I3 i3 = fusion::next(i2);
                typedef typename result_of::next<I3>::type I4;
                I4 i4 = fusion::next(i3);

                return unrolled_fold<N-4>::call(
                        i4,
                        f(fusion::deref(i3),
                            f(fusion::deref(i2),
                                f(fusion::deref(i1),
                                    f(fusion::deref(i0),
                                     BOOST_FUSION_FORWARD(State,state)
                                     )
                                 )
                             )
                         ),
                        f);
            }
        };

        template<>
        struct unrolled_fold<3>
        {
            template<typename I0, typename State, typename F>
            static typename result_of_unrolled_fold<I0, State, F, 3>::type
            call(I0 const& i0,
                    BOOST_FUSION_R_ELSE_LREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                typedef typename result_of::next<I0>::type I1;
                I1 i1 = fusion::next(i0);
                typedef typename result_of::next<I1>::type I2;
                I2 i2 = fusion::next(i1);

                return f(fusion::deref(i2),
                        f(fusion::deref(i1),
                            f(fusion::deref(i0), BOOST_FUSION_FORWARD(State,state))
                        ));
            }
        };

        template<>
        struct unrolled_fold<2>
        {
            template<typename I0, typename State, typename F>
            static typename result_of_unrolled_fold<I0, State, F, 2>::type
            call(I0 const& i0,
                    BOOST_FUSION_R_ELSE_LREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                typedef typename result_of::next<I0>::type I1;
                I1 i1 = fusion::next(i0);

                return f(fusion::deref(i1),
                        f(fusion::deref(i0),
                          BOOST_FUSION_FORWARD(State,state)));
            }
        };

        template<>
        struct unrolled_fold<1>
        {
            template<typename I0, typename State, typename F>
            static typename result_of_unrolled_fold<I0, State, F, 1>::type
            call(I0 const& i0,
                    BOOST_FUSION_R_ELSE_LREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                return f(fusion::deref(i0), BOOST_FUSION_FORWARD(State,state));
            }
        };

        template<>
        struct unrolled_fold<0>
        {
            template<typename I0, typename State, typename F>
            static State
            call(I0 const&,
                    BOOST_FUSION_R_ELSE_LREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F))
            {
                return state;
            }
        };

        // terminal case
        template <typename First, typename Last, typename State, typename F>
        inline BOOST_FUSION_R_ELSE_LREF(State)
        linear_fold(First const&,
                    Last const&,
                    BOOST_FUSION_R_ELSE_LREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F),
                    mpl::true_)
        {
            return state;
        }

        // non-terminal case
        template <typename First, typename Last, typename State, typename F>
        inline typename static_fold<First, Last, State, F>::type
        linear_fold(
            First const& first
          , Last const& last
          , BOOST_FUSION_R_ELSE_CLREF(State) state
          , BOOST_FUSION_R_ELSE_CLREF(F) f
          , mpl::false_)
        {
            return detail::linear_fold(
                fusion::next(first)
              , last
              , f(*first, state)
              , BOOST_FUSION_FORWARD(F,f)
              , result_of::equal_to<
                    typename result_of::next<First>::type, Last
                >()
            );
        }

        template<typename I0, typename State, typename F, int N>
        struct result_of_unrolled_fold
        {
            typedef typename result_of::next<I0>::type I1;
            typedef typename result_of::next<I1>::type I2;
            typedef typename result_of::next<I2>::type I3;
            typedef typename result_of::next<I3>::type I4;
            typedef typename fold_apply<I0, State, F>::type Rest1;
            typedef typename fold_apply<I1, Rest1, F>::type Rest2;
            typedef typename fold_apply<I2, Rest2, F>::type Rest3;
            typedef typename fold_apply<I3, Rest3, F>::type Rest4;

            typedef typename
                result_of_unrolled_fold<I4, Rest4, F, N-4>::type
            type;
        };

        template<typename I0, typename State, typename F>
        struct result_of_unrolled_fold<I0, State, F, 3>
        {
            typedef typename result_of::next<I0>::type I1;
            typedef typename result_of::next<I1>::type I2;
            typedef typename fold_apply<I0, State, F>::type Rest;
            typedef typename fold_apply<I1, Rest, F>::type Rest2;

            typedef typename
                fold_apply<I2, Rest2, F>::type
            type;
        };

        template<typename I0, typename State, typename F>
        struct result_of_unrolled_fold<I0, State, F, 2>
        {
            typedef typename result_of::next<I0>::type I1;
            typedef typename fold_apply<I0, State, F>::type Rest;

            typedef typename
                fold_apply<I1, Rest, F>::type
            type;
        };

        template<typename I0, typename State, typename F>
        struct result_of_unrolled_fold<I0, State, F, 1>
        {
            typedef typename
                fold_apply<I0, State, F>::type
            type;
        };

        template<typename I0, typename State, typename F>
        struct result_of_unrolled_fold<I0, State, F, 0>
        {
            typedef State type;
        };

        template<typename SeqRef, typename State, typename F, bool>
        struct choose_fold;

        template<typename SeqRef, typename State, typename F>
        struct choose_fold<SeqRef, State, F, true>
        {
            typedef typename result_of::begin<SeqRef>::type begin;
            typedef typename result_of::end<SeqRef>::type end;

            typedef typename
                result_of_unrolled_fold<
                    begin
                  , State
                  , F
                  , result_of::size<SeqRef>::value
                >::type
            type;
        };

        template<typename SeqRef, typename State, typename F>
        struct choose_fold<SeqRef, State, F, false>
        {
            typedef typename
                detail::static_fold<
                    typename result_of::begin<SeqRef>::type
                  , typename result_of::end<SeqRef>::type
                  , State
                  , F
                >::type
            type;
        };

        template<typename Seq, typename State, typename F, typename Tag>
        typename result_of::fold<Seq, State, F>::type
        fold(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
             BOOST_FUSION_R_ELSE_CLREF(State) state,
             BOOST_FUSION_R_ELSE_CLREF(F) f,
             Tag)
        {
            return linear_fold(
                fusion::begin(seq)
              , fusion::end(seq)
              , BOOST_FUSION_FORWARD(State,state)
              , BOOST_FUSION_FORWARD(F,f)
              , result_of::equal_to<
                    typename result_of::begin<Seq>::type
                  , typename result_of::end<Seq>::type
                >()
            );
        }

        template<typename Seq, typename State, typename F>
        typename result_of::fold<Seq, State, F>::type
        fold(BOOST_FUSION_R_ELSE_LREF(Seq) seq,
             BOOST_FUSION_R_ELSE_CLREF(State) state,
             BOOST_FUSION_R_ELSE_CLREF(F) f,
             random_access_traversal_tag)
        {
            typedef
                unrolled_fold<
                    result_of::distance<
                        typename result_of::begin<Seq>::type
                      , typename result_of::end<Seq>::type
                    >::value
                >
            gen;

            return gen::call(
                fusion::begin(seq)
              , BOOST_FUSION_FORWARD(State,state)
              , BOOST_FUSION_FORWARD(F,f));
        }
    }
}}

#endif
