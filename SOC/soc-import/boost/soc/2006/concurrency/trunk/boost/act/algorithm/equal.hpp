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

#ifndef BOOST_ACT_ALGORITHM_EQUAL_HPP
#define BOOST_ACT_ALGORITHM_EQUAL_HPP

#include <algorithm>
#include <functional>

#include "equal/equal_fwd.hpp"

#include "detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  equal
, 2
, ( ( ((typename),InputIterator1)((typename),InputIterator2)
    , (bool)
    , ((InputIterator1),first1)((InputIterator1),last1)
      ((InputIterator2),first2)
    )
  , ( ((typename),InputIterator1)((typename),InputIterator2)
      ((typename),BinaryPredicate)
    , (bool)
    , ((InputIterator1),first1)((InputIterator1),last1)
      ((InputIterator2),first2)
      ((BinaryPredicate),pred)
    )
  )
)

template< typename AlgoModel >
struct equal_base_impl
{
  template< typename ExtendedParamsType
          , typename InputIterator1, typename InputIterator2
          , typename BinaryPredicate
          >
  static
  bool execute( ExtendedParamsType const& extended_params
              , InputIterator1 first1, InputIterator1 last1
              , InputIterator2 first2, BinaryPredicate pred
              )
  {
    return ::std::equal( first1, last1, first2, pred );
  }
};

// ToDo: Change definition to rely on other overload
BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),InputIterator1)((typename),InputIterator2)
, (bool)
, equal
, ((InputIterator1),first1)((InputIterator1),last1)
  ((InputIterator2),first2)
)
{
  // ToDo: Change to use a better ::std::less

  typedef typename iterator_value< InputIterator1 >::type value_type;

  return equal_base_impl< AlgoModel >::execute( extended_params
                                              , first1, last1, first2
                                              , ::std::equal_to< value_type >()
                                              );
}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),InputIterator1)((typename),InputIterator2)
  ((typename),BinaryPredicate)
, (bool)
, equal
, ((InputIterator1),first1)((InputIterator1),last1)
  ((InputIterator2),first2)
  ((BinaryPredicate),pred)
)
{
  return equal_base_impl< AlgoModel >::execute( extended_params
                                              , first1, last1, first2
                                              , pred
                                              );
}

}
}

#endif
