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

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<int N>
    struct unrolled_fold
    {
        template<typename Result,typename It0, typename State, typename F>
        static Result
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

            return unrolled_fold<N-4>::template call<Result>(
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
        template<typename Result,typename It0, typename State, typename F>
        static Result
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
        template<typename Result,typename It0, typename State, typename F>
        static Result
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
        template<typename Result,typename It0, typename State, typename F>
        static Result
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
        template<typename Result,typename It0, typename State, typename F>
        static Result
        call(It0 const&,
                BOOST_FUSION_R_ELSE_CLREF(State) state,
                BOOST_FUSION_R_ELSE_LREF(F))
        {
            return state;
        }
    };

    template <typename It, typename StateRef, typename FRef>
    struct fold_apply
    {
        typedef typename result_of::deref<It>::type deref_type;

#ifdef BOOST_NO_RVALUE_REFERENCES
        typedef typename
            support::result_of<FRef(deref_type, StateRef)>::type
        type;
#else
        typedef typename
            mpl::eval_if<
                typename detail::is_lrref<deref_type>::type
              , support::result_of<FRef(deref_type, StateRef)>
              , support::result_of<FRef(deref_type&&, StateRef)>
            >::type
        type;
#endif
    };

    template<typename It0, typename StateRef, typename FRef, int N>
    struct result_of_unrolled_fold
    {
        typedef typename fold_apply<It0, StateRef, FRef>::type rest1;
        typedef typename result_of::next<It0>::type it1;
        typedef typename fold_apply<it1, BOOST_FUSION_R_ELSE_CLREF(rest1), FRef>::type rest2;
        typedef typename result_of::next<it1>::type it2;
        typedef typename fold_apply<it2, BOOST_FUSION_R_ELSE_CLREF(rest2), FRef>::type rest3;
        typedef typename result_of::next<it2>::type it3;
        typedef typename fold_apply<it3, BOOST_FUSION_R_ELSE_CLREF(rest3), FRef>::type rest4;
        typedef typename result_of::next<it3>::type it4;

        typedef typename
            mpl::eval_if_c<
                !(N-4)
              , mpl::identity<rest4>
              , result_of_unrolled_fold<
                    it4
                  , BOOST_FUSION_R_ELSE_CLREF(rest4)
                  , FRef
                  , N-4
                >
            >::type
        type;
    };

    template<typename It0, typename StateRef, typename FRef>
    struct result_of_unrolled_fold<It0, StateRef, FRef, 3>
    {
        typedef typename fold_apply<It0, StateRef, FRef>::type rest1;
        typedef typename result_of::next<It0>::type it1;
        typedef typename fold_apply<it1, BOOST_FUSION_R_ELSE_CLREF(rest1), FRef>::type rest2;
        typedef typename result_of::next<it1>::type it2;

        typedef typename
            fold_apply<it2, BOOST_FUSION_R_ELSE_CLREF(rest2), FRef>::type
        type;
    };

    template<typename It0, typename StateRef, typename FRef>
    struct result_of_unrolled_fold<It0, StateRef, FRef, 2>
    {
        typedef typename fold_apply<It0, StateRef, FRef>::type rest;
        typedef typename result_of::next<It0>::type it1;

        typedef typename
            fold_apply<it1, BOOST_FUSION_R_ELSE_CLREF(rest), FRef>::type
        type;
    };

    template<typename It0, typename StateRef, typename FRef>
    struct result_of_unrolled_fold<It0, StateRef, FRef, 1>
    {
        typedef typename
            fold_apply<It0, BOOST_FUSION_R_ELSE_CLREF(StateRef), FRef>::type
        type;
    };

    template<int SeqSize, typename It0, typename StateRef, typename FRef>
    struct fold_impl
    {
        typedef typename
            mpl::eval_if<
                typename support::is_preevaluable<FRef>::type
              , support::preevaluate<FRef>
              , result_of_unrolled_fold<It0, StateRef, FRef, SeqSize>
            >::type
        type;

        static type
        call(It0 const& it0, StateRef state, FRef f)
        {
            return unrolled_fold<SeqSize>::template call<type>(
                    it0,
                    BOOST_FUSION_FORWARD(StateRef,state),
                    BOOST_FUSION_FORWARD(FRef,f));
        }
    };

    template<typename It0, typename StateRef, typename FRef>
    struct fold_impl<0,It0,StateRef,FRef>
    {
        typedef StateRef type;

        static StateRef
        call(It0 const&, StateRef state, FRef)
        {
            return state;
        }
    };
}}}

#endif
