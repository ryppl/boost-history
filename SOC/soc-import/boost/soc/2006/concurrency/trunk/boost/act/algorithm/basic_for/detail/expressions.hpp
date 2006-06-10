//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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

namespace boost
{
namespace act
{
namespace detail
{

struct assign_var_result
{
};

struct comp_var_result
{
};

struct op_var_result
{
};

#define BOOST_ACT_DETAIL_FOR_RELATIONAL_OPS()                                  \
( 4, ( (<)(less)(>)(greater), (<=)(less_equal)(>=)(greater_equal)              \
     , (>)(greater)(<)(less), (>=)(greater_equal)(<=)(less_equal)              \
     )                                                                         \
) 

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

#define BOOST_ACT_DETAIL_FOR_BINARY_OP_COMP_MACRO( r, ops )                    \
BOOST_ACT_DETAIL_FOR_COMPARISON_OP                                             \
  ( ::boost::act::detail::comp_var_result                                      \
  , BOOST_PP_SEQ_ELEM( 0, BOOST_PP_ARRAY_ELEM( 0, ops ) )                      \
  , BOOST_PP_SEQ_ELEM( 1, BOOST_PP_ARRAY_ELEM( 0, ops ) )                      \
  ) 

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_RELATIONAL_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_FOR_BINARY_OP_COMP_MACRO
            )


#define BOOST_ACT_DETAIL_FOR_OP_UNARY_OPS()                                    \
( 2, ( (++)(increment), (--)(decrement) ) )

#define BOOST_ACT_DETAIL_FOR_OP_BINARY_OPS()                                   \
( 2, ( (+=)(plus_assign), (-=)(minus_assign) ) ) 

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

#define BOOST_ACT_DETAIL_FOR_UNARY_OP_OP_MACRO( r, ops )                       \
BOOST_ACT_DETAIL_FOR_UNARY_OP                                                  \
  ( ::boost::act::detail::op_var_result                                        \
  , BOOST_PP_SEQ_ELEM( 0, BOOST_PP_ARRAY_ELEM( 0, ops ) )                      \
  , BOOST_PP_SEQ_ELEM( 1, BOOST_PP_ARRAY_ELEM( 0, ops ) )                      \
  ) 

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_OP_UNARY_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_FOR_UNARY_OP_OP_MACRO
            )


#define BOOST_ACT_DETAIL_FOR_BINARY_OP_OP_MACRO( r, ops )                      \
BOOST_ACT_DETAIL_FOR_BINARY_OP                                                 \
  ( ::boost::act::detail::op_var_result                                        \
  , BOOST_PP_SEQ_ELEM( 0, BOOST_PP_ARRAY_ELEM( 0, ops ) )                      \
  , BOOST_PP_SEQ_ELEM( 1, BOOST_PP_ARRAY_ELEM( 0, ops ) )                      \
  ) 

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_OP_BINARY_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_FOR_BINARY_OP_OP_MACRO
            )

template< typename IntegralType >
class assign
  : public assign_var_result
{
public:
  typedef IntegralType value_type;

  assign( IntegralType value_init )
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
  for_var_type()
  {
  }

#define BOOST_ACT_DETAIL_BINARY_VAR_OP( operation, operation_name )            \
  template< typename IntegerType >                                             \
  friend typename ::boost::enable_if                                           \
  <                                                                            \
    ::boost::is_integral< IntegerType >                                        \
  , operation_name< IntegerType >                                              \
  >                                                                            \
  ::type                                                                       \
  operator operation( for_var_type const&, IntegerType operand )               \
  {                                                                            \
    return operation_name< IntegerType >( operand );                           \
  } 

#define BOOST_ACT_DETAIL_BINARY_VAR_OP_FORWARD( operation_info )               \
BOOST_ACT_DETAIL_BINARY_VAR_OP( BOOST_PP_SEQ_ELEM( 0, operation_info )         \
                              , BOOST_PP_SEQ_ELEM( 1, operation_info )         \
                              ) 

#define BOOST_ACT_DETAIL_BINARY_VAR_OP_MACRO( r, operations )                  \
BOOST_ACT_DETAIL_BINARY_VAR_OP_FORWARD( BOOST_PP_ARRAY_ELEM( 0, operations ) ) 

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_OP_BINARY_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_BINARY_VAR_OP_MACRO
            )

#define BOOST_ACT_DETAIL_REVERSE_BINARY_VAR_OP( operation, operation_name )    \
  template< typename IntegerType >                                             \
  friend typename ::boost::enable_if                                           \
  <                                                                            \
    ::boost::is_integral< IntegerType >                                        \
  , operation_name< IntegerType >                                              \
  >                                                                            \
  ::type                                                                       \
  operator operation( IntegerType operand, for_var_type const& )               \
  {                                                                            \
    return operation_name< IntegerType >( operand );                           \
  } 


#define BOOST_ACT_DETAIL_REVERSIBLE_VAR_OP(op,op_name,reverse_op,reverse_name )\
  BOOST_ACT_DETAIL_BINARY_VAR_OP( op, op_name )                                \
  BOOST_ACT_DETAIL_BINARY_VAR_OP( reverse_op, reverse_name ) 

#define BOOST_ACT_DETAIL_REVERSIBLE_OP_FWD(op,op_name,reverse_op,reverse_name )\
BOOST_ACT_DETAIL_BINARY_VAR_OP( BOOST_PP_SEQ_ELEM( 0, operation_info )         \
                              , BOOST_PP_SEQ_ELEM( 1, operation_info )         \
                              )                                                \
BOOST_ACT_DETAIL_REVERSE_BINARY_VAR_OP( BOOST_PP_SEQ_ELEM( 2, operation_info ) \
                                      , BOOST_PP_SEQ_ELEM( 3, operation_info ) \
                                      ) 

#define BOOST_ACT_DETAIL_REVERSIBLE_BINARY_VAR_OP_MACRO( r, operations )       \
BOOST_ACT_DETAIL_BINARY_VAR_OP_FORWARD( BOOST_PP_ARRAY_ELEM( 0, operations ) ) 

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_RELATIONAL_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_REVERSIBLE_BINARY_VAR_OP_MACRO
            )

#define BOOST_ACT_DETAIL_OP_VAR_PRE_UNARY_OP( operation, operation_name )      \
  friend operation_name operator operation( for_var_type const& )              \
  {                                                                            \
    return operation_name();                                                   \
  } 

#define BOOST_ACT_DETAIL_OP_VAR_POST_UNARY_OP( operation, operation_name )     \
  friend operation_name operator operation( for_var_type const&, int )         \
  {                                                                            \
    return operation_name();                                                   \
  } 

#define BOOST_ACT_DETAIL_OP_VAR_UNARY_OP_FORWARD( operation_info )             \
BOOST_ACT_DETAIL_OP_VAR_PRE_UNARY_OP( BOOST_PP_SEQ_ELEM( 0, operation_info )   \
                                    , BOOST_PP_SEQ_ELEM( 1, operation_info )   \
                                    )                                          \
BOOST_ACT_DETAIL_OP_VAR_POST_UNARY_OP( BOOST_PP_SEQ_ELEM( 0, operation_info )  \
                                     , BOOST_PP_SEQ_ELEM( 1, operation_info )  \
                                     ) 

#define BOOST_ACT_DETAIL_OP_VAR_UNARY_OP_MACRO( r, operations )                \
BOOST_ACT_DETAIL_OP_VAR_UNARY_OP_FORWARD( BOOST_PP_ARRAY_ELEM( 0, operations ) ) 

BOOST_PP_FOR( BOOST_ACT_DETAIL_FOR_OP_UNARY_OPS()
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_PREDICATE
            , BOOST_ACT_DETAIL_FOR_OP_TYPE_OP
            , BOOST_ACT_DETAIL_OP_VAR_UNARY_OP_MACRO
            )

  template< typename IntegerType >
  assign< IntegerType > operator =( IntegerType operand ) const
  {
    return assign< IntegerType >( operand );
  }
};

template< typename Type, typename Enabler = void >
struct is_assign_type
  : ::boost::mpl::false_
{
};

template< typename Type >
struct is_assign_type
<
  Type
, typename ::boost::enable_if
  <
    ::boost::is_convertible
    <
      Type const volatile*
    , assign_var_result const volatile*
    >
  >
  ::type
>
  : ::boost::mpl::true_
{
};

template< typename Type, typename Enabler = void >
struct is_comp_type
  : ::boost::mpl::false_
{
};

template< typename Type >
struct is_comp_type
<
  Type
, typename ::boost::enable_if
  <
    ::boost::is_convertible
    <
      Type const volatile*
    , comp_var_result const volatile*
    >
  >
  ::type
>
  : ::boost::mpl::true_
{
};

template< typename Type, typename Enabler = void >
struct is_op_type
  : ::boost::mpl::false_
{
};

template< typename Type >
struct is_op_type
<
  Type
, typename ::boost::enable_if
  <
    ::boost::is_convertible
    <
      Type const volatile*
    , op_var_result const volatile*
    >
  >
  ::type
>
  : ::boost::mpl::true_
{
};

template< typename AlgoModel
        , typename AssignType
        , typename ComparisonType
        , typename OperationType
        >
struct for_params_are_valid
  : ::boost::mpl::and_
    <
      is_algo_model< AlgoModel >
    , is_assign_type< AssignType >
    , is_comp_type< ComparisonType >
    , is_op_type< OperationType >
    >
{
};

}

detail::for_var_type const for_var;

}
}

#endif
