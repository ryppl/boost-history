/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_SUPPORT_HPP
#define BOOST_AUTO_FUNCTION_SUPPORT_HPP

#include <boost/config.hpp>

// ToDo: Remove this, add such a macro to Boost.Config
// Currently no compilers support this feature
#define BOOST_NO_LAMBDA_IN_RETURN_TYPE

// The base requirements for BOOST_AUTO_FUNCTION
// (should probably add extended SFINAE check as well)
#if !(    defined( BOOST_NO_DECLTYPE )                                         \
       && defined( BOOST_NO_TRAILING_RETURN_TYPE )                             \
       && defined( BOOST_NO_VARIADIC_MACROS )                                  \
     )

#include <boost/auto_function/detail/is_recognized_arg.hpp>
#include <boost/auto_function/detail/param_id.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/iif.hpp>

#if 0

#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_IMPL_UNREC(param_id) 1

// ToDo: Possibly produce error here if not recognized instead of yielding 1
#define BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( param_id )                         \
BOOST_PP_IIF( BOOST_AUTO_FUNCTION_DETAIL_IS_RECOGNIZED_ARG( ( param_id ) )     \
            , BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_IMPL           \
            , BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_IMPL_UNREC     \
            )( param_id )


#else

#define BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( param_id )                         \
BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_IMPL( param_id )

#endif

#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_IMPL( param_id )     \
BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_ ## param_id

// "auto explicit" isn't implemented yet for any compiler
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_auto_explicit     1
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_auto_if           0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_auto_not          0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_explicit          0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_if                0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_not               0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_return            0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_return_ref        0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_return_value      0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_try               0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_virtual_explicit  0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_void              0

#ifndef BOOST_NO_RVALUE_REFERENCES // If rvalue references are supported
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_return_rvalue_ref 0
#else // Else, rvalue references are not supported
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_return_rvalue_ref 1
#endif // End rvalue reference support check

// If we can use lambda functions in the return type
#ifndef BOOST_NO_LAMBDA_IN_RETURN_TYPE

#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_auto_try          0
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_do                0

#else // Else, lambda functions in the return type are not supported

#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_auto_try          1
#define BOOST_AUTO_FUNCTION_DETAIL_NO_AUTO_FUNCTION_PARAM_do                1

#endif // End lambda in return type check

#else // Else, BOOST_AUTO_FUNCTION isn't supported

#define BOOST_NO_BOOST_AUTO_FUNCTION

// Nothing is supported
#define BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( arg ) 1

#endif // End BOOST_AUTO_FUNCTION support check

// BOOST_BASIC_AUTO_FUNCTION requires do support
#if BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( do )

#define BOOST_NO_BOOST_BASIC_AUTO_FUNCTION

#endif

#endif // BOOST_AUTO_FUNCTION_SUPPORT_HPP
