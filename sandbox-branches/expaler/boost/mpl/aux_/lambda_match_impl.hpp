
#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_HPP_INCLUDED

// + file: boost/mpl/aux_/lambda_match_impl.hpp
// + last modified: 12/aug/03

// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/apply.hpp"
#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/arg.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/is_placeholder.hpp"
#include "boost/mpl/logical.hpp"
#include "boost/mpl/replace.hpp"
#include "boost/mpl/void.hpp"
#include "boost/type_traits/is_same.hpp"

#include "boost/mpl/limits/arity.hpp"
#if BOOST_MPL_METAFUNCTION_MAX_ARITY <= 10
#   include "boost/mpl/vector/vector10.hpp"
#else
#   error "lambda_match needs update for increased metafunction arity!"
#endif

#include "boost/preprocessor/iterate.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/arithmetic/inc.hpp"
#include "boost/mpl/aux_/preprocessor/params.hpp"
#include "boost/mpl/aux_/preprocessor/repeat.hpp"

namespace boost {
namespace mpl {
namespace aux {

// failed_lambda_match
//
struct no_prior_matches;
struct failed_lambda_match;

// lambda_match_placeholder_check_impl
//

template <typename PH, typename MatchExpr, typename PriorMatches>
struct lambda_match_placeholder_check_impl;

template <typename MatchExpr, typename PriorMatches>
struct lambda_match_placeholder_check_impl<
      arg<-1>, MatchExpr, PriorMatches
    >
{
    typedef PriorMatches type;
};

template <int N, typename MatchExpr, typename PriorMatches>
struct lambda_match_placeholder_check_impl<
      arg<N>, MatchExpr, PriorMatches
    >
{
    typedef typename at_c<PriorMatches,(N-1)>::type prior_match;

    typedef typename apply_if<
          is_same< prior_match, arg<N> >
        , replace< PriorMatches, prior_match, MatchExpr >
        , if_<
              is_same< prior_match, MatchExpr >
            , PriorMatches
            , failed_lambda_match
            >
        >::type type;
};

// lambda_match_placeholder_check
//

template <typename PH, typename MatchExpr, typename PriorMatches>
struct lambda_match_placeholder_check
    : lambda_match_placeholder_check_impl< PH, MatchExpr, PriorMatches >
{
};

template <typename MatchExpr>
struct lambda_match_placeholder_check< arg<-1>, MatchExpr, no_prior_matches >
{
    typedef no_prior_matches type;
};

template <typename PH, typename MatchExpr>
struct lambda_match_placeholder_check< PH, MatchExpr, no_prior_matches >
    : lambda_match_placeholder_check_impl<
          PH
        , MatchExpr
        , BOOST_PP_CAT(vector,BOOST_MPL_METAFUNCTION_MAX_ARITY)<
              BOOST_MPL_PP_PARAMS(BOOST_MPL_METAFUNCTION_MAX_ARITY, _)
            >
        >
{
};

// lambda_match_impl (primary template)
//
template <typename PatternExpr, typename MatchExpr, typename PriorMatches>
struct lambda_match_impl
    : apply_if<
          is_placeholder<PatternExpr>
        , lambda_match_placeholder_check<PatternExpr, MatchExpr, PriorMatches>
        , if_<
              is_same<PatternExpr,MatchExpr>
            , PriorMatches
            , failed_lambda_match
            >
        >
{
};

// lambda_match_impl_chain
//
template <typename T, typename U, typename Chain>
struct lambda_match_impl_chain
{
    template <typename PriorMatches>
    struct apply
    {
        typedef typename lambda_match_impl<T,U,PriorMatches>::type
            result_;

        typedef typename apply_if<
              is_same< result_,failed_lambda_match >
            , identity< failed_lambda_match >
            , apply1< Chain, result_ >
            >::type type;
    };
};

// macro BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN
//
// lambda_match_impl_chain< T1,U1, lambda_match_impl_chain< T2,U2, ...
//       lambda_match_impl_chain< TN,UN, identity<> >
//   ... > >
//

#define BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN_ELEM(z, N, _) \
    lambda_match_impl_chain< \
          BOOST_PP_CAT(T,BOOST_PP_INC(N)) \
        , BOOST_PP_CAT(U,BOOST_PP_INC(N)) \
        , \
    /**/

#define BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN_CLOSER(z, N, _) \
    > \
    /**/

#define BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN(N) \
    BOOST_MPL_PP_REPEAT(N, BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN_ELEM, _) \
      identity<> \
    BOOST_MPL_PP_REPEAT(N, BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN_CLOSER, _) \
    /**/

// lambda_match_impl (specializations)
//

#define BOOST_PP_ITERATION_LIMITS (1, BOOST_MPL_METAFUNCTION_MAX_ARITY)
#define BOOST_PP_FILENAME_1 "boost/mpl/aux_/lambda_match_impl.hpp"
#include BOOST_PP_ITERATE()

#undef BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN_ELEM
#undef BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN_CLOSER
#undef BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN

} // namespace aux
} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_HPP_INCLUDED

///// iteration, depth == 1

#elif BOOST_PP_ITERATION_DEPTH() == 1
#define i BOOST_PP_FRAME_ITERATION(1)

template <
      template <BOOST_MPL_PP_PARAMS(i, typename P)> class E
    , BOOST_MPL_PP_PARAMS(i, typename T)
    , BOOST_MPL_PP_PARAMS(i, typename U)
    , typename PriorMatches
    >
struct lambda_match_impl<
      E< BOOST_MPL_PP_PARAMS(i,T) >
    , E< BOOST_MPL_PP_PARAMS(i,U) >
    , PriorMatches
    >
    : apply1<
          BOOST_MPL_AUX_LAMBDA_MATCH_IMPL_CHAIN(i)
        , PriorMatches
        >
{
};

#undef i
#endif // BOOST_PP_IS_ITERATING
