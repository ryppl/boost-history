/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_BASIC_AUTO_FUNCTION_HPP
#define BOOST_AUTO_FUNCTION_BASIC_AUTO_FUNCTION_HPP

#include <boost/auto_function/support.hpp>

// If BOOST_BASIC_AUTO_FUNCTION is supported
#ifndef BOOST_NO_BOOST_BASIC_AUTO_FUNCTION

#include <boost/auto_function/auto_function.hpp>

#define BOOST_BASIC_AUTO_FUNCTION( ... )                                       \
BOOST_AUTO_FUNCTION( ( __VA_ARGS__ ), BOOST_BASIC_AUTO_FUNCTION_DETAIL_IMPL

// Takes the function body as a parameter
#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_IMPL( ... ) ( do __VA_ARGS__ ) )

#else // Else, BOOST_BASIC_AUTO_FUNCTION is not supported

#include <boost/auto_function/detail/error.hpp>

#ifndef BOOST_NO_VARIADIC_MACROS // If variadic macros are supported

// Output an error when BOOST_AUTO_FUNCTION is used when it is not supported
#define BOOST_BASIC_AUTO_FUNCTION( ... )                                       \
void __VA_ARGS__; BOOST_BASIC_AUTO_FUNCTION_DETAIL_IMPL

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_IMPL( ... )                           \
BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR();

#else // Else, variadic macros aren't supported

#include <boost/preprocessor/cat.hpp>

// Output an error when BOOST_AUTO_FUNCTION is used when it is not supported
#define BOOST_BASIC_AUTO_FUNCTION                                              \
void BOOST_PP_CAT( dummy_fun_, __LINE__ );                                     \
BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR();                           \
BOOST_BASIC_AUTO_FUNCTION_DETAIL_TRY_EAT_TWICE

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_TRY_EAT_TWICE( arg )                  \
BOOST_BASIC_AUTO_FUNCTION_DETAIL_TRY_EAT

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_TRY_EAT( arg )

#endif // End check for variadic macro support

#endif // End BOOST_BASIC_AUTO_FUNCTION support check

#endif // BOOST_AUTO_FUNCTION_BASIC_AUTO_FUNCTION_HPP
