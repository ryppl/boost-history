/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_HPP

#include <boost/auto_function/detail/head_argument.hpp>
#include <boost/auto_function/detail/variadic_cat.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/empty.hpp>

#define BOOST_AUTO_FUNCTION_DETAIL_EAT_AUTO_auto

#define BOOST_AUTO_FUNCTION_DETAIL_EAT_VIRTUAL_virtual

// Macros that combine "auto" with its secondary param ID and separates
// the param ID from the parameter by a comma
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_AUTO_PARAM_ID_AND_PARAM_explicit      \
auto_explicit,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_AUTO_PARAM_ID_AND_PARAM_if            \
auto_if,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_AUTO_PARAM_ID_AND_PARAM_not           \
auto_not,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_AUTO_PARAM_ID_AND_PARAM_try           \
auto_try,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_VIRTUAL_PARAM_ID_AND_PARAM_explicit   \
virtual_explicit,

// Macros that separate 1 word param IDs from the parameter by a comma
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_do do,
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_explicit explicit,
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_if if,
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_not not,
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_return return,
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_return_ref         \
return_ref,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_return_rvalue_ref  \
return_rvalue_ref,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_return_value       \
return_value,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_void               \
void,

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_try try,

// Macros that determine if auto is the first word of a param ID
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_auto              1,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_do                0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_explicit          0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_if                0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_not               0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_return            0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_return_ref        0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_return_rvalue_ref 0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_return_value      0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_try               0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_virtual           0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_void              0,

// Macros that determine if virtual is the first word of a param ID
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_auto              0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_do                0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_explicit          0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_if                0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_not               0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_return            0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_return_ref        0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_return_rvalue_ref 0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_return_value      0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_try               0,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_virtual           1,
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_void              0,

// Combines "auto" with the secondary param ID and separates the full ID
// from the parameter by a comma
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_AUTO_IMPL(...)     \
BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT                                        \
( BOOST_AUTO_FUNCTION_DETAIL_SPLIT_AUTO_PARAM_ID_AND_PARAM_                    \
, BOOST_AUTO_FUNCTION_DETAIL_EAT_AUTO_ ## __VA_ARGS__                          \
)

// Combines "virtual" with the secondary param ID and separates the full ID
// from the parameter by a comma
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_VIRTUAL_IMPL(...)  \
BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT                                        \
( BOOST_AUTO_FUNCTION_DETAIL_SPLIT_VIRTUAL_PARAM_ID_AND_PARAM_                 \
, BOOST_AUTO_FUNCTION_DETAIL_EAT_VIRTUAL_ ## __VA_ARGS__                       \
)

// Yields 1 if the parameter id starts with "auto"
// otherwise yields 0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO( ... )                    \
BOOST_AUTO_FUNCTION_DETAIL_HEAD_ARGUMENT_AFTER_RESCAN                          \
( BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT                                      \
  ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO_                              \
  , __VA_ARGS__                                                                \
  )                                                                            \
)

// Yields 1 if the parameter id starts with "virtual"
// otherwise yields 0
#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL( ... )                 \
BOOST_AUTO_FUNCTION_DETAIL_HEAD_ARGUMENT_AFTER_RESCAN                          \
( BOOST_AUTO_FUNCTION_DETAIL_VARIADIC_CAT                                      \
  ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL_                           \
  , __VA_ARGS__                                                                \
  )                                                                            \
)

// Separates a param id from the parameter by a comma
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_IMPL( ... )        \
BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_ ## __VA_ARGS__

// Separates a param id from the parameter by a comma
// If the parameter ID is multiple words, it combines them and replaces spaces
// with underscores
#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM( arg_in_paren )    \
BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_IML arg_in_paren

#define BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_IML( ... )         \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_AUTO( __VA_ARGS__ )                  \
, BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_AUTO_IMPL                \
, BOOST_PP_IIF                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_HAS_VIRTUAL( __VA_ARGS__ )             \
  , BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_VIRTUAL_IMPL           \
  , BOOST_AUTO_FUNCTION_DETAIL_SPLIT_PARAM_ID_AND_PARAM_IMPL                   \
  )                                                                            \
)( __VA_ARGS__ BOOST_PP_EMPTY )

#endif //BOOST_AUTO_FUNCTION_DETAIL_SPLIT_SPLIT_PARAM_ID_AND_PARAM_AND_PARAM_HPP
