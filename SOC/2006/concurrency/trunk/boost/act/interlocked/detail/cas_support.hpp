/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_HPP

#include <boost/act/interlocked/detail/implementation_info/operation_support.hpp>

// ToDo: Change to use appropriate default macro definitions when undefined
#ifdef BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT

#include <climits>
#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/filter.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/act/interlocked/detail/full_name.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NUM_PARAMS 5

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NAME                          \
  BOOST_PP_TUPLE_ELEM( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NUM_PARAMS     \
                     , 0, BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT       \
                     )


#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ                           \
  BOOST_PP_TUPLE_ELEM( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NUM_PARAMS     \
                     , 1, BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT       \
                     )

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_LOAD_INFO                     \
  BOOST_PP_TUPLE_ELEM( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NUM_PARAMS     \
                     , 2, BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT       \
                     )

// ToDo: Remove this eventually
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_ARITHMETIC_RETURN             \
  old_value

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_STORE_INFO                    \
  BOOST_PP_TUPLE_ELEM( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NUM_PARAMS     \
                     , 3, BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT       \
                     )

#ifndef BOOST_ACT_INTERLOCKED_FORCE_DEFAULTS

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_IMPL_INFO                     \
  BOOST_PP_TUPLE_ELEM( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_NUM_PARAMS     \
                     , 4, BOOST_ACT_INTERLOCKED_DETAIL_OPERATION_SUPPORT       \
                     )

#else

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_IMPL_INFO                     \
( ( assign,        ( acq_rel ) ) )                                          \
( ( assign_if_was, ( acq_rel ) ) )                                          \
( ( load,          ( acquire ) ) )                                             \
( ( store,         ( release ) ) )

#endif

#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_0               0
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_add_assign      1
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_assign          2
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_assign_if_was   3
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_bitand_assign   4
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_bitor_assign    5
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_compiler_fence  6
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_compl_assign    7
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_decrement       8
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_divide_assign   9
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_fence           10
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_increment       11
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_lshift_assign   12
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_mod_assign      13
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_modify          14
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_multiply_assign 15
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_negate_assign   16
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_not_assign      17
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_load            18
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_rshift_assign   19
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_store           20
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_subtract_assign 21
#define BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_xor_assign      22

#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_0               0
#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_unordered       1
#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_acquire         2
#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_read            2
#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_release         3
#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_write           3
#define BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_acq_rel      4

#define BOOST_ACT_INTERLOCKED_DETAIL_COMPARE( data, elem )                     \
BOOST_PP_EQUAL                                                                 \
(                                                                              \
  BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_, elem )              \
, data                                                                         \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_APPEND_IF_SAME( s, state, value )         \
BOOST_PP_IF                                                                    \
(                                                                              \
  BOOST_ACT_INTERLOCKED_DETAIL_COMPARE( BOOST_PP_SEQ_ELEM( 0, state ), value ) \
, state (value)                                                                \
, state                                                                        \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_SEQ_CONTAINS( s, seq, value )             \
BOOST_PP_NOT_EQUAL                                                             \
(                                                                              \
  BOOST_PP_SEQ_SIZE                                                            \
  (                                                                            \
    BOOST_PP_CAT( BOOST_PP_SEQ_FOLD_LEFT_, s )                                 \
    ( BOOST_ACT_INTERLOCKED_DETAIL_APPEND_IF_SAME                              \
    , (value)                                                                  \
    , seq                                                                      \
    )                                                                          \
  )                                                                            \
, 1                                                                            \
)

// ToDo: Change to lazily evaluate 2nd and argument
#define BOOST_ACT_INTERLOCKED_DETAIL_IS_SAME_IMPL_OR_TERM( s, data, elem )     \
BOOST_PP_BITAND                                                                \
(                                                                              \
  BOOST_PP_BITOR                                                               \
  (                                                                            \
    BOOST_PP_EQUAL                                                             \
    (                                                                          \
      BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_                  \
                  , BOOST_PP_TUPLE_ELEM( 2, 0, elem )                          \
                  )                                                            \
    , BOOST_PP_TUPLE_ELEM( 2, 0, data )                                        \
    )                                                                          \
  , BOOST_PP_EQUAL                                                             \
    (                                                                          \
      BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_                  \
                  , BOOST_PP_TUPLE_ELEM( 2, 0, elem )                          \
                  )                                                            \
    , 0                                                                        \
    )                                                                          \
  )                                                                            \
, BOOST_ACT_INTERLOCKED_DETAIL_SEQ_CONTAINS                                    \
  ( s, BOOST_PP_TUPLE_ELEM( 2, 1, elem )                                       \
     , BOOST_PP_TUPLE_ELEM( 2, 1, data )                                       \
  )                                                                            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_CUSTOM_IMPL( name, semantics )        \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_SEQ_SIZE                                                            \
  ( BOOST_PP_SEQ_FILTER                                                        \
    ( BOOST_ACT_INTERLOCKED_DETAIL_IS_SAME_IMPL_OR_TERM                        \
    , ( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_, name )        \
      , BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_SEMANTIC_ID_, semantics )   \
      )                                                                        \
    , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_IMPL_INFO                       \
      ((0,(unordered)(acquire)(release)(acq_rel)))                          \
    )                                                                          \
  )                                                                            \
, 2                                                                            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_custom_load              0
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_custom_store             0
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_load_unordered  1
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_store_unordered 1
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_load_acquire    2
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_store_release   2

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_LOAD_UNORDERED               \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_                      \
              , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_LOAD_INFO             \
              )                                                                \
, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_load_unordered             \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_LOAD_ACQUIRE                 \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_                      \
              , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_LOAD_INFO             \
              )                                                                \
, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_load_acquire               \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_STORE_UNORDERED              \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_                      \
              , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_LOAD_INFO             \
              )                                                                \
, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_store_unordered            \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_HAS_VOLATILE_STORE_RELEASE                \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_                      \
              , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_LOAD_INFO             \
              )                                                                \
, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_volatile_store_release              \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_IS_NAME( name, desired_name )             \
BOOST_PP_EQUAL                                                                 \
( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_, name )              \
, BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_FUNCTION_ID_, desired_name )      \
)

namespace boost { namespace act { namespace interlocked { namespace detail {

#define BOOST_ACT_INTERLOCKED_DETAIL_ARITHMETIC_PARAMS_BASE_MACRO(r,data,elem) \
are_valid_arithmeticn_params< ( elem / CHAR_BIT )                              \
                            , LeftOperand, RightOperand, ComparisonType        \
                            >,
#define BOOST_ACT_INTERLOCKED_DETAIL_ADDITIVE_PARAMS_BASE_MACRO(r,data,elem)   \
are_valid_additiven_params< ( elem / CHAR_BIT )                                \
                            , LeftOperand, RightOperand                        \
                            >,

// ToDo: remove_volatile for RightOperand and ComparisonType
template< typename LeftOperand, typename RightOperand = LeftOperand const
        , typename ComparisonType = LeftOperand const
        >
struct are_valid_arithmetic_params
  : mpl::or_
    <
      BOOST_PP_SEQ_FOR_EACH
      ( BOOST_ACT_INTERLOCKED_DETAIL_ARITHMETIC_PARAMS_BASE_MACRO
      , ~
      , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ
      )
      mpl::true_
    >
{
};

// ToDo: remove_volatile for RightOperand
template< typename LeftOperand, typename RightOperand = LeftOperand const >
struct are_valid_additive_params
  : mpl::or_
    <
      BOOST_PP_SEQ_FOR_EACH
      ( BOOST_ACT_INTERLOCKED_DETAIL_ADDITIVE_PARAMS_BASE_MACRO
      , ~
      , BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ
      )
      mpl::true_
    >
{
};

// ToDo: remove_volatile for RightOperand and ComparisonType
template< typename LeftOperand, typename RightOperand = LeftOperand const
        , typename ComparisonType = LeftOperand const
        >
struct are_valid_store_style_params
  : mpl::or_< are_valid_arithmetic_params< LeftOperand, RightOperand
                                         , ComparisonType
                                         >
            , are_valid_pointer_params< LeftOperand, RightOperand
                                      , ComparisonType
                                      >
            , are_valid_bool_params< LeftOperand, RightOperand
                                   , ComparisonType
                                   >
            > {};

#undef BOOST_ACT_INTERLOCKED_DETAIL_ADDITIVE_PARAMS_BASE_MACRO
#undef BOOST_ACT_INTERLOCKED_DETAIL_ARITHMETIC_PARAMS_BASE_MACRO

} } } }

#else // Else: No CAS support defined

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename LeftOperand, typename RightOperand = LeftOperand
        , typename ComparisonType = LeftOperand
        >
struct are_valid_arithmetic_params
  : mpl::false_
{
  BOOST_MPL_ASSERT_MSG
  (
    ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
  , INTERLOCKED_OPERATIONS_NOT_DEFINED_FOR_THIS_SYSTEM
  , ()
  );
};

template< typename LeftOperand, typename RightOperand = LeftOperand
        , typename ComparisonType = LeftOperand
        >
struct are_valid_store_style_params
  : mpl::false_
{

  BOOST_MPL_ASSERT_MSG
  (
    ( mpl::apply< mpl::always< mpl::false_ >, TargetType >::type::value )
  , INTERLOCKED_OPERATIONS_NOT_DEFINED_FOR_THIS_SYSTEM
  , ()
  );

};


} } } }

#endif

#endif
