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

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_OMP_TRANSFORM_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_OMP_TRANSFORM_HPP

#include <boost/act/functional/parallel_ranges_fun.hpp>
#include <boost/act/algorithm/parallelize.hpp>

namespace boost
{
namespace act
{
/*
namespace detail
{

struct parallel_unary_transform
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
    // ToDo: Use a size type not a difference type
    typename iterator_difference< InputIterator >::type difference_type;

    difference_type const range_size = last1 - first1;

    parallelize[ extended_params ]
      ( make_parallel_ranges_fun( first1, result, range_size
                                , make_parallel_transformer( op )
                                )
      );

    return result + range_size;
  }
};

struct serial_unary_transform
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
    return ::std::transform( first1, last1, result, op );
  }
};

struct parallel_binary_transform
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
    // ToDo: Use a size type not a difference type
    typename iterator_difference< InputIterator >::type difference_type;

    difference_type const range_size = last1 - first1;

    parallelize[ extended_params ]
      ( make_parallel_ranges_fun( first1, first2, result, range_size
                                , make_parallel_transformer( binary_op )
                                )
      );

    // ToDo: Figure out a better return that doesn't rely on random access
    return result + range_size;
  }
};

struct serial_binary_transform
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
    return ::std::transform( first1, last1, first2, result, binary_op );
  }
};

}

template< typename AlgoModel >
struct unary_transform_impl;

template< typename AlgoModel >
struct binary_transform_impl;

template< typename AlgoModel >
struct unary_transform_impl< parallel_algo_model_type >
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
    return mpl::if_
           <
             mpl::and_
             <
               is_same< typename iterator_category< InputIterator >::type
                      , ::std::random_access_iterator_tag
                      >
             , is_same< typename iterator_category< OutputIterator >::type
                      , ::std::random_access_iterator_tag
                      >
             >
           , detail::parallel_unary_transform
           , detail::serial_unary_transform
           >
           ::execute( extended_params, first1, last1, result, op );
  }
};

template< typename AlgoModel >
struct binary_transform_impl< parallel_algo_model_type >
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
    return mpl::if_
           < 
             mpl::and_
             <
               is_same< typename iterator_category< InputIterator1 >::type
                      , ::std::random_access_iterator_tag
                      >
             , is_same< typename iterator_category< InputIterator2 >::type
                      , ::std::random_access_iterator_tag
                      >
             , is_same< typename iterator_category< OutputIterator >::type
                      , ::std::random_access_iterator_tag
                      >
             >
           , detail::parallel_binary_transform
           , detail::serial_binary_transform
           >
           ::execute( extended_params
                    , first1, last1, first2, result, binary_op
                    );
  }
};
*/
}
}

#endif
