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

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_DETAIL_PARALLEL_BASIC_FOR_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_DETAIL_PARALLEL_BASIC_FOR_HPP

#include "../../../config/openmp.hpp"

#if !BOOST_SUPPORTS_OPENMP
#include "../../serial_algo_model/detail/serial_basic_for.hpp"
#endif

namespace boost
{
namespace act
{
namespace detail
{

#if BOOST_SUPPORTS_OPENMP

#define BOOST_ACT_DETAIL_PARALLEL_UNARY_FOR_HEADER( comp_name, op_name )       \
template< typename ExtendedParamsType                                          \
        , typename AssignIntegerType                                           \
        , typename CompIntegerType                                             \
        , typename FunctionType                                                \
        >                                                                      \
FunctionType parallel_basic_for( ExtendedParamsType const& extended_params     \
                               , assign< AssignIntegerType > assign_op         \
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
template< typename ExtendedParamsType                                          \
        , typename AssignIntegerType                                           \
        , typename CompIntegerType                                             \
        , typename OpIntegerType                                               \
        , typename FunctionType                                                \
        >                                                                      \
FunctionType parallel_basic_for( ExtendedParamsType const& extended_params     \
                               , assign< AssignIntegerType > assign_op         \
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

template< typename ExtendedParamsType
        , typename AssignType
        , typename ConditionType
        , typename StepType
        , typename FunctionType
        >
FunctionType parallel_basic_for( ExtendedParamsType const& extended_params
                               , AssignType assign
                               , ConditionType condition
                               , StepType step
                               , FunctionType& body
                               )
{
  return serial_basic_for( extended_params, assign, condition, step, body );
}

#endif

}
}
}


#endif
