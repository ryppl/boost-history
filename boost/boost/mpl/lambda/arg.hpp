//-----------------------------------------------------------------------------
// boost mpl/lambda/arg.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Peter Dimov, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_LAMBDA_ARG_HPP_INCLUDED
#define BOOST_MPL_LAMBDA_ARG_HPP_INCLUDED

#include "boost/mpl/limits/arity.hpp"

#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/cat.hpp"

namespace boost {
namespace mpl {

template<int N> struct arg;

// agurt, 17/jan/02: not sure if it's needed
typedef arg<-1> unknown;
typedef unknown placeholder; // deprecated

#define BOOST_MPL_AUX_ARG_N_DEF_BODY(n) \
typedef arg<n> BOOST_PREPROCESSOR_CAT(_,n); \
namespace lambda_arg { \
using boost::mpl::BOOST_PREPROCESSOR_CAT(_,n); \
} \
/**/

#define BOOST_MPL_AUX_ARG_N_DEF(n, unused) \
    BOOST_MPL_AUX_ARG_N_DEF_BODY(BOOST_PREPROCESSOR_INC(n)) \
/**/

// typedef arg<#> _#; namespace lambda_arg { using boost::mpl::_#; }
BOOST_PREPROCESSOR_REPEAT(
      BOOST_MPL_FUNCTION_CLASS_MAX_ARITY
    , BOOST_MPL_AUX_ARG_N_DEF
    , unused
    )

#undef BOOST_MPL_AUX_ARG_N_DEF
#undef BOOST_MPL_AUX_ARG_N_DEF_BODY

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LAMBDA_ARG_HPP_INCLUDED
