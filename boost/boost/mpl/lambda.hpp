//-----------------------------------------------------------------------------
// boost mpl/labmda.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_LAMBDA_HPP_INCLUDED
#define BOOST_MPL_LAMBDA_HPP_INCLUDED

#include "boost/mpl/bind.hpp"
#include "boost/mpl/et/placeholder.hpp"
#include "boost/mpl/make_fun.hpp"
#include "boost/mpl/limits/arity.hpp"
#include "boost/mpl/aux_/preprocessor.hpp"
#include "boost/mpl/aux_/pred.hpp" // for users
#include "boost/mpl/mpl_config.hpp" // for BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/cat.hpp"

namespace boost {
namespace mpl {

template< typename T >
struct lambda
{
    typedef T type;
};

// agurt, 15/jan/02: full-fledged implementation requires both 
// template template parameters _and_ partial specialization (is it?)

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS) \
 && !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#define BOOST_MPL_AUX_LAMBDA_PARAMS(n, param) \
    BOOST_MPL_TEMPLATE_PARAMETERS(1, BOOST_PREPROCESSOR_INC(n), param) \
/**/

#define BOOST_MPL_AUX_LAMBDA_INVOCATION(n, T) \
    BOOST_PREPROCESSOR_COMMA_IF(n) \
    typename lambda< \
        BOOST_PREPROCESSOR_CAT(T, BOOST_PREPROCESSOR_INC(n)) \
        >::type \
/**/

#define BOOST_MPL_AUX_LAMBDA_0_SPEC(unused)
#define BOOST_MPL_AUX_LAMBDA_N_SPEC(n) \
template< \
      template<BOOST_MPL_AUX_LAMBDA_PARAMS(n, typename P)> class Function \
    , BOOST_MPL_AUX_LAMBDA_PARAMS(n, typename T) \
    > \
struct lambda< Function<BOOST_MPL_AUX_LAMBDA_PARAMS(n, T)> > \
{ \
    typedef typename mpl::BOOST_PREPROCESSOR_CAT(bind,n)< \
          typename mpl::BOOST_PREPROCESSOR_CAT(make_f,n)<Function>::type \
        , BOOST_PREPROCESSOR_REPEAT(n, BOOST_MPL_AUX_LAMBDA_INVOCATION, T) \
        >::type type; \
}; \
/**/

#define BOOST_MPL_AUX_LAMBDA_SPEC(n, unused) \
    BOOST_PREPROCESSOR_IF( \
          n \
        , BOOST_MPL_AUX_LAMBDA_N_SPEC \
        , BOOST_MPL_AUX_LAMBDA_0_SPEC \
        )(n) \
/**/

// lambda<#-ary class template>
BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY)
    , BOOST_MPL_AUX_LAMBDA_SPEC
    , unused
    )

#undef BOOST_MPL_AUX_LAMBDA_SPEC
#undef BOOST_MPL_AUX_LAMBDA_N_SPEC
#undef BOOST_MPL_AUX_LAMBDA_0_SPEC
#undef BOOST_MPL_AUX_LAMBDA_INVOCATION
#undef BOOST_MPL_AUX_LAMBDA_PARAMS

#endif // defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LAMBDA_HPP_INCLUDED
