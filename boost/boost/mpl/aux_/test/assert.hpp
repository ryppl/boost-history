
#ifndef BOOST_MPL_AUX_TEST_ASSERT_HPP_INCLUDED
#define BOOST_MPL_AUX_TEST_ASSERT_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2002-2004
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/config/ctps.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#include <boost/static_assert.hpp>

#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/cat.hpp>

#define MPL_ASSERT( expr ) BOOST_STATIC_ASSERT( expr )
#define MPL_ASSERT_EQUAL(arity, tuple) assert_equal< BOOST_PP_TUPLE_REM(arity)tuple >()
#define MPL_ASSERT_NOT_EQUAL(arity, tuple) assert_not_equal< BOOST_PP_TUPLE_REM(arity)tuple >()
#define MPL_ASSERT_SAME(arity, tuple) assert_same< BOOST_PP_TUPLE_REM(arity)tuple >()
#define MPL_ASSERT_NOT_SAME(arity, tuple) assert_not_same< BOOST_PP_TUPLE_REM(arity)tuple >()

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#   define AUX_ASSERT_DEF( param_type, suffix, test_equality ) \
template< param_type x, param_type y > struct assert_##suffix \
BOOST_PP_EXPR_IF( BOOST_PP_NOT(test_equality), {} ); \
template< param_type x > struct assert_##suffix<x,x> \
BOOST_PP_EXPR_IF( test_equality, {} ); \
/**/
#elif BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#   define AUX_ASSERT_DEF( param_type, suffix, test_equality ) \
template< param_type x > struct assert_##suffix##_impl \
{ \
    template< param_type y > struct result_; \
    template<> struct result_<x> \
    { BOOST_PP_EXPR_IF( BOOST_PP_NOT(test_equality), private: virtual ~result_<x>() = 0; ) }; \
    template< param_type y > struct result_ \
    { BOOST_PP_EXPR_IF( test_equality, private: virtual ~result_<y>() = 0;  ) }; \
}; \
\
template< param_type x, param_type y > struct assert_##suffix \
    : assert_##suffix##_impl<x>::template result_<y> {}; \
/**/
#else
#   define AUX_ASSERT_DEF( param_type, suffix, test_equality ) \
template< param_type x > struct assert_##suffix##_impl \
{ \
    template< param_type y > struct result_ \
    BOOST_PP_EXPR_IF( BOOST_PP_NOT(test_equality), {} ); \
}; \
template< param_type x > struct assert_##suffix##_impl<x>::result_<x> \
BOOST_PP_EXPR_IF( test_equality, {} ); \
\
template< param_type x, param_type y > struct assert_##suffix \
    : assert_##suffix##_impl<x>::template result_<y> {}; \
/**/
#endif

namespace {
AUX_ASSERT_DEF(long, equal, 1)
AUX_ASSERT_DEF(long, not_equal, 0)
AUX_ASSERT_DEF(typename, same, 1)
AUX_ASSERT_DEF(typename, not_same, 0)
}

#undef AUX_ASSERT_DEF



template< typename T > struct MPL_ASSERTION_FAILED
{ 
#if !BOOST_WORKAROUND(BOOST_INTEL_CXX_VERSION, BOOST_TESTED_AT(800))
    int MESSAGE(T);
#else
    int MESSAGE(MPL_ASSERTION_FAILED<T>); 
#endif
};

struct MPL_ASSERTION_ARGUMENTS;

namespace mpl_ {
struct assert {};
template< bool C > struct assert_msg_arg { typedef void* type; };
template<> struct assert_msg_arg<false> { typedef assert type; };

template< bool C > int assert_msg(typename assert_msg_arg<C>::type );

}

#if !BOOST_WORKAROUND(__EDG_VERSION__, BOOST_TESTED_AT(303))
#   define MPL_ASSERT_MSG( c, msg, types ) \
    struct msg; \
    typedef mpl_::assert BOOST_PP_CAT(assert,__LINE__) types; \
    enum { \
        BOOST_PP_CAT(MplAssertion,__LINE__) = sizeof( \
            mpl_::assert_msg<(c)>( (BOOST_PP_CAT(assert,__LINE__) **************** (msg::****************))0 ) \
            ) \
    }\
/**/
#else
#   define MPL_ASSERT_MSG( c, msg, types ) \
    struct msg; \
    enum { \
        BOOST_PP_CAT(MplAssertion,__LINE__) = sizeof( \
            mpl_::assert_msg<(c)>( (mpl_::assert **************** (msg::****************) types )0 ) \
            ) \
    }\
/**/
#endif


#endif // BOOST_MPL_AUX_TEST_ASSERT_HPP_INCLUDED
