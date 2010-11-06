/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_ERROR_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_HPP

#include <boost/config.hpp>
#include <boost/auto_function/detail/param_id.hpp>
#include <boost/auto_function/support.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#ifndef BOOST_NO_STATIC_ASSERT // If static_assert is supported

#define BOOST_AUTO_FUNCTION_DETAIL_ERROR( string_, identifier_ )               \
static_assert( false, string_ )

#else // Else, static_assert isn't supported so use BOOST_MPL_ASSERT_MSG

#include <boost/mpl/assert.hpp>

#define BOOST_AUTO_FUNCTION_DETAIL_ERROR( string_, identifier_ )               \
BOOST_MPL_ASSERT_MSG( false, identifier_, () )

#endif // End check for static_assert support

#ifndef BOOST_NO_BOOST_AUTO_FUNCTION // If BOOST_AUTO_FUNCTION is supported

// Error if an argument isn't parenthesized
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_NOT_PARENTHESIZED( arg_num, line )    \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "Argument #" BOOST_PP_STRINGIZE( arg_num ) " of BOOST_AUTO_FUNCTION is not " \
  "parenthesized: " BOOST_PP_STRINGIZE( line )                                 \
, BOOST_PP_CAT                                                                 \
  ( BOOST_PP_CAT                                                               \
    ( ARGUMENT_                                                                \
    , BOOST_PP_SEQ_ELEM( 0, data )                                             \
    )                                                                          \
  , _OF_BOOST_AUTO_FUNCTION_IS_NOT_PARENTHESIZED                               \
  )                                                                            \
)

// Error if an argument's argument doesn't start with parentheses
// This is used with try and auto try since they are required to take
// preprocessor sequences
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_ARG_ARG_NOT_PAREN( id )               \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "A BOOST_AUTO_FUNCTION \""                                                   \
  BOOST_PP_STRINGIZE( BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID( id ) )             \
  "\" argument should be a Boost.Preprocessor sequence of expressions"         \
, BOOST_PP_CAT                                                                 \
  ( BOOST_PP_CAT( A_BOOST_AUTO_FUNCTION_                                       \
                , BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID( id )                     \
                )                                                              \
  , _ARGUMENT_SHOULD_BE_A_BOOST_PREPROCESSOR_SEQUENCE                          \
  )                                                                            \
)

// Error if the function name and parameters are invalid
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INVALID_NAME_AND_PARAMS()             \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "Invalid function name and parameters passed as the first argument to "      \
  "BOOST_AUTO_FUNCTION",                                                       \
INVALID_FUNCTION_NAME_AND_PARAMS_PASSED_AS_THE_FIRST_ARG_TO_BOOST_AUTO_FUNCTION\
)

// Error when a parameter kind isn't supported
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_NO_PARAM_SUPPORT( id, argnum )        \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_AUTO_FUNCTION \""                                                     \
  BOOST_PP_STRINGIZE( BOOST_AUTO_FUNCTION_DETAIL_SPACED_ID( id ) )             \
  "\" parameters are not supported with this compiler"                         \
, BOOST_PP_CAT                                                                 \
  ( BOOST_PP_CAT( BOOST_AUTO_FUNCTION_                                         \
                , BOOST_AUTO_FUNCTION_DETAIL_CAPS_ID( id )                     \
                )                                                              \
  , _PARAMETERS_ARE_NOT_SUPPORTED_WITH_THIS_COMPILER                           \
  )                                                                            \
)

// Error if "auto explicit" is used without a function body
#define BOOST_AUTO_FUNCTION_DETAIL_AUTO_EXPLICIT_WITHOUT_BODY( arg_num, line ) \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "Cannot use \"auto explicit\" without a \"return\" or \"do\""                \
, CANNOT_USE_AUTO_EXPLICIT_WITHOUT_A_RETURN_OR_DO                              \
)

// Error when multiple arguments of a particular category are encountered
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_OF_CATEGORY(cat,argnum,first)\
BOOST_PP_CAT( BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_, cat )( argnum, first )

// Error if an unrecognized parameter ID is encountered
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_UNRECOGNIZED_PARAM_ID( arg_num, line )\
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "Unrecognized parameter ID used with argument #"                             \
  BOOST_PP_STRINGIZE( arg_num )                                                \
  " of BOOST_AUTO_FUNCTION: " BOOST_PP_STRINGIZE( line )                       \
, BOOST_PP_CAT                                                                 \
  ( BOOST_PP_CAT                                                               \
    ( UNRECOGNIZED_PARAMETER_ID_USED_WITH_ARGUMENT_                            \
    , BOOST_PP_SEQ_ELEM( 0, data )                                             \
    )                                                                          \
  , _OF_BOOST_AUTO_FUNCTION                                                    \
  )                                                                            \
)

// Error if break is specified without an explicit result type
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_END_WITHOUT_RESULT_TYPE()             \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "Cannot end a BOOST_AUTO_FUNCTION without a deducible result type "          \
  "(provide a function body with \"return\" or \"do\", or specify a result "   \
  "type with \"explicit\")"                                                    \
, CANNOT_END_A_BOOST_AUTO_FUNCTION_WITHOUT_A_DEDUCIBLE_RESULT_TYPE             \
)

// Error when multiple explicit result types are specified
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_explicit( arg_number, first )\
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "More than one \"explicit\" or \"virtual explicit\" specified for "          \
  "BOOST_AUTO_FUNCTION (argument #" BOOST_PP_STRINGIZE( arg_number )           \
  " conflicts with argument #" BOOST_PP_STRINGIZE( first ) ")"                 \
, MORE_THAN_ONE_EXPLICIT_OR_VIRTUAL_EXPLICIT_WITH_BOOST_AUTO_FUNCTION          \
)

// Error when multiple if[_not] are specified
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_if( arg_number, first )      \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "More than one \"if\" or \"not\" specified for BOOST_AUTO_FUNCTION (argument"\
  " #" BOOST_PP_STRINGIZE( arg_number ) " conflicts with argument # "          \
  BOOST_PP_STRINGIZE( arg_number ) "): To require multiple conditions, combine"\
  " them together with && or ::boost::mpl::and_"                               \
, MORE_THAN_ONE_IF_OR_NOT_WITH_BOOST_AUTO_FUNCTION                             \
)

// Error when multiple try are specified
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_try( arg_number, first )     \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "More than one \"try\" specified for BOOST_AUTO_FUNCTION: (argument #"       \
  BOOST_PP_STRINGIZE( arg_number ) " conflicts with argument #"                \
  BOOST_PP_STRINGIZE( first ) "): To require multiple expressions, use a "     \
  "Boost.Preprocessor sequence"                                                \
, MORE_THAN_ONE_TRY_WITH_BOOST_AUTO_FUNCTION                                   \
)

// Error when multiple try are specified
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_auto_try( arg_number, first )\
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "More than one \"auto try\" specified for BOOST_AUTO_FUNCTION: (argument #"  \
  BOOST_PP_STRINGIZE( arg_number ) " conflicts with argument #"                \
  BOOST_PP_STRINGIZE( first ) "): To require multiple expressions, use a "     \
  "Boost.Preprocessor sequence"                                                \
, MORE_THAN_ONE_AUTO_TRY_WITH_BOOST_AUTO_FUNCTION                              \
)

// Error when multiple return type requirements are specified
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_auto_if(arg_number,first)\
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "More than one \"auto if\" or \"auto not\" specified for "                   \
  "BOOST_AUTO_FUNCTION: (argument #" BOOST_PP_STRINGIZE( arg_number )          \
  " conflicts with argument #" BOOST_PP_STRINGIZE( first ) "): To require "    \
  "multiple conditions for the return type, combine them together with "       \
  " ::boost::mpl::and_"                                                        \
, MORE_THAN_ONE_AUTO_IF_OR_AUTO_NOT_WITH_BOOST_AUTO_FUNCTION                   \
)

// Error when multiple function bodies requirements are specified
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_do( arg_number, first )      \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "More than one function body specified for BOOST_AUTO_FUNCTION (argument #"  \
  BOOST_PP_STRINGIZE( arg_number ) " conflicts with argument #"                \
  BOOST_PP_STRINGIZE( first ) ")"                                              \
, MORE_THAN_ONE_FUNCTION_BODY_SPECIFIED_FOR_BOOST_AUTO_FUNCTION                \
)

#else // Else, BOOST_AUTO_FUNCTION is not supported

// Start a series of checks for why BOOST_AUTO_FUNCTION isn't supported
#ifdef BOOST_NO_VARIADIC_MACROS

#define BOOST_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                          \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_AUTO_FUNCTION requires variadic macro support from the compiler"      \
, BOOST_AUTO_FUNCTION_REQUIRES_VARIADIC_MACRO_SUPPORT_FROM_THE_COMPILER        \
)

#elif defined( BOOST_NO_TRAILING_RETURN_TYPE )

#define BOOST_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                          \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_AUTO_FUNCTION requires trailing return type support from the compiler"\
, BOOST_AUTO_FUNCTION_REQUIRES_TRAILING_RETURN_TYPE_SUPPORT_FROM_THE_COMPILER  \
)

#elif defined( BOOST_NO_DECLTYPE )

#define BOOST_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                          \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_AUTO_FUNCTION requires decltype support from the compiler"            \
, BOOST_AUTO_FUNCTION_REQUIRES_DECLTYPE_SUPPORT_FROM_THE_COMPILER              \
)

#else // Otherwise, we don't have a specific message for why it's not supported

#define BOOST_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                          \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_AUTO_FUNCTION is not supported on this compiler"                      \
, BOOST_AUTO_FUNCTION_IS_NOT_SUPPORTED_ON_THIS_COMPILER                        \
)

#endif // End checks for why BOOST_AUTO_FUNCTION isn't supported

#endif // End check for BOOST_AUTO_FUNCTION support

// If BOOST_BASIC_AUTO_FUNCTION is not supported
#ifdef BOOST_NO_BOOST_BASIC_AUTO_FUNCTION

// Start a series of checks for why BOOST_BASIC_AUTO_FUNCTION isn't supported
#ifdef BOOST_NO_VARIADIC_MACROS

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                    \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_BASIC_AUTO_FUNCTION requires variadic macro support from the compiler"\
, BOOST_BASIC_AUTO_FUNCTION_REQUIRES_VARIADIC_MACRO_SUPPORT_FROM_THE_COMPILER  \
)

#elif defined( BOOST_NO_TRAILING_RETURN_TYPE )

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                    \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_BASIC_AUTO_FUNCTION requires trailing return type support from the "  \
  "compiler"                                                                   \
,    BOOST_BASIC_AUTO_FUNCTION_REQUIRES_TRAILING_RETURN_TYPE_SUPPORT_FROM_THE_ \
  ## COMPILER                                                                  \
)

#elif defined( BOOST_NO_DECLTYPE )

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                    \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_BASIC_AUTO_FUNCTION requires decltype support from the compiler"      \
, BOOST_BASIC_AUTO_FUNCTION_REQUIRES_DECLTYPE_SUPPORT_FROM_THE_COMPILER        \
)

#else // Otherwise, we don't have a specific message for why it's not supported

#if BOOST_NO_BOOST_AUTO_FUNCTION_PARAM( do )

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                    \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_BASIC_AUTO_FUNCTION requires \"do\" parameter support with "          \
  "BOOST_AUTO_FUNCTION"                                                        \
,    BOOST_BASIC_AUTO_FUNCTION_REQUIRES_DO_PARAMETER_SUPPORT_WITH_BOOST_AUTO_  \
  ## FUNCTION                                                                  \
)

#else // Otherwise, we don't have a specific message for why it's not supported

#define BOOST_BASIC_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR()                    \
BOOST_AUTO_FUNCTION_DETAIL_ERROR                                               \
( "BOOST_BASIC_AUTO_FUNCTION is not supported on this compiler"                \
, BOOST_BASIC_AUTO_FUNCTION_IS_NOT_SUPPORTED_ON_THIS_COMPILER                  \
)

#endif // End check for do parameter support

#endif // End checks for why BOOST_BASIC_AUTO_FUNCTION is not supported

#endif // End check for BOOST_BASIC_AUTO_FUNCTION support

#endif // BOOST_AUTO_FUNCTION_DETAIL_ERROR_HPP
