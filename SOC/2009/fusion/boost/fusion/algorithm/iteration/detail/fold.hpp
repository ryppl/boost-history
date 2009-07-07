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
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/support/result_of.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template <typename ItRef, typename StateRef, typename FRef>
        struct fold_apply
          : support::result_of<
                FRef(
                    typename result_of::deref<ItRef>::type
                  , StateRef
                )
            >
        {};

        template<typename It0Ref, typename StateRef, typename FRef, int N>
        struct result_of_unrolled_fold;

        template<int N>
        struct unrolled_fold
        {
            template<typename It0, typename State, typename F>
            static typename
                result_of_unrolled_fold<
                    It0 const&
                  , BOOST_FUSION_R_ELSE_CLREF(State)
                  , BOOST_FUSION_R_ELSE_LREF(F)
                  , N
                >::type
            call(It0 const& it0,
                    BOOST_FUSION_R_ELSE_CLREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                typedef typename result_of::next<It0 const&>::type It1;
                It1 it1 = fusion::next(it0);
                typedef typename result_of::next<It1&>::type It2;
                It2 it2 = fusion::next(it1);
                typedef typename result_of::next<It2&>::type It3;
                It3 it3 = fusion::next(it2);

                return unrolled_fold<N-4>::call(
                        fusion::next(it3),
                        f(fusion::deref(it3),
                            f(fusion::deref(it2),
                                f(fusion::deref(it1),
                                    f(fusion::deref(it0),
                                     BOOST_FUSION_FORWARD(State,state)
                                    )
                                )
                            )
                        ),
                        BOOST_FUSION_FORWARD(F,f));
            }
        };

        template<>
        struct unrolled_fold<3>
        {
            template<typename It0, typename State, typename F>
            static typename
                result_of_unrolled_fold<
                    It0 const&
                  , BOOST_FUSION_R_ELSE_CLREF(State)
                  , BOOST_FUSION_R_ELSE_LREF(F)
                  , 3>::type
            call(It0 const& it0,
                    BOOST_FUSION_R_ELSE_CLREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                typedef typename result_of::next<It0 const&>::type It1;
                It1 it1 = fusion::next(it0);
                typedef typename result_of::next<It1&>::type It2;
                It2 it2 = fusion::next(it1);

                return f(fusion::deref(it2),
                            f(fusion::deref(it1),
                                f(
                                    fusion::deref(it0),
                                    BOOST_FUSION_FORWARD(State,state)
                                )
                            )
                        );
            }
        };

        template<>
        struct unrolled_fold<2>
        {
            template<typename It0, typename State, typename F>
            static typename
                result_of_unrolled_fold<
                    It0 const&
                  , BOOST_FUSION_R_ELSE_CLREF(State)
                  , BOOST_FUSION_R_ELSE_LREF(F)
                  , 2
                >::type
            call(It0 const& it0,
                    BOOST_FUSION_R_ELSE_CLREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                return f(fusion::deref(fusion::next(it0)),
                        f(fusion::deref(it0),
                          BOOST_FUSION_FORWARD(State,state)));
            }
        };

        template<>
        struct unrolled_fold<1>
        {
            template<typename It0, typename State, typename F>
            static typename
                result_of_unrolled_fold<
                    It0 const&
                  , BOOST_FUSION_R_ELSE_CLREF(State)
                  , BOOST_FUSION_R_ELSE_LREF(F)
                  , 1
                >::type
            call(It0 const& it0,
                    BOOST_FUSION_R_ELSE_CLREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F) f)
            {
                return f(fusion::deref(it0), BOOST_FUSION_FORWARD(State,state));
            }
        };

        template<>
        struct unrolled_fold<0>
        {
            template<typename It0, typename State, typename F>
            static BOOST_FUSION_R_ELSE_CLREF(State)
            call(It0 const&,
                    BOOST_FUSION_R_ELSE_CLREF(State) state,
                    BOOST_FUSION_R_ELSE_LREF(F))
            {
                return state;
            }
        };

        template<typename It0, typename StateRef, typename FRef, int N>
        struct result_of_unrolled_fold
        {
            typedef typename result_of::next<It0>::type It1;
            typedef typename result_of::next<It1>::type It2;
            typedef typename result_of::next<It2>::type It3;
            typedef typename result_of::next<It3>::type It4;
            typedef typename fold_apply<It0, StateRef, FRef>::type Rest1;
            typedef typename fold_apply<It1, Rest1, FRef>::type Rest2;
            typedef typename fold_apply<It2, Rest2, FRef>::type Rest3;
            typedef typename fold_apply<It3, Rest3, FRef>::type Rest4;

            typedef typename
                result_of_unrolled_fold<It4, Rest4, FRef, N-4>::type
            type;
        };

        template<typename It0Ref, typename StateRef, typename FRef>
        struct result_of_unrolled_fold<It0Ref, StateRef, FRef, 3>
        {
            typedef typename result_of::next<It0Ref>::type It1;
            typedef typename result_of::next<It1>::type It2;
            typedef typename fold_apply<It0Ref, StateRef, FRef>::type Rest;
            typedef typename fold_apply<It1, Rest, FRef>::type Rest2;

            typedef typename
                fold_apply<It2, Rest2, FRef>::type
            type;
        };

        template<typename It0Ref, typename StateRef, typename FRef>
        struct result_of_unrolled_fold<It0Ref, StateRef, FRef, 2>
        {
            typedef typename result_of::next<It0Ref>::type It1;
            typedef typename fold_apply<It0Ref, StateRef, FRef>::type Rest;

            typedef typename
                fold_apply<It1, Rest, FRef>::type
            type;
        };

        template<typename It0Ref, typename StateRef, typename FRef>
        struct result_of_unrolled_fold<It0Ref, StateRef, FRef, 1>
        {
            typedef typename
                fold_apply<It0Ref, StateRef, FRef>::type
            type;
        };

        template<typename It0Ref, typename StateRef, typename FRef>
        struct result_of_unrolled_fold<It0Ref, StateRef, FRef, 0>
        {
            typedef StateRef type;
        };
    }
}}

#endif
