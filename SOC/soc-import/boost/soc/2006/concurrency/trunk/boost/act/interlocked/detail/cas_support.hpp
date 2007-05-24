/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_HPP

#include <boost/act/interlocked/detail/cas_support_data.hpp>

#ifdef BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA

#include <climits>
#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_SEQ                           \
  BOOST_PP_TUPLE_ELEM( 3, 0, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA )

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_CAS_RETURN                    \
  BOOST_PP_TUPLE_ELEM( 3, 1, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA )

#define BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_ARITHMETIC_RETURN             \
  BOOST_PP_TUPLE_ELEM( 3, 2, BOOST_ACT_INTERLOCKED_DETAIL_CAS_SUPPORT_DATA )

namespace boost { namespace act { namespace interlocked { namespace detail {

#define BOOST_ACT_INTERLOCKED_DETAIL_ARITHMETIC_PARAMS_BASE_MACRO(r,data,elem) \
are_valid_arithmeticn_params< ( elem / CHAR_BIT )                              \
                            , LeftOperand, RightOperand, ComparisonType        \
                            >,

template< typename LeftOperand, typename RightOperand
        , typename ComparisonType = LeftOperand
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

template< typename LeftOperand, typename RightOperand
        , typename ComparisonType = LeftOperand
        >
struct are_valid_assign_style_params
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


#undef BOOST_ACT_INTERLOCKED_DETAIL_ARITHMETIC_PARAMS_BASE_MACRO

} } } }

#else // Else: No CAS support defined

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename LeftOperand, typename RightOperand
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

template< typename LeftOperand, typename RightOperand
        , typename ComparisonType = LeftOperand
        >
struct are_valid_assign_style_params
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
