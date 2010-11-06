/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_HPP

#include <boost/auto_function/detail/error.hpp>
#include <boost/auto_function/detail/is_parenthesized.hpp>
#include <boost/auto_function/detail/is_recognized_arg.hpp>
#include <boost/auto_function/detail/is_valid_name_and_params.hpp>
#include <boost/auto_function/detail/num_args.hpp>
#include <boost/auto_function/detail/param.hpp>
#include <boost/auto_function/detail/param_id.hpp>
#include <boost/auto_function/detail/param_id_category_value.hpp>
#include <boost/auto_function/detail/param_id_implies_parentheses.hpp>
#include <boost/auto_function/support.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/array/data.hpp>
#include <boost/preprocessor/array/push_back.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/logical/bool.hpp>
#include <boost/preprocessor/logical/compl.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/replace.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

// Takes in the parameters passed to BOOST_AUTO_FUNCTION as a sequence. The
// sequence should not include the initial argument of function name and params.
// It yields a sequence of 3 element tuples of an argument number of the error
// message as a string, and an identifier-format error message
// ToDo: Handle no errors
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO( ... )                           \
BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_IMPL                                     \
( BOOST_PP_TUPLE_TO_SEQ( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ )    \
                       , ( __VA_ARGS__ )                                       \
                       )                                                       \
)

// Converts a preprocessor array to a sequence
#define BOOST_AUTO_FUNCTION_DETAIL_ARRAY_TO_SEQ( result_as_array )             \
BOOST_PP_TUPLE_TO_SEQ                                                          \
( BOOST_PP_ARRAY_SIZE( result_as_array )                                       \
, BOOST_PP_ARRAY_DATA( result_as_array )                                       \
)

// Check the first argument (the function name and parameters) for errors
#define BOOST_AUTO_FUNCTION_DETAIL_CHECK_FIRST_ARG( first_arg )                \
BOOST_AUTO_FUNCTION_DETAIL_CHECK_FIRST_ARG_IMPL                                \
( BOOST_AUTO_FUNCTION_DETAIL_IS_VALID_NAME_AND_PARAMS_INFO( first_arg ) )

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_CATEGORY_BIT_TABLE_EMPTY()            \
BOOST_PP_REPEAT                                                                \
( BOOST_AUTO_FUNCTION_DETAIL_NUM_PARAM_CATEGORIES                              \
, BOOST_AUTO_FUNCTION_DETAIL_PARAM_CATEGORY_BIT_TABLE_EMPTY_MACRO              \
, ~                                                                            \
)

#define BOOST_AUTO_FUNCTION_DETAIL_PARAM_CATEGORY_BIT_TABLE_EMPTY_MACRO(z,n,t) \
((0))

#define BOOST_AUTO_FUNCTION_DETAIL_CHECK_FIRST_ARG_IMPL( first_arg_errors )    \
( BOOST_PP_SEQ_ELEM( 0, first_arg_errors ) )( 2 )                              \
  ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_CATEGORY_BIT_TABLE_EMPTY() )              \
( BOOST_PP_SEQ_ELEM( 1, first_arg_errors ) )

// Same as above only it yields an array instead of a sequence
// State is a sequence of:
//   A bit specifying if a fatal error was encountered
//     (in which case we don't try to parse any more arguments)
//
//   A value specifying the current argument number
//     (starts at 2 since error handling for the first arg is handled elsewhere)
//
//   A sequence of sequences of category info where the first element is a
//     bit that specifies whether or not the category was encountered. If and
//     only if the first element is 1, there is a second element which
//     specifies the line number where the first occurrence of the category
//     appeared
//
//   An array of errors
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_IMPL( args_as_seq )              \
BOOST_PP_SEQ_ELEM                                                              \
( 3, BOOST_AUTO_FUNCTION_DETAIL_CHECK_FOR_VALID_END                            \
     ( BOOST_PP_SEQ_FOLD_LEFT                                                  \
       ( BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO                           \
       , BOOST_AUTO_FUNCTION_DETAIL_CHECK_FIRST_ARG                            \
         ( BOOST_PP_SEQ_HEAD( args_as_seq ) )                                  \
       , BOOST_PP_SEQ_TAIL( args_as_seq )                                      \
       )                                                                       \
     )                                                                         \
)

#define BOOST_AUTO_FUNCTION_DETAIL_CHECK_FOR_VALID_END( error_info )           \
BOOST_PP_IIF( BOOST_PP_SEQ_ELEM( 0, error_info )                               \
            , BOOST_AUTO_FUNCTION_DETAIL_FORWARD_FATAL_ERROR                   \
            , BOOST_AUTO_FUNCTION_DETAIL_CHECK_FOR_VALID_END_IMPL              \
            )( error_info, ~ )

#define BOOST_AUTO_FUNCTION_DETAIL_CHECK_FOR_VALID_END_IMPL( error_info,dummy )\
( BOOST_PP_SEQ_ELEM( 0, error_info ) )                                         \
( BOOST_PP_INC( BOOST_PP_SEQ_ELEM( 1, error_info ) ) )                         \
( BOOST_PP_SEQ_ELEM( 2, error_info ) )                                         \
( BOOST_PP_IIF                                                                 \
  ( BOOST_PP_BITOR                                                             \
    ( BOOST_PP_SEQ_HEAD                                                        \
      ( BOOST_PP_SEQ_ELEM                                                      \
        ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( ( do ) )         \
        , BOOST_PP_SEQ_ELEM( 2, error_info )                                   \
        )                                                                      \
      )                                                                        \
    , BOOST_PP_SEQ_HEAD                                                        \
      ( BOOST_PP_SEQ_ELEM                                                      \
        ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( ( explicit ) )   \
        , BOOST_PP_SEQ_ELEM( 2, error_info )                                   \
        )                                                                      \
      )                                                                        \
    )                                                                          \
  , BOOST_PP_SEQ_ELEM( 3, error_info )                                         \
  , BOOST_PP_ARRAY_PUSH_BACK                                                   \
    ( BOOST_PP_SEQ_ELEM( 3, error_info )                                       \
    , BOOST_AUTO_FUNCTION_DETAIL_ERROR_END_WITHOUT_RESULT_TYPE()               \
    )                                                                          \
  )                                                                            \
)

// Invoked when no error is found (updates line counter and categories)
#define BOOST_AUTO_FUNCTION_DETAIL_NO_ERROR_WITH_ARGUMENT( data, elem )        \
( BOOST_PP_SEQ_ELEM( 0, data ) )                                               \
( BOOST_PP_INC( BOOST_PP_SEQ_ELEM( 1, data ) ) )                               \
( BOOST_PP_SEQ_REPLACE                                                         \
  ( BOOST_PP_SEQ_ELEM( 2, data )                                               \
  , BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( elem )                 \
  , ( 1 )( BOOST_PP_SEQ_ELEM( 1, data ) )                                      \
  )                                                                            \
)                                                                              \
( BOOST_PP_SEQ_ELEM( 3, data ) )

// Update the "fold" state when we hit an argument that should take a sequence
// but it is detected that the argument does not have parentheses
#define BOOST_AUTO_FUNCTION_DETAIL_ARG_ARG_NOT_PAREN_INFO( data, elem )        \
( BOOST_PP_SEQ_ELEM( 0, data ) )                                               \
( BOOST_PP_INC( BOOST_PP_SEQ_ELEM( 1, data ) ) )                               \
( BOOST_PP_SEQ_ELEM( 2, data ) )                                               \
( BOOST_PP_ARRAY_PUSH_BACK                                                     \
  ( BOOST_PP_SEQ_ELEM( 3, data )                                               \
  , BOOST_AUTO_FUNCTION_DETAIL_ERROR_ARG_ARG_NOT_PAREN                         \
    ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( elem ) )                            \
  )                                                                            \
)

// Update the "fold" state when we hit an argument that duplicates a
// category that was already encountered
#define BOOST_AUTO_FUNCTION_DETAIL_DUPLICATE_CATEGORY_INFO( data, elem )       \
( BOOST_PP_SEQ_ELEM( 0, data ) )                                               \
( BOOST_PP_INC( BOOST_PP_SEQ_ELEM( 1, data ) ) )                               \
( BOOST_PP_SEQ_ELEM( 2, data ) )                                               \
( BOOST_PP_ARRAY_PUSH_BACK                                                     \
  ( BOOST_PP_SEQ_ELEM( 3, data )                                               \
  , BOOST_AUTO_FUNCTION_DETAIL_ERROR_MULTIPLE_OF_CATEGORY                      \
    ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY( elem )                     \
    , BOOST_PP_SEQ_ELEM( 1, data )                                             \
    , BOOST_PP_SEQ_ELEM                                                        \
      ( 1                                                                      \
      , BOOST_PP_SEQ_ELEM                                                      \
        ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( elem )           \
        , BOOST_PP_SEQ_ELEM( 2, data )                                         \
        )                                                                      \
      )                                                                        \
    )                                                                          \
  )                                                                            \
)

// Update the "fold" state when we hit an argument that wasn't parenthesized
// An argument that isn't parenthesized is a fatal error and we can't parse
// anything after that since we can't know whether or not commas there are
// intended to end that argument or if they are supposed to be a part of that
// argument
#define BOOST_AUTO_FUNCTION_DETAIL_NOT_PARENTHESIZED( data, elem )             \
( 1 )                                                                          \
( ~ )                                                                          \
( ~ )                                                                          \
( BOOST_PP_ARRAY_PUSH_BACK                                                     \
  ( BOOST_PP_SEQ_ELEM( 3, data )                                               \
  , BOOST_AUTO_FUNCTION_DETAIL_ERROR_NOT_PARENTHESIZED                         \
    ( BOOST_PP_SEQ_ELEM( 1, data ), elem )                                     \
  )                                                                            \
)

// Update the "fold" state when we hit an unrecognized param id
#define BOOST_AUTO_FUNCTION_DETAIL_UNRECOGNIZED_ARG( data, elem )              \
( BOOST_PP_SEQ_ELEM( 0, data ) )                                               \
( BOOST_PP_INC( BOOST_PP_SEQ_ELEM( 1, data ) ) )                               \
( BOOST_PP_SEQ_ELEM( 2, data ) )                                               \
( BOOST_PP_ARRAY_PUSH_BACK                                                     \
  ( BOOST_PP_SEQ_ELEM( 3, data )                                               \
  , BOOST_AUTO_FUNCTION_DETAIL_ERROR_UNRECOGNIZED_PARAM_ID                     \
    ( BOOST_PP_SEQ_ELEM( 1, data ), elem )                                     \
  )                                                                            \
)

// On a fatal error don't bother trying to parse any more arguments
#define BOOST_AUTO_FUNCTION_DETAIL_FORWARD_FATAL_ERROR( data, elem ) data

// Determines if the category of this argument has already appeared
#define BOOST_AUTO_FUNCTION_DETAIL_IS_DUPLICATE_CATEGORY( data, elem )         \
BOOST_PP_SEQ_ELEM                                                              \
( 0                                                                            \
, BOOST_PP_SEQ_ELEM( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( elem )\
                   , BOOST_PP_SEQ_ELEM( 2, data )                              \
                   )                                                           \
)

// Update the "fold" state when we hit a parameter kind that we can't support
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_NO_SUPPORT_INFO( data, elem )         \
( BOOST_PP_SEQ_ELEM( 0, data ) )                                               \
( BOOST_PP_INC( BOOST_PP_SEQ_ELEM( 1, data ) ) )                               \
( BOOST_PP_SEQ_ELEM( 2, data ) )                                               \
( BOOST_PP_ARRAY_PUSH_BACK                                                     \
  ( BOOST_PP_SEQ_ELEM( 3, data )                                               \
  , BOOST_AUTO_FUNCTION_DETAIL_ERROR_NO_PARAM_SUPPORT                          \
    ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( elem )                              \
    , BOOST_PP_SEQ_ELEM( 1, data )                                             \
    )                                                                          \
  )                                                                            \
)

// Checks starting after we have recognized the parameter kind
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO_RECOGNIZED( data, elem )   \
BOOST_PP_IIF                                                                   \
( BOOST_NO_BOOST_AUTO_FUNCTION_PARAM                                           \
  ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( elem ) )                              \
, BOOST_AUTO_FUNCTION_DETAIL_ERROR_NO_SUPPORT_INFO                             \
, BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO_SUPPORTED                        \
)( data, elem )

// Checks starting after we have determined that the parameter kind is supported
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO_SUPPORTED( data, elem )    \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_IS_DUPLICATE_CATEGORY( data, elem )               \
, BOOST_AUTO_FUNCTION_DETAIL_DUPLICATE_CATEGORY_INFO                           \
, BOOST_AUTO_FUNCTION_DETAIL_ERROR_NO_DUPLICATE                                \
)( data, elem )

#define BOOST_AUTO_FUNCTION_DETAIL_LACKS_PARENT_FOR_ARG( data, elem )          \
BOOST_PP_BITAND                                                                \
( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_IMPLIES_PARENTHESES( elem )              \
, BOOST_PP_COMPL                                                               \
  ( BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED                            \
    ( BOOST_AUTO_FUNCTION_DETAIL_PARAM( elem ) )                               \
  )                                                                            \
)

// Checks starting after we have determined that there is not a duplicate
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_NO_DUPLICATE( data, elem )            \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_LACKS_PARENT_FOR_ARG( data, elem )                \
, BOOST_AUTO_FUNCTION_DETAIL_ARG_ARG_NOT_PAREN_INFO                            \
, BOOST_AUTO_FUNCTION_DETAIL_NO_ERROR_WITH_ARGUMENT                            \
)( data, elem )

// Checks starting after it is established that the argument is parenthesized
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO_PARENTHESIZED( data, elem )\
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_IS_RECOGNIZED_ARG( elem )                         \
, BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO_RECOGNIZED                       \
, BOOST_AUTO_FUNCTION_DETAIL_UNRECOGNIZED_ARG                                  \
)( data, elem )

// data is a 2 element tuple of the current argument number and an array of
// tuples of argument numbers and the associated unrecognized argument
// ToDo: Push off some of the checks to other macros to prevent errors when not
// parenthesized properly or when there are too many commas
#define BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO( s, data, elem )           \
BOOST_PP_IIF                                                                   \
( BOOST_PP_SEQ_ELEM( 0, data )                                                 \
, BOOST_AUTO_FUNCTION_DETAIL_FORWARD_FATAL_ERROR                               \
, BOOST_PP_IIF                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_STARTS_PARENTHESIZED( elem )                    \
  , BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_MACRO_PARENTHESIZED                  \
  , BOOST_AUTO_FUNCTION_DETAIL_NOT_PARENTHESIZED                               \
  )                                                                            \
)( data, elem )

// Yields 1 if there were errors, otherwise it yields 0
#define BOOST_AUTO_FUNCTION_DETAIL_HAS_ERRORS( error_array )                   \
BOOST_PP_BOOL( BOOST_PP_ARRAY_SIZE( error_array ) )

// Yields all errors as statements
#define BOOST_AUTO_FUNCTION_DETAIL_EMIT_ERRORS( error_array )                  \
BOOST_PP_IF( BOOST_PP_ARRAY_SIZE( error_array )                                \
           , BOOST_AUTO_FUNCTION_DETAIL_EMIT_ERRORS_IMPL                       \
           , BOOST_AUTO_FUNCTION_DETAIL_EMIT_NO_ERRORS_IMPL                    \
           )( error_array )

// Do nothing if there are no errors to emit
#define BOOST_AUTO_FUNCTION_DETAIL_EMIT_NO_ERRORS_IMPL( error_array )

// Yields all errors as statements (only valid if array size is greater than 1)
#define BOOST_AUTO_FUNCTION_DETAIL_EMIT_ERRORS_IMPL( error_array )             \
void BOOST_PP_CAT( failed_auto_function_on_line_, __LINE__ )();                \
BOOST_PP_SEQ_FOR_EACH                                                          \
( BOOST_AUTO_FUNCTION_DETAIL_EMIT_ERROR_IMPL, ~                                \
, BOOST_AUTO_FUNCTION_DETAIL_ARRAY_TO_SEQ( error_array )                       \
)

// Yields an error as a statement
#define BOOST_AUTO_FUNCTION_DETAIL_EMIT_ERROR_IMPL( r, dummy, error ) error;

#endif // BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO_HPP
