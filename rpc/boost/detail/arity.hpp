#ifndef BOOST_ARITY
#define BOOST_ARITY

#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_shifted_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/detail/storable.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include "mrepeat.hpp"

// Number of arguments
#define BOOST_ARITY_NUM_ARGS BOOST_PP_ITERATION()
#define BOOST_ARITY_NUM_ARGS_SHIFTED BOOST_PP_INC(BOOST_ARITY_NUM_ARGS)
// deprecated form
#define BOOST_ARITY_NUM_ARGS_INC BOOST_PP_INC(BOOST_ARITY_NUM_ARGS)

// helper macros
// Concatenate Prefix, Insertion, and Suffix
#define BOOST_ARITY_INSERT(P,I,S) BOOST_PP_CAT(P,BOOST_PP_CAT(I,S))
#define BOOST_ARITY_COMMA BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,BOOST_PP_COMMA,BOOST_PP_EMPTY)()
#define BOOST_ARITY_COLON BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,:,BOOST_PP_EMPTY())

// for a variable/argument
// BOOST_ARITY_VAR_[name]_NAME(n) - name of the n^th variable
#define BOOST_ARITY_aN_NAME(z,n,data) BOOST_PP_CAT(a,BOOST_PP_INC(n))
#define BOOST_ARITY_argN_type_NAME(z,n,data) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(n),_type)

#define BOOST_ARITY_function_traits_argN_type_TYPENAME(z,n,signature) \
    typename boost::function_traits<signature>::BOOST_ARITY_argN_type_NAME(z,n,BOOST_PP_EMPTY())

// repetition mechanisms
#define BOOST_ARITY_REPEAT(macro,data) BOOST_PP_REPEAT(BOOST_ARITY_NUM_ARGS,macro,data)
#define BOOST_ARITY_ENUM(macro,data) BOOST_PP_ENUM(BOOST_ARITY_NUM_ARGS,macro,data)
#define BOOST_ARITY_ENUM_TRAILING(macro,data) BOOST_PP_ENUM_TRAILING(BOOST_ARITY_NUM_ARGS,macro,data)

// argI_type definitions
#define BOOST_ARITY_ARG_TYPE_NAME(I) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_type)
// argI_storable_type definition
#define BOOST_ARITY_ARG_STORABLE_NAME(I) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_storable_type)

#define BOOST_ARITY_TYPE(z,I,signature) \
    typedef typename boost::function_traits<signature>::BOOST_ARITY_ARG_TYPE_NAME(I) \
    BOOST_ARITY_ARG_TYPE_NAME(I);


#define BOOST_ARITY_ENUM_TRAILING_MAM(m1, a2, m3) \
    BOOST_ARITY_ENUM_TRAILING(BOOST_ARITY_MAM_MACRO, (m1, a2, m3))

#define BOOST_ARITY_CMAMC(c1, m2, a3, m4, c5) \
    BOOST_ARITY_REPEAT(BOOST_ARITY_CMAMC_MACRO, (c1, m2, a3, m4, c5))

#define BOOST_ARITY_TYPEDEF_MACRO(z, n, m1_a1_m2_a2) \
    typedef BOOST_ARITY_MAMA_MACRO(z, n, m1_a1_m2_a2);

#define BOOST_ARITY_TYPEDEF(oldtype,newtype) \
    BOOST_ARITY_REPEAT(BOOST_ARITY_TYPEDEF_MACRO, (oldtype, BOOST_PP_EMPTY(), newtype, BOOST_PP_EMPTY()))

#define BOOST_ARITY_CMAMA(c1, m2, a3, m4, a5) \
    BOOST_ARITY_REPEAT(BOOST_ARITY_CMAMA_MACRO, (c1, m2, a3, m4, a5))

#define BOOST_ARITY_MMAM(m1, m2, a3, m4) \
    BOOST_ARITY_REPEAT(BOOST_ARITY_MMAM_MACRO, (m1, m2, a3, m4))

#define BOOST_ARITY_CMC(c1, m, c2) \
    BOOST_ARITY_REPEAT(BOOST_ARITY_CMC_MACRO, (c1, m, c2))

#endif