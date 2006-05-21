// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// See examples in boost/named_params.hpp for expansion.
#if !defined(BOOST_PP_IS_ITERATING)
# error Boost.NamedParams - do not include this file!
#endif

#define N BOOST_PP_ITERATION()

template<BOOST_PP_ENUM_PARAMS(N, class A)>
typename
  BOOST_NAMED_PARAMS_build_arg_list(N,aux::make_arg_list,PS,A)
::type
operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a)) const
{
    typedef typename
      BOOST_NAMED_PARAMS_build_arg_list(N,aux::make_arg_list,PS,A)
    ::type result_type;

    return result_type(
        BOOST_PP_ENUM_PARAMS(N, a)
        BOOST_PP_ENUM_TRAILING_PARAMS(
            BOOST_PP_SUB(BOOST_NAMED_PARAMS_MAX_ARITY, N)
          , aux::void_() BOOST_PP_INTERCEPT
        )
    );
}

#undef N

