/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_INFO
#error Improper incusion of interlocked implementation header. Contact Boost.
#else

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_HPP

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_AFFIX_inverse(dummy)    1
#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_AFFIX_no_inverse(dummy) 0

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_OPERATOR_inverse( op ) op

#endif

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_NAME                            \
BOOST_PP_TUPLE_ELEM( 3, 0, BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP                                 \
BOOST_PP_TUPLE_ELEM( 3, 1, BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_INFO )

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_HAS_INVERSE                     \
BOOST_PP_CAT                                                                   \
( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_AFFIX_                         \
, BOOST_PP_TUPLE_ELEM( 3, 2                                                    \
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_INFO      \
                     )                                                         \
)

#if !BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_HAS_INVERSE

#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/bool.hpp>

#endif

namespace boost { namespace act { namespace interlocked { namespace detail {

struct BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_NAME
                   , _operation
                   )
{
  struct new_from_old
  {
    template< typename ValueType, typename OperandType >
    static ValueType execute( ValueType& value, OperandType& operand )
    {
      return value BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP operand;
    }
  };

  struct old_from_new
  {
    template< typename ValueType, typename OperandType >
    static ValueType execute( ValueType& value, OperandType& operand )
    {
      #if BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_HAS_INVERSE
      return value BOOST_PP_CAT
                   ( BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_OPERATOR_
                   , BOOST_PP_TUPLE_ELEM
                     ( 3, 2
                     , BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_INFO
                     )
                   )
                   operand;
      #else
        BOOST_MPL_ASSERT_MSG( ( mpl::apply< mpl::always< mpl::true_ >
                                          , ValueType
                                          >
                                          ::type::value
                              )
                            , NO_INVERSE_FOR_OPERATION
                            , ()
                            );

        return ValueType();
      #endif
    }
  };
};

} } } }

#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_HAS_INVERSE
#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP
#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OP_NAME

#undef BOOST_ACT_INTERLOCKED_DETAIL_BINARY_OPERATION_INFO

#endif
