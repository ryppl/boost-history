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

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_DETAIL_COUNT_IF_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_DETAIL_COUNT_IF_HPP

// ToDo: Only do if openmp is supported

#include <boost/iterator/iterator_traits.hpp>

#include "../../../atomic.hpp"
#include "../../../parallel_safe.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename CountType, typename Predicate >
class increment_count_if
  : public parallel_safe
{
public:
  increment_count_if( CountType& count_init
                    , Predicate const& predicate_init
                    )
    : count_m( count_init )
    , predicate_m( predicate_init )
  {
  }
public:
  template< typename TargetType >
  void operator ()( TargetType& target ) const
  {
    if( predicate_m( target ) )
      ++count_m;
  }
private:
  CountType& count_m;
  Predicate predicate_m; // ToDo: Possibly make mutable
};

template< typename CountType, typename Predicate >
increment_count_if< CountType, Predicate >
make_increment_count_if( CountType& count_init
                       , Predicate const& predicate_init
                       )
{
  return increment_count_if< CountType, Predicate >( count_init
                                                   , predicate_init
                                                   );
}

}

template< typename AlgoModel >
struct count_if_impl;

template<>
struct count_if_impl< parallel_algo_model_type >
{
  template< typename ExtendedParamsType
          , typename InputIterator
          , typename Predicate
          >
  static typename iterator_difference< InputIterator >::type
  execute( ExtendedParamsType const& extended_params
         , InputIterator first
         , InputIterator last
         , Predicate const& pred
         )
  {
    typedef typename iterator_difference< InputIterator >::type
              difference_type;

    BOOST_ATOMIC_T((difference_type)) curr_count = 0;

    for_each[ extended_params ]( first, last
                               , detail::make_increment_count_if( curr_count
                                                                , pred
                                                                )
                               );

    return value_of( curr_count );
  }
};

}
}

#endif
