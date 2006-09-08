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

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_OMP_EQUAL_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_OMP_EQUAL_HPP

#include "../parallel_range_fun.hpp"

#include "../../../../algorithm/parallel.hpp"

#include "../../../../parallel_safe.hpp"

#include <algorithm>
#include <iterator>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Handle parallel safety
template< typename Predicate >
class parallel_equal_fun
  : parallel_safe
{
public:
  parallel_equal_fun( Predicate const& predicate_init
                    , bool volatile& result_init
                    )
    : predicate_m( predicate_init )
    , result_m( result_init )
  {
  }
public:
  template< typename InputIterator1, typename Size
          , typename InputIterator2
          >
  void operator ()( InputIterator1 curr1, Size range_length
                  , InputIterator2 curr2
                  )
  {
    for( Size index = 0; index < range_length; ++range_length )
    {
      if( !predicate_m( *curr1, *curr2 ) )
      {
        result_m = false;
        break;
      }

      ++curr1;
      ++curr2;
    }
  }
private:
  Predicate predicate_m;
  bool volatile& result_m;
};

template< typename Predicate >
parallel_equal_fun< Predicate >
make_parallel_equal_fun( Predicate const& predicate, bool volatile& result )
{
  return parallel_equal_fun< Predicate >( predicate, result );
}

// ToDo: Fix parallel safety
struct equal_fun
{
  template< typename LeftType, typename RightType >
  bool operator ()( LeftType const& left, RightType const& right ) const
  {
    return left == right;
  }
};

}

template< typename AlgoModel >
struct equal_impl;

template<>
struct equal_impl< parallel_algo_model >
{
  template< typename ExtendedParams
          , typename InputIterator1, typename InputIterator2
          >
  static bool execute( ExtendedParams& extended_params
                     , InputIterator1& first1, InputIterator1& last1
                     , InputIterator2& first2
                     )
  {
    return execute( extended_params
                  , first1, last1, first2, detail::equal_fun()
                  );
  }

  template< typename ExtendedParams
          , typename InputIterator1, typename InputIterator2
          , typename BinaryPredicate
          >
  static bool execute( ExtendedParams& extended_params
                     , InputIterator1& first1, InputIterator1& last1
                     , InputIterator2& first2, BinaryPredicate& pred
                     , typename enable_if
                       <
                         mpl::and_
                         <
                           is_same
                           <
                             typename iterator_category< InputIterator1 >::type
                           , ::std::random_access_iterator_tag
                           >
                         , is_same
                           <
                             typename iterator_category< InputIterator2 >::type
                           , ::std::random_access_iterator_tag
                           >
                         >
                       >
                       ::type** dummy = 0
                     )
  {
    bool volatile result = true;

    parallel[ extended_params ]
      ( detail::make_binary_parallel_range_fun
          ( first1, last1 - first1
          , first2, detail::make_parallel_equal_fun( pred, result )
          )
      );

    return result;
  }

  template< typename ExtendedParams
          , typename InputIterator1, typename InputIterator2
          , typename BinaryPredicate
          >
  static
  typename disable_if
  <
    mpl::and_
    <
      is_same
      <
        typename iterator_category< InputIterator1 >::type
      , ::std::random_access_iterator_tag
      >
    , is_same
      <
        typename iterator_category< InputIterator2 >::type
      , ::std::random_access_iterator_tag
      >
    >
  , bool
  >
  ::type
  execute( ExtendedParams& extended_params
         , InputIterator1& first1, InputIterator1& last1
         , InputIterator2& first2, BinaryPredicate& pred
         )
  {
    return ::std::equal( first1, last1, first2, pred );
  }
};

}
}

#endif
