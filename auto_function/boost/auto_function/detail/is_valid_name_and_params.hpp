/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_HPP

#include <boost/auto_function/detail/error.hpp>
#include <boost/auto_function/detail/is_parenthesized.hpp>
#include <boost/auto_function/detail/num_args.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/logical/bitand.hpp>

// If this is the case, we can't do any more checking at this point, so don't
// yield an error
#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_NOT_COMMA( arg ) 1

// If we have more than one comma then this can't possibly be a valid function
// header (since conversion operators can't be supported by BOOST_AUTO_FUNCTION)
#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_TOO_MANY_ARGS(...) 0

// Check if the first parameter is "operator"
// ToDo: If you are experiencing problems when operator is passed, this may need
// to be changed to force an expansion before passing the args to "NUM_ARGS"
#define BOOST_AUTO_FUNCTION_DETAIL_IS_OPERATOR( name )                         \
BOOST_PP_EQUAL                                                                 \
( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS                                          \
  ( BOOST_AUTO_FUNCTION_DETAIL_IS_OPERATOR_ ## name )                          \
, 2                                                                            \
)

// Yields 2 tokens separate by a comma
#define BOOST_AUTO_FUNCTION_DETAIL_IS_OPERATOR_operator _0, _1

// If it's 2 arguments, we need to check that the first argument is "operator"
// and the that second parameter starts with something that's parenthesized
#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_COMMA(name,params) \
BOOST_PP_BITAND                                                                \
( BOOST_AUTO_FUNCTION_DETAIL_IS_OPERATOR( name )                               \
, BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED( params )                    \
)

// Note: argument must have the first set of parenthesis stripped
#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS(name_params_paren) \
BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_IMPL name_params_paren

#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_IMPL( ... )        \
BOOST_PP_IIF                                                                   \
( BOOST_PP_EQUAL( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ ), 1 )      \
, BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_NOT_COMMA                \
, BOOST_PP_IIF                                                                 \
  ( BOOST_PP_EQUAL( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ ), 2 )    \
  , BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_COMMA                  \
  , BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_TOO_MANY_ARGS          \
  )                                                                            \
)( __VA_ARGS__ )

// Yields a 2 element sequence of a bit specified as 1 if there's a fatal error
// with the second element of the sequence being an array of errors.
#define BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_INFO( arg )        \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED( arg )                       \
, BOOST_PP_IIF                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS( arg )                 \
  , (0)((0,()))                                                                \
  , (0)((1,( BOOST_AUTO_FUNCTION_DETAIL_ERROR_INVALID_NAME_AND_PARAMS() )))    \
  )                                                                            \
, (1)                                                                          \
  (( 1                                                                         \
   ,( BOOST_AUTO_FUNCTION_DETAIL_ERROR_NOT_PARENTHESIZED( 1, arg ) )           \
   )                                                                           \
  )                                                                            \
)

#endif // BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_HPP
