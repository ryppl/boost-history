/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/ for library home page.
 */
#ifndef BOOST_PP_IS_ITERATING

#ifndef BOOST_DSL_EXPR_PP_HPP_
#define BOOST_DSL_EXPR_PP_HPP_

#    include <boost/preprocessor/repetition.hpp>
#    include <boost/preprocessor/arithmetic/sub.hpp>
#    include <boost/preprocessor/punctuation/comma_if.hpp>
#    include <boost/preprocessor/iteration/iterate.hpp>

// generate specializations
#    define BOOST_PP_ITERATION_LIMITS (1, BOOST_DSL_EXPR_ARITY)
#    define BOOST_PP_FILENAME_1       "boost/dsl/expr_pp.hpp" // this file
#    include BOOST_PP_ITERATE()

#endif // BOOST_DSL_EXPR_HPP_

#else // BOOST_PP_IS_ITERATING

#  define n BOOST_PP_ITERATION()

#  define BOOST_DSL_EXPR_signature_parameter(z, n, data) \
	typename signature_parameter<Param##n>::type


namespace boost { 
namespace dsl {
namespace detail{

template <BOOST_PP_ENUM_PARAMS(n,typename Param)> 
struct signature<mpl::vector<BOOST_PP_ENUM_PARAMS(n,Param)> >{
    typedef parameter::parameters<
    BOOST_PP_ENUM(n,BOOST_DSL_EXPR_signature_parameter,~)
    >                                      type;
};

template <typename Signature, BOOST_PP_ENUM_PARAMS(n,typename Arg)> 
struct args<Signature, mpl::vector<BOOST_PP_ENUM_PARAMS(n,Arg)> > {
	typedef typename Signature::template bind<BOOST_PP_ENUM_PARAMS(n,Arg)>::type type;
};

template <typename UnmatchedSignature, BOOST_PP_ENUM_PARAMS(n,typename Arg)> 
struct unmatched_args<UnmatchedSignature, mpl::vector<BOOST_PP_ENUM_PARAMS(n,Arg)> > {
	typedef typename UnmatchedSignature::template bind<BOOST_PP_ENUM_PARAMS(n,Arg)>::type type;
};

}
}
}

#  undef n
#  undef BOOST_DSL_EXPR_signature_parameter

#endif // BOOST_PP_IS_ITERATING 
