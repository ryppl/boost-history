//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ALGORITHM_BASIC_FOR_DETAIL_EXPRESSIONS_HPP
#define BOOST_ACT_ALGORITHM_BASIC_FOR_DETAIL_EXPRESSIONS_HPP

#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/array/pop_front.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

#include <limits>

namespace boost
{
namespace act
{
namespace detail
{

struct assign_var_result {};

struct comp_var_result {};

struct op_var_result {};

template< typename Type >
struct is_signed_impl
  : mpl::bool_< ::std::numeric_limits< Type >::is_signed >
{
};

template< typename Type >
struct is_signed_integral
  : mpl::and_< is_integral< Type >, is_signed_impl< Type > >
{
};

#define BOOST_ACT_DETAIL_FOR_RELATIONAL_OPS()                                  \
( 4, ( (<)(less)(>)(greater), (<=)(less_equal)(>=)(greater_equal)              \
     , (>)(greater)(<)(less), (>=)(greater_equal)(<=)(less_equal)              \
     )                                                                         \
)

#define BOOST_ACT_DETAIL_FOR_OP_UNARY_OPS()                                    \
( 2, ( (++)(increment), (--)(decrement) ) )

#define BOOST_ACT_DETAIL_FOR_OP_BINARY_OPS()                                   \
( 2, ( (+=)(plus_assign), (-=)(minus_assign) ) )

#define BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE( r, ops )                       \
BOOST_PP_ARRAY_SIZE( ops )

#define BOOST_ACT_DETAIL_FOR_OP_TYPE_OP( r, ops )                              \
BOOST_PP_ARRAY_POP_FRONT( ops )

#define BOOST_ACT_DETAIL_FOR_COMPARISON_OP( classification, operation, name )  \
template< typename IntegralType >                                              \
struct name                                                                    \
  : classification                                                             \
{                                                                              \
  name( IntegralType value_init )                                              \
    : value( value_init )                                                      \
  {                                                                            \
  }                                                                            \
                                                                               \
  template< typename TargetIntegralType >                                      \
  bool operator ()( TargetIntegralType var ) const                             \
  {                                                                            \
    return var operation value;                                                \
  }                                                                            \
                                                                               \
  IntegralType value;                                                          \
};

#define BOOST_ACT_DETAIL_FOR_BINARY_OP( classification, operation, name )      \
template< typename IntegralType >                                              \
struct name                                                                    \
  : classification                                                             \
{                                                                              \
  name( IntegralType value_init )                                              \
    : value( value_init )                                                      \
  {                                                                            \
  }                                                                            \
                                                                               \
  template< typename TargetIntegralType >                                      \
  void operator ()( TargetIntegralType& var ) const                            \
  {                                                                            \
    return var operation value;                                                \
  }                                                                            \
                                                                               \
  IntegralType value;                                                          \
};

#define BOOST_ACT_DETAIL_FOR_UNARY_OP( classification, operation, name )       \
struct name                                                                    \
  : classification                                                             \
{                                                                              \
  template< typename TargetIntegralType >                                      \
  void operator ()( TargetIntegralType& var ) const                            \
  {                                                                            \
    operation var;                                                             \
  }                                                                            \
};

BOOST_ACT_DETAIL_FOR_COMPARISON_OP( comp_var_result, <, less )
BOOST_ACT_DETAIL_FOR_COMPARISON_OP( comp_var_result, <=, less_equal )
BOOST_ACT_DETAIL_FOR_COMPARISON_OP( comp_var_result, >, greater )
BOOST_ACT_DETAIL_FOR_COMPARISON_OP( comp_var_result, >=, greater_equal )

BOOST_ACT_DETAIL_FOR_UNARY_OP( op_var_result, ++, increment )
BOOST_ACT_DETAIL_FOR_UNARY_OP( op_var_result, --, decrement )

BOOST_ACT_DETAIL_FOR_BINARY_OP( op_var_result, +=, plus_assign )
BOOST_ACT_DETAIL_FOR_BINARY_OP( op_var_result, -=, minus_assign )

template< typename SignedIntegralType >
class assign
  : public assign_var_result
{
public:
  typedef SignedIntegralType value_type;

  assign( SignedIntegralType value_init )
    : value_m( value_init )
  {
  }

  value_type operator ()() const
  {
    return value_m;
  }
private:
  value_type const value_m;
};

struct for_var_type
{
#define BOOST_ACT_DETAIL_BINARY_VAR_OP( operation, operation_name )            \
  template< typename IntegralType >                                            \
  friend typename enable_if                                                    \
  <                                                                            \
    is_integral< IntegralType >                                                \
  , operation_name< IntegralType >                                             \
  >                                                                            \
  ::type                                                                       \
  operator operation( for_var_type, IntegralType operand )                     \
  {                                                                            \
    return operation_name< IntegralType >( operand );                          \
  } 

#define BOOST_ACT_DETAIL_REVERSE_BINARY_VAR_OP( operation, operation_name )    \
  template< typename IntegralType >                                            \
  friend typename enable_if                                                    \
  <                                                                            \
    is_integral< IntegralType >                                                \
  , operation_name< IntegralType >                                             \
  >                                                                            \
  ::type                                                                       \
  operator operation( IntegralType operand, for_var_type )                     \
  {                                                                            \
    return operation_name< IntegralType >( operand );                          \
  } 

BOOST_ACT_DETAIL_BINARY_VAR_OP( +=, plus_assign )
BOOST_ACT_DETAIL_BINARY_VAR_OP( -=, minus_assign )

#define BOOST_ACT_DETAIL_REVERSIBLE_VAR_OP(op,op_name,reverse_op,reverse_name )\
  BOOST_ACT_DETAIL_BINARY_VAR_OP( op, op_name )                                \
  BOOST_ACT_DETAIL_BINARY_VAR_OP( reverse_op, reverse_name )

#define BOOST_ACT_DETAIL_REVERSIBLE_OP_FORWARD( operation_info )               \
BOOST_ACT_DETAIL_BINARY_VAR_OP( BOOST_PP_SEQ_ELEM( 0, operation_info )         \
                              , BOOST_PP_SEQ_ELEM( 1, operation_info )         \
                              )                                                \
BOOST_ACT_DETAIL_REVERSE_BINARY_VAR_OP( BOOST_PP_SEQ_ELEM( 2, operation_info ) \
                                      , BOOST_PP_SEQ_ELEM( 3, operation_info ) \
                                      )

#define BOOST_ACT_DETAIL_REVERSIBLE_BINARY_VAR_OP_MACRO( r, operations )       \
BOOST_ACT_DETAIL_REVERSIBLE_OP_FORWARD( BOOST_PP_ARRAY_ELEM( 0, operations ) )

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_RELATIONAL_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_REVERSIBLE_BINARY_VAR_OP_MACRO
            )

#define BOOST_ACT_DETAIL_OP_VAR_UNARY_OP( operation, operation_name )          \
  friend operation_name operator operation( for_var_type )                     \
  {                                                                            \
    return operation_name();                                                   \
  }                                                                            \
                                                                               \
  friend operation_name operator operation( for_var_type, int )                \
  {                                                                            \
    return operation_name();                                                   \
  } 

BOOST_ACT_DETAIL_OP_VAR_UNARY_OP( ++, increment )
BOOST_ACT_DETAIL_OP_VAR_UNARY_OP( --, decrement )

  template< typename SignedIntegralType >
  typename enable_if
  <
    is_signed_integral< SignedIntegralType >
  , assign< SignedIntegralType >
  >
  ::type
  operator =( SignedIntegralType operand ) const
  {
    return assign< SignedIntegralType >( operand );
  }
};

#define BOOST_IS_EXP_CLASSIFICATION_DEF( classification )                      \
template< typename Type >                                                      \
struct BOOST_PP_CAT( BOOST_PP_CAT( is_, classification ), _type )              \
  : is_convertible                                                             \
    <                                                                          \
      Type const volatile*                                                     \
    , BOOST_PP_CAT( classification, _var_result ) const volatile*              \
    >                                                                          \
{                                                                              \
}; 

BOOST_IS_EXP_CLASSIFICATION_DEF( assign )
BOOST_IS_EXP_CLASSIFICATION_DEF( comp )
BOOST_IS_EXP_CLASSIFICATION_DEF( op )

template< typename AssignType
        , typename ComparisonType
        , typename OperationType
        >
struct for_params_are_valid
  : mpl::and_
    < is_assign_type< AssignType >
    , is_comp_type< ComparisonType >
    , is_op_type< OperationType >
    >
{
};

}

detail::for_var_type const for_var = detail::for_var_type();

}
}

#endif
