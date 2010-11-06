/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_AUTO_FUNCTION_HPP
#define BOOST_AUTO_FUNCTION_AUTO_FUNCTION_HPP

#include <boost/auto_function/detail/error.hpp>
#include <boost/auto_function/support.hpp>
#include <boost/config.hpp>

#ifndef BOOST_NO_BOOST_AUTO_FUNCTION

#include <boost/auto_function/detail/arg_of_category.hpp>
#include <boost/auto_function/detail/error_info.hpp>
#include <boost/auto_function/detail/is_conditional_arg.hpp>
#include <boost/auto_function/detail/num_args.hpp>
#include <boost/auto_function/detail/param_id_category.hpp>
#include <boost/auto_function/detail/param_templates.hpp>
#include <boost/auto_function/detail/rescan.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

// ToDo: Possibly make auto constructor macro

// Requires at least 2 parameters
#define BOOST_AUTO_FUNCTION( function_name_and_params_in_parentheses, ... )    \
BOOST_AUTO_FUNCTION_DETAIL_IMPL                                                \
( BOOST_AUTO_FUNCTION_DETAIL_ERROR_INFO                                        \
  ( function_name_and_params_in_parentheses, __VA_ARGS__ )                     \
, function_name_and_params_in_parentheses, __VA_ARGS__                         \
)

// This macro is variadic in order to directly support operator,
#define BOOST_AUTO_FUNCTION_DETAIL_IMPL( errors, ... )                         \
BOOST_AUTO_FUNCTION_DETAIL_EMIT_ERRORS( errors )                               \
BOOST_PP_IIF( BOOST_AUTO_FUNCTION_DETAIL_HAS_ERRORS( errors )                  \
            , BOOST_AUTO_FUNCTION_DETAIL_IMPL_ON_ERROR                         \
            , BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS                        \
            )( __VA_ARGS__ )

// When he have an error that's detected during preprocessing don't do anything
#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_ON_ERROR( ... )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_FUN_NAME_AND_PARAMS( ... )             \
auto __VA_ARGS__ ->

#define BOOST_AUTO_FUNCTION_DETAIL_UNCHECKED_RETURN_TYPE( ... )                \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_HAS_ARG_OF_CATEGORY( explicit, __VA_ARGS__ )      \
, BOOST_PP_IIF                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_HAS_ARG_OF_CATEGORY( do, __VA_ARGS__ )          \
  , BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_UNCHECKED_EXPLICIT_DO            \
  , BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_UNCHECKED_EXPLICIT               \
  )                                                                            \
, BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_UNCHECKED_DO                       \
)( __VA_ARGS__ )

// Branch out to different implementations depending on what's present
#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS( fun_name_and_params, ... )  \
BOOST_AUTO_FUNCTION_DETAIL_IMPL_FUN_NAME_AND_PARAMS fun_name_and_params        \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_HAS_ARG_OF_CATEGORY( explicit, __VA_ARGS__ )      \
, BOOST_PP_IIF                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_HAS_ARG_OF_CATEGORY( do, __VA_ARGS__ )          \
  , BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_EXPLICIT_AND_DO                  \
  , BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_EXPLICIT                         \
  )                                                                            \
, BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_DO                                 \
)( __VA_ARGS__ )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE( ... )                   \
BOOST_AUTO_FUNCTION_DETAIL_RESCAN                                              \
( BOOST_PP_CAT                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE_                             \
  , BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_PARAM_ID( explicit              \
                                                       , __VA_ARGS__           \
                                                       )                       \
  )                                                                            \
  BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY( explicit, __VA_ARGS__ )          \
)

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE_auto_explicit(return_type\
                                                                   , ...       \
                                                                   )           \
decltype                                                                       \
( ::boost::auto_function::detail::impl_auto_explicit_type                      \
  < return_type, __VA_ARGS__ >()                                               \
)

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE_explicit( ... )          \
decltype                                                                       \
( ::boost::auto_function::detail::impl_explicit_type< __VA_ARGS__ >() )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE_virtual_explicit( ... )  \
decltype                                                                       \
( ::boost::auto_function::detail::impl_virtual_explicit_type< __VA_ARGS__ >() )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE( ... )                         \
BOOST_AUTO_FUNCTION_DETAIL_RESCAN                                              \
( BOOST_PP_CAT                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_                                   \
  , BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_PARAM_ID( do, __VA_ARGS__ )     \
  ) BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY( do, __VA_ARGS__ )              \
)

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY( ... )                         \
BOOST_AUTO_FUNCTION_DETAIL_RESCAN                                              \
( BOOST_PP_CAT                                                                 \
  ( BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_                                   \
  , BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_PARAM_ID( do, __VA_ARGS__ )     \
  ) BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY( do, __VA_ARGS__ )              \
)

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_do( ... )                      \
decltype( [&]{ __VA_ARGS__ }() )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_return( ... )                  \
decltype( __VA_ARGS__ )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_return_ref( ... )              \
decltype( __VA_ARGS__ )&

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_return_rvalue_ref( ... )       \
decltype( __VA_ARGS__ )&&

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_return_value( ... )            \
decltype( ::boost::auto_function::detail::impl_return_value_type               \
          < decltype( __VA_ARGS__ ) >()                                        \
        )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE_void( ... )                    \
decltype( ::boost::auto_function::detail::impl_return_void_type                \
          < decltype( __VA_ARGS__ ) >()                                        \
        )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_do( ... ) __VA_ARGS__

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_return( ... )                  \
return __VA_ARGS__;

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_return_ref( ... )              \
return static_cast< decltype( __VA_ARGS__ )& >( __VA_ARGS__ );

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_return_rvalue_ref( ... )       \
return static_cast< decltype( __VA_ARGS__ )&& >( __VA_ARGS__ );

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_return_value( ... )            \
return __VA_ARGS__;

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY_void( ... ) __VA_ARGS__;

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_UNCHECKED_EXPLICIT( ... )    \
decltype( ::boost::auto_function::detail::impl_explicit                        \
          < BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE( __VA_ARGS__ )       \
          , void                                                               \
          >()                                                                  \
        )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_UNCHECKED_DO( ... )          \
decltype( ::boost::auto_function::detail::impl_do                              \
          < BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE( __VA_ARGS__ )             \
          , void                                                               \
          >()                                                                  \
        )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_UNCHECKED_EXPLICIT_DO( ... ) \
decltype( ::boost::auto_function::detail::impl_explicit_and_do                 \
          < BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE( __VA_ARGS__ )       \
          , BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE( __VA_ARGS__ )             \
          , void                                                               \
          >()                                                                  \
        )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_EXPLICIT( ... )              \
decltype( ::boost::auto_function::detail::impl_explicit                        \
          < BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE( __VA_ARGS__ )       \
          , ::boost::auto_function::detail::types                              \
            < BOOST_AUTO_FUNCTION_DETAIL_IMPL_CONDITIONAL_ARGS                 \
              ( BOOST_AUTO_FUNCTION_DETAIL_UNCHECKED_RETURN_TYPE( __VA_ARGS__ )\
              , __VA_ARGS__                                                    \
              )                                                                \
              void                                                             \
            >                                                                  \
          >()                                                                  \
        )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_DO( ... )                    \
decltype( ::boost::auto_function::detail::impl_do                              \
          < BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE( __VA_ARGS__ )             \
          , ::boost::auto_function::detail::types                              \
            < BOOST_AUTO_FUNCTION_DETAIL_IMPL_CONDITIONAL_ARGS                 \
              ( BOOST_AUTO_FUNCTION_DETAIL_UNCHECKED_RETURN_TYPE( __VA_ARGS__ )\
              , __VA_ARGS__                                                    \
              )                                                                \
              void                                                             \
            >                                                                  \
          >()                                                                  \
        ) { BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY( __VA_ARGS__ ) }

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_NO_ERRORS_EXPLICIT_AND_DO( ... )       \
decltype( ::boost::auto_function::detail::impl_explicit_and_do                 \
          < BOOST_AUTO_FUNCTION_DETAIL_IMPL_EXPLICIT_TYPE( __VA_ARGS__ )       \
          , BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_TYPE( __VA_ARGS__ )             \
          , ::boost::auto_function::detail::types                              \
            < BOOST_AUTO_FUNCTION_DETAIL_IMPL_CONDITIONAL_ARGS                 \
              ( BOOST_AUTO_FUNCTION_DETAIL_UNCHECKED_RETURN_TYPE( __VA_ARGS__ )\
              , __VA_ARGS__                                                    \
              )                                                                \
              void                                                             \
            >                                                                  \
          >()                                                                  \
        ) { BOOST_AUTO_FUNCTION_DETAIL_IMPL_DO_BODY( __VA_ARGS__ ) }

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_CONDITIONAL_ARGS( return_type, ... )   \
BOOST_PP_SEQ_FOR_EACH                                                          \
( BOOST_AUTO_FUNCTION_DETAIL_IMPL_CONDITIONAL_ARGS_MACRO                       \
, return_type                                                                  \
, BOOST_PP_TUPLE_TO_SEQ( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ )    \
                       , ( __VA_ARGS__ )                                       \
                       )                                                       \
)

#define BOOST_AUTO_FUNCTION_DETAIL_EAT_ARGS( ... )

#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_IMPL( return_type, arg )   \
BOOST_PP_CAT( BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_                     \
            , BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( arg )                       \
            )( return_type, BOOST_AUTO_FUNCTION_DETAIL_PARAM( arg ) )

#define BOOST_AUTO_FUNCTION_DETAIL_IMPL_CONDITIONAL_ARGS_MACRO( r, return_type \
                                                              , elem           \
                                                              )                \
BOOST_PP_IIF                                                                   \
( BOOST_AUTO_FUNCTION_DETAIL_IS_CONDITIONAL_ARG( elem )                        \
, BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_IMPL                             \
, BOOST_AUTO_FUNCTION_DETAIL_EAT_ARGS                                          \
)( return_type, elem )

#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_auto_if( return_type, ... )\
decltype                                                                       \
( ::boost::auto_function::detail::perform_auto_if_check                        \
  < return_type, bool ( __VA_ARGS__ ) >()                                      \
),

#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_auto_not( return_type,... )\
decltype                                                                       \
( ::boost::auto_function::detail::perform_auto_not_check                       \
  < return_type, bool ( __VA_ARGS__ ) >()                                      \
),

// ToDo: Define (requires lambda trick)
#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_auto_try( return_type,... )

#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_if( return_type, ... )     \
decltype                                                                       \
( ::boost::auto_function::detail::                                             \
your_if_condition_isnt_dependent_on_a_function_template_argument               \
< bool ( __VA_ARGS__ ) >()                                                     \
),

#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_not( return_type, ... )    \
decltype                                                                       \
( ::boost::auto_function::detail::                                             \
your_not_condition_isnt_dependent_on_a_function_template_argument              \
< bool ( __VA_ARGS__ ) >()                                                     \
),

// ToDo: Make this a preprocess sequence for each and change the top level loop
// to be a repeat (sequence for each can't nest)
#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_try( return_type, seq )    \
BOOST_PP_REPEAT( BOOST_PP_SEQ_SIZE( seq )                                      \
               , BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_TRY_MACRO         \
               , seq                                                           \
               )

#define BOOST_AUTO_FUNCTION_DETAIL_CONDITIONAL_ARGS_TRY_MACRO( z, n, seq )     \
decltype( BOOST_PP_SEQ_ELEM( n, seq ) ),

#else // Else, BOOST_AUTO_FUNCTION is not supported

#ifndef BOOST_NO_VARIADIC_MACROS // If variadic macros are supported

// Output an error when BOOST_AUTO_FUNCTION is used when it is not supported
#define BOOST_AUTO_FUNCTION( function_name_and_params_in_parentheses, ... )    \
BOOST_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR();

#else // Else, variadic macros aren't supported

// Output an error when BOOST_AUTO_FUNCTION is used when it is not supported
#define BOOST_AUTO_FUNCTION                                                    \
BOOST_AUTO_FUNCTION_DETAIL_NO_SUPPORT_ERROR();

#endif // End check for variadic macro support

#endif // Ends check for BOOST_AUTO_FUNCTION support

#endif // BOOST_AUTO_FUNCTION_AUTO_FUNCTION_HPP
