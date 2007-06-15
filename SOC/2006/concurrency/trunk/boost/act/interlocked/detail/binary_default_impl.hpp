/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/act/interlocked/detail/full_name.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME
#undef BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME
#endif

#define BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME                           \
BOOST_PP_TUPLE_ELEM( 3, 0                                                      \
                   , BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO            \
                   )

#define BOOST_ACT_INTERLOCKED_DETAIL_CURR_SEMANTIC_ID_IS_FULL_BARRIER          \
BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_                              \
            , BOOST_PP_TUPLE_ELEM                                              \
              ( 3, 1                                                           \
              , BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO                 \
              )                                                                \
            )                                                                  \
== BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_full_barrier

#if BOOST_ACT_INTERLOCKED_DETAIL_CURR_SEMANTIC_ID_IS_FULL_BARRIER

#include <boost/act/interlocked/assign_if_was/assign_if_was_fwd.hpp>
#include <boost/act/interlocked/retrieve_fwd.hpp>

#else

#define BOOST_ACT_INTERLOCKED_CURR_FWD_HEADER()                                \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( BOOST_PP_CAT( <boost/act/interlocked/                          \
                            , BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME     \
                            )                                                  \
              , BOOST_PP_CAT( /                                                \
                            , BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME     \
                            )                                                  \
              )                                                                \
, _fwd.hpp>                                                                    \
)

#include BOOST_ACT_INTERLOCKED_CURR_FWD_HEADER()

#undef BOOST_ACT_INTERLOCKED_CURR_FWD_HEADER

#endif

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct BOOST_PP_CAT
       (
         BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME
         ( BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME
         , BOOST_PP_TUPLE_ELEM( 3, 1
                              , BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO
                              )
         )
       , _default_impl
       )
{
  template< typename TargetType, typename OperandType >
  static ResultType execute( TargetType& target, OperandType& operand )
  {

#if BOOST_ACT_INTERLOCKED_DETAIL_CURR_SEMANTIC_ID_IS_FULL_BARRIER

    UnqualifiedType new_value;

    for( UnqualifiedType curr_value = retrieve( target )
       ;    ( new_value = assign_if_was
                          ( target
                          , curr_value
                            BOOST_PP_TUPLE_ELEM
                            ( 3, 2
                            , BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO
                            )
                            operand
                          , curr_value
                          )
                            .old_value()
            )
         != curr_value
       ; curr_value = new_value
       );

    return ResultType( new_value, operand );

#else

    return BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME( target, operand );

#endif

  }
};

} } } }

#if BOOST_ACT_INTERLOCKED_DETAIL_CURR_SEMANTIC_ID_IS_FULL_BARRIER

#undef BOOST_ACT_INTERLOCKED_DETAIL_CURR_SEMANTIC_ID_IS_FULL_BARRIER
#undef BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME
#undef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_INFO

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>
#include <boost/act/interlocked/retrieve.hpp>

#else

#undef BOOST_ACT_INTERLOCKED_DETAIL_CURR_SEMANTIC_ID_IS_FULL_BARRIER

#define BOOST_ACT_INTERLOCKED_CURR_HEADER()                                    \
BOOST_PP_CAT                                                                   \
(                                                                              \
  BOOST_PP_CAT( BOOST_PP_CAT( <boost/act/interlocked/                          \
                            , BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME     \
                            )                                                  \
              , BOOST_PP_CAT( /                                                \
                            , BOOST_ACT_INTERLOCKED_DETAIL_CURR_SHORT_NAME     \
                            )                                                  \
              )                                                                \
, .hpp>                                                                        \
)

#include BOOST_ACT_INTERLOCKED_CURR_HEADER()

#undef BOOST_ACT_INTERLOCKED_CURR_HEADER

#endif

#endif
