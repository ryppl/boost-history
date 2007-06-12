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

#ifndef BOOST_ACT_ALGORITHM_NEXT_PERMUTATION_HPP
#define BOOST_ACT_ALGORITHM_NEXT_PERMUTATION_HPP

#include "detail/make_algo.hpp"

#include "next_permutation/next_permutation_fwd.hpp"

#include <algorithm>
#include <functional>

#include <boost/iterator/iterator_traits.hpp>

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  next_permutation
, 2
, ( ( ((typename),BidirectionalIterator)
    , (bool)
    , ((BidirectionalIterator),first)((BidirectionalIterator),last)
    )
  , ( ((typename),BidirectionalIterator)((typename),Compare)
    , (bool)
    , ((BidirectionalIterator),first)((BidirectionalIterator),last)
      ((Compare),comp)
    )
  )
)

template< typename AlgoModel >
struct next_permutation_base_impl
{
  template< typename ExtendedParamsType
          , typename BidirectionalIterator
          >
  static
  bool execute( ExtendedParamsType const& extended_params
              , InputIterator1 first1, InputIterator1 last1
              , InputIterator2 first2, Compare comp
              )
  {
    return ::std::next_permutation( first, last, comp );
  }
};

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),BidirectionalIterator)
, (bool)
, next_permutation
, ((BidirectionalIterator),first)((BidirectionalIterator),last)
)
{
  typedef typename iterator_value< BidirectionalIterator >::type value_type;

  return next_permutation_base_impl< AlgoModel >
         ::execute( extended_params
                  , first, last
                  , ::std::less< value_type >()
                  );
}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),BidirectionalIterator)((typename),Compare)
, (bool)
, next_permutation
, ((BidirectionalIterator),first)((BidirectionalIterator),last)
  ((Compare),comp)
)
{
  return next_permutation_base_impl< AlgoModel >::execute( extended_params
                                                         , first, last
                                                         , comp
                                                         );
}

}
}

#endif
