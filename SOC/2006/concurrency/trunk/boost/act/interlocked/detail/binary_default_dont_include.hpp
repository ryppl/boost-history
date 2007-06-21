/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/act/interlocked/detail/full_name.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME                 \
BOOST_PP_TUPLE_ELEM( 3, 0                                                      \
                   , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_INFO          \
                   )

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SEMANTICS                  \
BOOST_PP_TUPLE_ELEM( 3, 1                                                      \
                   , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_INFO          \
                   )

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME                  \
BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME                                         \
( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME                       \
, BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SEMANTICS                        \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_OP                         \
BOOST_PP_TUPLE_ELEM( 3, 2                                                      \
                   , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_INFO          \
                   )

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_FULL_BARRIER            \
BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_                              \
            , BOOST_PP_TUPLE_ELEM                                              \
              ( 3, 1                                                           \
              , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_INFO               \
              )                                                                \
            )                                                                  \
== BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_full_barrier

#if BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_FULL_BARRIER

#include <boost/act/interlocked/assign_if_was/assign_if_was_fwd.hpp>
#include <boost/act/interlocked/retrieve_fwd.hpp>

#else

#define BOOST_ACT_INTERLOCKED_BINARY_DEFAULT_INFO_FWD_HEADER()                 \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( BOOST_PP_CAT                                                   \
                ( <boost/act/interlocked/                                      \
                , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME       \
                )                                                              \
              , BOOST_PP_CAT                                                   \
                ( /                                                            \
                , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME       \
                )                                                              \
              )                                                                \
, _fwd.hpp>                                                                    \
)

#include BOOST_ACT_INTERLOCKED_BINARY_DEFAULT_INFO_FWD_HEADER()

#undef BOOST_ACT_INTERLOCKED_BINARY_DEFAULT_INFO_FWD_HEADER

#endif

#include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

struct BOOST_PP_CAT
       (
         BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
       , _default_impl
       )
{
  template< typename TargetType, typename OperandType >
  static typename remove_cv< TargetType >::type
  execute( TargetType& target, OperandType& operand );
};

} } } }

#if BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_FULL_BARRIER

#include <boost/act/interlocked/assign_if_was/assign_if_was_fwd.hpp>
#include <boost/act/interlocked/retrieve_fwd.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename TargetType, typename OperandType >
typename remove_cv< TargetType >::type
BOOST_PP_CAT
(
  BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
, _default_impl
)
::
execute( TargetType& target, OperandType& operand )
{
  typedef typename remove_cv< TargetType >::type unqualified_type;
  unqualified_type new_value;

  for( unqualified_type curr_value = interlocked::retrieve( target )
     ;    ( new_value = interlocked::assign_if_was
                        ( target
                        , curr_value
                          BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_OP
                          operand
                        , curr_value
                        )
          )
       != curr_value
     ; curr_value = new_value
     );

  // Note: new_value is the old value here
  return new_value;
}

} } } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_HEADER() BOOST_PP_NIL

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_READY_FOR_CLEANUP

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>
#include <boost/act/interlocked/retrieve.hpp>

#else

#define BOOST_ACT_INTERLOCKED_DEFAULT_IS_FWD_HEADER()                          \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( BOOST_PP_CAT                                                   \
                ( <boost/act/interlocked/                                      \
                , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME       \
                )                                                              \
              , BOOST_PP_CAT                                                   \
                ( /                                                            \
                , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME       \
                )                                                              \
              )                                                                \
, _fwd.hpp>                                                                    \
)

#include BOOST_ACT_INTERLOCKED_DEFAULT_IS_FWD_HEADER()

#undef BOOST_ACT_INTERLOCKED_DEFAULT_IS_FWD_HEADER

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename TargetType, typename OperandType >
typename remove_cv< TargetType >::type
BOOST_PP_CAT
(
  BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
, _default_impl
)
::execute( TargetType& target, OperandType& operand )
{
  return interlocked
         ::BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME( target
                                                                 , operand
                                                                 );

}

} } } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_HEADER()                \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( BOOST_PP_CAT                                                   \
                ( <boost/act/interlocked/                                      \
                , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME       \
                )                                                              \
              , BOOST_PP_CAT                                                   \
                ( /                                                            \
                , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SHORT_NAME       \
                )                                                              \
              )                                                                \
, .hpp>                                                                        \
)

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_READY_FOR_CLEANUP

#include BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_HEADER()

#endif

#endif
