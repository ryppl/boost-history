/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

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
#include <boost/fusion/support/internal/result_of.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#ifdef BOOST_NO_RVALUE_REFERENCES
#   include <boost/type_traits/add_const.hpp>
#endif

namespace boost { namespace fusion { namespace detail
{
    template<typename Result,int N>
    struct unrolled_fold
    {
        template<typename It0, typename State, typename F>
        static Result
        call(It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(State) state,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            typedef typename result_of::next<It1&>::type It2;
            It2 it2 = fusion::next(it1);
            typedef typename result_of::next<It2&>::type It3;
            It3 it3 = fusion::next(it2);

            return unrolled_fold<Result,N-4>::call(
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

    template<typename Result>
    struct unrolled_fold<Result,3>
    {
        template<typename It0, typename State, typename F>
        static Result
        call(It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(State) state,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            typedef typename result_of::next<It1&>::type It2;
            It2 it2 = fusion::next(it1);

            return f(fusion::deref(fusion::next(it1)),
                        f(fusion::deref(it1),
                            f(
                                fusion::deref(it0),
                                BOOST_FUSION_FORWARD(State,state)
                            )
                        )
                    );
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,2>
    {
        template<typename It0, typename State, typename F>
        static Result
        call(It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(State) state,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            return f(fusion::deref(fusion::next(it0)),
                    f(fusion::deref(it0),BOOST_FUSION_FORWARD(State,state)));
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,1>
    {
        template<typename It0, typename State, typename F>
        static Result
        call(It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(State) state,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            return f(fusion::deref(it0), BOOST_FUSION_FORWARD(State,state));
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,0>
    {
        template<typename It0, typename State, typename F>
        static Result
        call(It0 const&,
                BOOST_FUSION_R_ELSE_CLREF(State) state,
                BOOST_FUSION_R_ELSE_CLREF(F))
        {
            return state;
        }
    };

    template <typename It, typename StateRef, typename F>
    struct fold_apply
      : boost::result_of<
            F(
#ifdef BOOST_NO_RVALUE_REFERENCES
            typename add_lref<
                typename add_const<
                    typename result_of::deref<It>::type
                >::type
            >::type
#else
            typename result_of::deref<It>::type&&
#endif
          , StateRef)
        >
    {};

    template<typename It, typename State, typename F>
    struct fold_apply_rvalue_state
      : fold_apply<
            It
#ifdef BOOST_NO_RVALUE_REFERENCES
          , typename add_lref<typename add_const<State>::type>::type
#else
          , State&&
#endif
          , F
        >
    {};

    template<typename It0, typename StateRef, typename F, int N>
    struct result_of_unrolled_fold
    {
        typedef typename fold_apply_rvalue_state<It0, StateRef, F>::type rest1;
        typedef typename result_of::next<It0>::type it1;
        typedef typename fold_apply_rvalue_state<it1, rest1, F>::type rest2;
        typedef typename result_of::next<it1>::type it2;
        typedef typename fold_apply_rvalue_state<it2, rest2, F>::type rest3;
        typedef typename result_of::next<it2>::type it3;

        typedef typename
            result_of_unrolled_fold<
                typename result_of::next<it3>::type
              , typename fold_apply_rvalue_state<it3, rest3, F>::type
              , F
              , N-4
            >::type
        type;
    };

    template<typename It0, typename StateRef, typename F>
    struct result_of_unrolled_fold<It0, StateRef, F, 3>
    {
        typedef typename fold_apply_rvalue_state<It0, StateRef, F>::type rest1;
        typedef typename result_of::next<It0>::type it1;

        typedef typename
            fold_apply_rvalue_state<
                typename result_of::next<it1>::type
              , typename fold_apply_rvalue_state<it1, rest1, F>::type
              , F
            >::type
        type;
    };

    template<typename It0, typename StateRef, typename F>
    struct result_of_unrolled_fold<It0, StateRef, F, 2>
      : fold_apply_rvalue_state<
            typename result_of::next<It0>::type
          , typename fold_apply_rvalue_state<It0, StateRef, F>::type
          , F
        >
    {};

    template<typename It0, typename StateRef, typename F>
    struct result_of_unrolled_fold<It0, StateRef, F, 1>
      : fold_apply_rvalue_state<It0, StateRef, F>
    {};

    template<typename It0, typename StateRef, typename F>
    struct result_of_unrolled_fold<It0, StateRef, F, 0>
    {
        typedef StateRef type;
    };

    template<typename It0, typename StateRef, typename FRef, int SeqSize>
    struct result_of_unrolled_fold_first
    {
        typedef typename get_func_base<FRef>::type f;

        typedef typename
            result_of_unrolled_fold<
                typename result_of::next<It0>::type
              , typename fold_apply<It0, StateRef, f>::type
              , f
              , SeqSize-1
            >::type
        type;
    };

    template<int SeqSize, typename It0, typename StateRef, typename FRef>
    struct fold_impl
    {
        typedef preevaluate<FRef> preevaluater;

        typedef typename
            mpl::eval_if<
                typename preevaluater::is_preevaluable
              , preevaluater
              , result_of_unrolled_fold_first<
                    It0
                  , StateRef
                  , FRef
                  , SeqSize
                >
            >::type
        type;

        static type
        call(It0 const& it0, StateRef state, FRef f)
        {
            return unrolled_fold<type, SeqSize>::call(
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
