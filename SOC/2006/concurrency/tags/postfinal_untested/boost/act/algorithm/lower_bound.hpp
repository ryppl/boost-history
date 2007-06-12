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

#ifndef BOOST_ACT_ALGORITHM_LOWER_BOUND_HPP
#define BOOST_ACT_ALGORITHM_LOWER_BOUND_HPP

#include "detail/make_algo.hpp"

#include "lower_bound/lower_bound_fwd.hpp"

#include <algorithm>
#include <functional>

#include <boost/iterator/iterator_traits.hpp>

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  lower_bound
, 2
, ( ( ((typename),ForwardIterator)((typename),T)
    , (ForwardIterator)
    , ((ForwardIterator),first)((ForwardIterator),last)
      ((T const&),value)
    )
  , ( ((typename),ForwardIterator)((typename),T)
      ((typename),Compare)
    , (ForwardIterator)
    , ((ForwardIterator),first)((ForwardIterator),last)
      ((T const&),value)((Compare),comp)
    )
  )
)

template< typename AlgoModel >
struct lower_bound_base_impl
{
  template< typename ExtendedParamsType
          , typename ForwardIterator, typename T
          , typename Compare
          >
  static
  ForwardIterator execute( ExtendedParamsType const& extended_params
                         , ForwardIterator first, ForwardIterator last
                         , T const& value, Compare comp
                         )
  {
    return ::std::lower_bound( first, last, value, comp );
  }
};

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),ForwardIterator)((typename),T)
, (ForwardIterator)
, lower_bound
, ((ForwardIterator),first)((ForwardIterator),last)
  ((T const&),value)
)
{
  typedef typename iterator_value< InputIterator1 >::type value_type;

  return lower_bound_base_impl< AlgoModel >
         ::execute( extended_params
                  , first, last
                  , value, ::std::less< value_type >()
                  );
}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER
(
  ((typename),ForwardIterator)((typename),T)
  ((typename),Compare)
, (ForwardIterator)
, lower_bound
, ((ForwardIterator),first)((ForwardIterator),last)
  ((T const&),value)((Compare),comp)
)
{
  return lower_bound_base_impl< AlgoModel >::execute( extended_params
                                                    , first, last
                                                    , value, comp
                                                    );
}

}
}

#endif
