
#ifndef BOOST_MPL_LAMBDA_MATCH_HPP_INCLUDED
#define BOOST_MPL_LAMBDA_MATCH_HPP_INCLUDED

// + file: boost/mpl/lambda_match.hpp
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

#include "boost/mpl/aux_/config/lambda.hpp"

#if !defined(BOOST_MPL_NO_FULL_LAMBDA_SUPPORT)
#   include "boost/mpl/aux_/lambda_match_impl.hpp"
#else
#   include "boost/mpl/aux_/lambda_match_impl_wknd.hpp"
#endif

#include "boost/mpl/aux_/config/static_constant.hpp"
#include "boost/mpl/aux_/value_wknd.hpp"
#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/bool.hpp"
#include "boost/mpl/not.hpp"
#include "boost/mpl/void.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"
#include "boost/type_traits/is_same.hpp"

namespace boost {
namespace mpl {

template <
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(PatternExpr)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(MatchExpr)
    >
struct lambda_match
{
private:
    typedef typename apply_if<
          is_same<PatternExpr,MatchExpr>
        , true_
        , aux::lambda_match_impl<PatternExpr,MatchExpr,aux::no_prior_matches>
        >::type result_;

public:
    typedef typename not_<
          is_same<result_,aux::failed_lambda_match>
        >::type type;

    BOOST_STATIC_CONSTANT(bool, value = BOOST_MPL_AUX_MSVC_VALUE_WKND(type)::value);

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,lambda_match,(PatternExpr,MatchExpr))
};

BOOST_MPL_AUX_VOID_SPEC(2, lambda_match)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_LAMBDA_MATCH_HPP_INCLUDED
