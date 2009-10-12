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
        template<typename State, typename It0, typename F>
        static Result
        call(BOOST_FUSION_R_ELSE_CLREF(State) state,
                It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            typedef typename result_of::next<It1&>::type It2;
            It2 it2 = fusion::next(it1);
            typedef typename result_of::next<It2&>::type It3;
            It3 it3 = fusion::next(it2);

            return unrolled_fold<Result,N-4>::call(
                    f(
                        f(
                            f(
                                f(
                                    BOOST_FUSION_FORWARD(State,state),
                                    fusion::deref(it0)
                                ),
                                fusion::deref(it1)
                            ),
                            fusion::deref(it2)
                        ),
                        fusion::deref(it3)
                    ),
                    fusion::next(it3),
                    BOOST_FUSION_FORWARD(F,f));
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,3>
    {
        template<typename State, typename It0, typename F>
        static Result
        call(BOOST_FUSION_R_ELSE_CLREF(State) state,
                It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            typedef typename result_of::next<It0 const&>::type It1;
            It1 it1 = fusion::next(it0);
            typedef typename result_of::next<It1&>::type It2;
            It2 it2 = fusion::next(it1);

            return f(
                        f(
                            f(
                                BOOST_FUSION_FORWARD(State,state),
                                fusion::deref(it0)
                            ),
                            fusion::deref(it1)
                        ),
                        fusion::deref(fusion::next(it1))
                    );
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,2>
    {
        template<typename State, typename It0, typename F>
        static Result
        call(BOOST_FUSION_R_ELSE_CLREF(State) state,
                It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            return f(
                    f(BOOST_FUSION_FORWARD(State,state),fusion::deref(it0)),
                    fusion::deref(fusion::next(it0)));
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,1>
    {
        template<typename State, typename It0, typename F>
        static Result
        call(BOOST_FUSION_R_ELSE_CLREF(State) state,
                It0 const& it0,
                BOOST_FUSION_R_ELSE_CLREF(F) f)
        {
            return f(BOOST_FUSION_FORWARD(State,state),fusion::deref(it0));
        }
    };

    template<typename Result>
    struct unrolled_fold<Result,0>
    {
        template<typename State, typename It0, typename F>
        static Result
        call(BOOST_FUSION_R_ELSE_CLREF(State) state,
                It0 const&,
                BOOST_FUSION_R_ELSE_CLREF(F))
        {
            return state;
        }
    };

    template <typename StateRef, typename It, typename F>
    struct fold_apply
      : boost::result_of<
            F(
            StateRef,
#ifdef BOOST_NO_RVALUE_REFERENCES
            typename add_lref<
                typename add_const<
                    typename result_of::deref<It>::type
                >::type
            >::type
#else
            typename result_of::deref<It>::type&&
#endif
            )
        >
    {};

    template<typename State, typename It, typename F>
    struct fold_apply_rvalue_state
      : fold_apply<
#ifdef BOOST_NO_RVALUE_REFERENCES
            typename add_lref<typename add_const<State>::type>::type
#else
            State&&
#endif
          , It
          , F
        >
    {};

    template<typename StateRef, typename It0, typename F, int N>
    struct result_of_unrolled_fold
    {
        typedef typename fold_apply_rvalue_state<StateRef, It0, F>::type rest1;
        typedef typename result_of::next<It0>::type it1;
        typedef typename fold_apply_rvalue_state<rest1, it1, F>::type rest2;
        typedef typename result_of::next<it1>::type it2;
        typedef typename fold_apply_rvalue_state<rest2, it2, F>::type rest3;
        typedef typename result_of::next<it2>::type it3;

        typedef typename
            result_of_unrolled_fold<
                typename fold_apply_rvalue_state<rest3, it3, F>::type
              , typename result_of::next<it3>::type
              , F
              , N-4
            >::type
        type;
    };

    template<typename StateRef, typename It0, typename F>
    struct result_of_unrolled_fold<StateRef, It0, F, 3>
    {
        typedef typename fold_apply_rvalue_state<StateRef, It0, F>::type rest1;
        typedef typename result_of::next<It0>::type it1;

        typedef typename
            fold_apply_rvalue_state<
                typename fold_apply_rvalue_state<rest1, it1, F>::type
              , typename result_of::next<it1>::type
              , F
            >::type
        type;
    };

    template<typename StateRef, typename It0, typename F>
    struct result_of_unrolled_fold<StateRef, It0, F, 2>
      : fold_apply_rvalue_state<
            typename fold_apply_rvalue_state<StateRef, It0, F>::type
          , typename result_of::next<It0>::type
          , F
        >
    {};

    template<typename StateRef, typename It0, typename F>
    struct result_of_unrolled_fold<StateRef, It0, F, 1>
      : fold_apply_rvalue_state<StateRef, It0, F>
    {};

    template<typename StateRef, typename It0, typename F>
    struct result_of_unrolled_fold<StateRef, It0, F, 0>
    {
        typedef StateRef type;
    };

    template<typename StateRef, typename It0, typename FRef, int SeqSize>
    struct result_of_unrolled_fold_first
    {
        typedef typename get_func_base<FRef>::type f;

        typedef typename
            result_of_unrolled_fold<
                typename fold_apply<StateRef, It0, f>::type
              , typename result_of::next<It0>::type
              , f
              , SeqSize-1
            >::type
        type;
    };

    template<int SeqSize, typename StateRef, typename It0, typename FRef>
    struct fold_impl
    {
        typedef preevaluate<FRef> preevaluater;

        typedef typename
            mpl::eval_if<
                typename preevaluater::is_preevaluable
              , preevaluater
              , result_of_unrolled_fold_first<
                    StateRef
                  , It0
                  , FRef
                  , SeqSize
                >
            >::type
        type;

        static type
        call(StateRef state, It0 const& it0, FRef f)
        {
            return unrolled_fold<type, SeqSize>::call(
                    BOOST_FUSION_FORWARD(StateRef,state),
                    it0,
                    BOOST_FUSION_FORWARD(FRef,f));
        }
    };

    template<typename StateRef, typename It0, typename FRef>
    struct fold_impl<0,StateRef,It0,FRef>
    {
        typedef StateRef type;

        static StateRef
        call(StateRef state, It0 const&, FRef)
        {
            return state;
        }
    };
}}}

#endif
