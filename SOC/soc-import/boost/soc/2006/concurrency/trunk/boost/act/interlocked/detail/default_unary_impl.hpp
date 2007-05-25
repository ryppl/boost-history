/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
/*
#ifndef BOOST_ACT_INTERLOCKED_DETAIL_DEFAULT_IMPL_DETAILS
#error Improper incusion of interlocked implementation header. Contact Boost.
//#else

#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_SHORT_NAME                         \
BOOST_PP_TUPLE_ELEM( 3, 0, BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_SEMANTICS                          \
BOOST_PP_TUPLE_ELEM( 3, 1, BOOST_ACT_INTERLOCKED_DETAIL_RESULT_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_AFFIX_full_barrier() 0
#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_AFFIX_acquire()      1
#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_AFFIX_release()      2

#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_FULL_NAME                          \
BOOST_PP_IF( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_RESULT_AFFIX_          \
                         , BOOST_ACT_INTERLOCKED_DETAIL_RESULT_SEMANTICS       \
                         )                                                     \
           , BOOST_PP_CAT                                                      \
             (                                                                 \
               _                                                               \
             , BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_RESULT_SHORT_NAME    \
                           , BOOST_ACT_INTERLOCKED_DETAIL_RESULT_SEMANTICS     \
                           )                                                   \
             )                                                                 \
           , BOOST_ACT_INTERLOCKED_DETAIL_RESULT_SHORT_NAME                    \
           )

// ToDo: Fix to account for acquire/release
#include <boost/act/interlocked/assign_if_was/assign.hpp>
#include <boost/act/interlocked/safe_get.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct BOOST_ACT_INTERLOCKED_DETAIL_RESULT_FULL_NAME
{
  template< typename TargetType >
  static ResultType execute( TargetType& target )
  {
    for( UnqualifiedType curr_value = safe_get( target ),
                         new_value  = curr_value
       ; curr_value = assign_if_was( target, ++new_value, curr_value )
       ; new_value = curr_value
       );
  }
};


} } } }

#endif
*/
