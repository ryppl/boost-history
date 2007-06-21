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

// ToDo: Change to only include appropriate versions of assign_if_was
#include <boost/act/interlocked/assign_if_was.hpp>
#include <boost/act/interlocked/retrieve.hpp>

#include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

// ToDo: Change to minimize memory barriers (don't always use full barrier form)
struct BOOST_PP_CAT
       (
         BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
       , _default_impl
       )
{
  template< typename TargetType, typename OperandType >
  static typename remove_cv< TargetType >::type
  execute( TargetType& target, OperandType operand )
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
};

} } } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_READY_FOR_CLEANUP

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>
#include <boost/act/interlocked/retrieve.hpp>

#endif
