/*==============================================================================
    Copyright (c) 2010 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_HPP
#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_HPP

#include <boost/auto_function/detail/param.hpp>
#include <boost/auto_function/detail/param_id.hpp>
#include <boost/auto_function/detail/param_id_category_value.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>

// Yields 1 if an argument of the given category was passed
#define BOOST_AUTO_FUNCTION_DETAIL_HAS_ARG_OF_CATEGORY( param_category         \
                                                      , ...                    \
                                                      )                        \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_SEQ_SIZE                                                            \
  ( BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_IMPL( param_category            \
                                                   , __VA_ARGS__               \
                                                   )                           \
  )                                                                            \
, 2                                                                            \
)

// Yields a parenthesized version of the parameter passed, without the ID
#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY( param_category             \
                                                  , ...                        \
                                                  )                            \
BOOST_PP_SEQ_ELEM                                                              \
( 1, BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_IMPL( param_category           \
                                                    , __VA_ARGS__              \
                                                    )                          \
)

// Yields the parameter ID of the argument that matches the desired category
#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_PARAM_ID( param_category    \
                                                           , ...               \
                                                           )                   \
BOOST_PP_SEQ_HEAD                                                              \
( BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_IMPL( param_category              \
                                                 , __VA_ARGS__                 \
                                                 )                             \
)

// Yields a 2 element sequence of the form (param_id)((param))
#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_IMPL( param_category        \
                                                       , ...                   \
                                                       )                       \
BOOST_PP_SEQ_FOLD_LEFT                                                         \
( BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_MACRO                             \
, ( param_category )                                                           \
, BOOST_PP_TUPLE_TO_SEQ( BOOST_AUTO_FUNCTION_DETAIL_NUM_ARGS( __VA_ARGS__ )    \
                       , ( __VA_ARGS__ )                                       \
                       )                                                       \
)

#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_NOT_YET_FOUND( state, elem )\
BOOST_PP_IIF                                                                   \
( BOOST_PP_EQUAL                                                               \
  ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( state )                \
  , BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID_CATEGORY_VALUE( elem )                 \
  )                                                                            \
, ( BOOST_AUTO_FUNCTION_DETAIL_PARAM_ID( elem ) )                              \
  ( ( BOOST_AUTO_FUNCTION_DETAIL_PARAM( elem ) ) )                             \
, state                                                                        \
)

#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_FOUND( state, elem ) state

#define BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_MACRO( s, state, elem )     \
BOOST_PP_IIF                                                                   \
( BOOST_PP_EQUAL( BOOST_PP_SEQ_SIZE( state ), 2 )                              \
, BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_FOUND                             \
, BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_NOT_YET_FOUND                     \
)( state, elem )

#endif // BOOST_AUTO_FUNCTION_DETAIL_ARG_OF_CATEGORY_HPP
