//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_DETAIL_PARALLEL_BASIC_FOR_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_DETAIL_PARALLEL_BASIC_FOR_HPP

#include "../../../config/openmp.hpp"

#ifndef BOOST_SUPPORTS_OPENMP
#include "../../serial_algo_model/detail/serial_basic_for.hpp"
#endif

namespace boost
{
namespace act
{
namespace detail
{

#ifdef BOOST_SUPPORTS_OPENMP

#define BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( comp_name, op_name )       \
template< typename AssignIntegerType                                           \
        , typename CompIntegerType                                             \
        , typename FunctionType                                                \
        >                                                                      \
FunctionType parallel_basic_for( assign< AssignIntegerType > assign_op         \
                               , comp_name< CompIntegerType > comp             \
                               , op_name op                                    \
                               , FunctionType& function                        \
                               ) 

#define BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( comp_op, op_op )                  \
for( AssignIntegerType i = assign_op(); i comp_op comp.value; op_op i )        \
{                                                                              \
  AssignIntegerType const& curr_val = i;                                       \
                                                                               \
  function( curr_val );                                                        \
}                                                                              \
return function; 

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( less, increment )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( <, ++ )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( less, decrement )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( <, -- )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( less_equal, increment )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( <=, ++ )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( less_equal, decrement )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( <=, -- )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( greater, increment )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( >, ++ )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( greater, decrement )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( >, -- )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( greater_equal, increment )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( >=, ++ )
}

BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( greater_equal, decrement )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR( >=, -- )
}

#define BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( comp_name, op_name )      \
template< typename AssignIntegerType                                           \
        , typename CompIntegerType                                             \
        , typename OpIntegerType                                               \
        , typename FunctionType                                                \
        >                                                                      \
FunctionType parallel_basic_for( assign< AssignIntegerType > assign_op         \
                               , comp_name< CompIntegerType > comp             \
                               , op_name< OpIntegerType > op                   \
                               , FunctionType& function                        \
                               ) 

#define BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( comp_op, op_op )                 \
for(AssignIntegerType i=assign_op();i comp_op comp.value;i op_op op.value)     \
{                                                                              \
  AssignIntegerType const& curr_val = i;                                       \
                                                                               \
  function( curr_val );                                                        \
}                                                                              \
return function; 

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( less, plus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( <, += )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( less, minus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( <, -= )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( greater, plus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( >, += )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( greater, minus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( >, -= )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( less_equal, plus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( <=, += )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( less_equal, minus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( <=, -= )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( greater_equal, plus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( >=, += )
}

BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR_HEADER( greater_equal, minus_assign )
{
  #pragma omp parallel for
  BOOST_ACT_DETAIL_PARALLEL_BINARY_FOR( >=, -= )
}

#else

template< typename AssignType
        , typename ConditionType
        , typename StepType
        , typename FunctionType
        >
FunctionType parallel_basic_for( AssignType assign
                               , ConditionType condition
                               , StepType step
                               , FunctionType& body
                               )
{
  return serial_basic_for( assign, condition, step, body );
}

#endif

}
}
}


#endif
