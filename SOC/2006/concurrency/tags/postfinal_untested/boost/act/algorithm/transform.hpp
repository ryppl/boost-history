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

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_TRANSFORM_TRANSFORM_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_TRANSFORM_TRANSFORM_FWD_HPP

#include "detail/make_algo.hpp"

#include "transform/transform_fwd.hpp"

#include "../functional/parallel_ranges_fun.hpp"

#include "parallelize.hpp"

#include <algorithm>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Do parallel safety checks
//       Make nary
template< typename FunctionType >
class transformer
{
public:
  parallel_transformer( FunctionType const& function_init )
    : function_m( function_init )
  {
  }
public:
  template< typename InputType, typename OutputType >
  void operator ()( InputType& input, OutputType& output )
  {
    output = function_m( input );
  }

  template< typename InputType1, typename InputType2, typename OutputType >
  void operator ()( InputType1& input1, InputType2& input2, OutputType& output )
  {
    output = function_m( input1, input2 );
  }
private:
  FunctionType function_m;
};

template< typename FunctionType >
transformer< FunctionType >
make_transformer( FunctionType const& function_init )
{
  return transformer< FunctionType >( function_init );
}

}

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  transform
, 2
, ( ( ((typename),InputIterator)((typename),OutputIterator)
      ((typename),UnaryOperation)
    , (OutputIterator)
    , ((InputIterator),first1)((InputIterator),last1)
      ((OutputIterator),result)((UnaryOperation),op)
    )
  , ( ((typename),InputIterator1)((typename),InputIterator2)
      ((typename),OutputIterator)((typename),BinaryOperation)
    , (OutputIterator)
    , ((InputIterator1),first1)((InputIterator1),last1)
      ((InputIterator2),first2)((OutputIterator),result)
      ((BinaryOperation),binary_op)
    )
  )
)

template< typename AlgoModel >
struct unary_transform_impl
{
  template< typename ExtendedParamsType
          , typename InputIterator, typename OutputIterator
          , typename UnaryOperation
          >
  static OutputIterator execute( ExtendedParamsType const& extended_params
                               , InputIterator first1, InputIterator last1
                               , OutputIterator result, UnaryOperation op
                               )
  {
    return for_each[ extended_params ]( first1, last1
                                      , result
                                      , detail::make_transformer( op )
                                      );
  }
};

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),InputIterator)((typename),OutputIterator)
  ((typename),UnaryOperation)
, (OutputIterator)
, transform
, ((InputIterator),first1)((InputIterator),last1)
  ((OutputIterator),result)((UnaryOperation),op)
)
{
  return ::std::transform( first1, last1, result, op );
}

template< typename AlgoModel >
struct binary_transform_impl
{
  template< typename ExtendedParamsType
          , typename InputIterator1, typename InputIterator2
          , typename OutputIterator, typename BinaryOperation
          >
  static OutputIterator execute( ExtendedParamsType const& extended_params
                               , InputIterator1 first1, InputIterator1 last1
                               , InputIterator2 first2, OutputIterator result
                               , BinaryOperation binary_op
                               )
  {
    return for_each[ extended_params ]( first1, last1
                                      , first2, result
                                      , detail::make_transformer( binary_op )
                                      );
  }
};

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),InputIterator1)((typename),InputIterator2)
  ((typename),OutputIterator)((typename),BinaryOperation)
, (OutputIterator)
, transform
, ((InputIterator1),first1)((InputIterator1),last1)
  ((InputIterator2),first2)((OutputIterator),result)
  ((BinaryOperation),binary_op)
)
{
  return binary_transform_impl< AlgoModel >
           ::execute( extended_params
                    , first1, last1
                    , first2, result
                    , binary_op
                    );
}

}
}

#endif
