/*=============================================================================
    Phoenix
    Copyright (c) 2001-2004 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_PP_IS_ITERATING)
#if !defined(PHOENIX_BIND_IMPL_BIND_FUNCTION_IPP)
#define PHOENIX_BIND_IMPL_BIND_FUNCTION_IPP

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/spirit/phoenix/core/impl/macro_utils.hpp>

#define BOOST_PP_ITERATION_PARAMS_1                                             \
    (3, (3, BOOST_PP_DEC(PHOENIX_COMPOSITE_LIMIT),                              \
    "boost/spirit/phoenix/bind/impl/bind_function.ipp"))
#include BOOST_PP_ITERATE()

#endif

///////////////////////////////////////////////////////////////////////////////
//
//  Preprocessor vertical repetition code
//
///////////////////////////////////////////////////////////////////////////////
#else // defined(BOOST_PP_IS_ITERATING)

#define N BOOST_PP_ITERATION()

    template <typename RT, PHOENIX_TEMPLATE(N, T), PHOENIX_TEMPLATE(N, A)>
    inline actor<
        typename as_composite<
            impl::function_eval<N>
          , impl::function_ptr<N, RT, RT(*)(BOOST_PP_ENUM_PARAMS(N, T))>
          , BOOST_PP_ENUM_PARAMS(N, A)
        >::type>
    bind(RT(*f)(BOOST_PP_ENUM_PARAMS(N, T)), PHOENIX_CREF_ARGS(N, A))
    {
        typedef impl::function_ptr<
            N, RT, RT(*)(BOOST_PP_ENUM_PARAMS(N, T))> fp_type;
        return compose<impl::function_eval<N> >(
            fp_type(f), BOOST_PP_ENUM_PARAMS(N, _));
    }

#undef N
#endif // defined(BOOST_PP_IS_ITERATING)


