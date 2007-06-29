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
#include <boost/act/interlocked/modify.hpp>

#include <boost/type_traits/remove_cv.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename OperandType >
class BOOST_PP_CAT
      (
        BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
      , _default_impl_fun_object
      )
{
public:
  explicit BOOST_PP_CAT
           (
             BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
           , _default_impl_fun_object
           )
           ( OperandType operand_init )
    : operand_m( operand_init ) {}
public:
  template< typename TargetType >
  typename remove_cv< TargetType >::type
  operator ()( TargetType const& target ) const
  {
    return static_cast< typename remove_cv< TargetType >::type >
           ( target BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_OP operand_m );
  }
private:
  OperandType operand_m;
};

// ToDo: Change to minimize memory barriers (don't always use full barrier form)
struct BOOST_PP_CAT
       (
         BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
       , _default_impl
       )
{
  template< typename TargetType, typename OperandType >
  static typename remove_cv< TargetType >::type
  execute( TargetType& target, OperandType const& operand )
  {
    typedef typename remove_cv< OperandType >::type stored_operand_type;

    return modify< BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_SEMANTICS >
           ( target, BOOST_PP_CAT
                     (
                       BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_FULL_NAME
                     , _default_impl_fun_object
                     )
                     < stored_operand_type >( operand )
           );
  }
};

} } } }

#define BOOST_ACT_INTERLOCKED_DETAIL_BINARY_DEFAULT_IS_READY_FOR_CLEANUP

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>
#include <boost/act/interlocked/load.hpp>

#endif
