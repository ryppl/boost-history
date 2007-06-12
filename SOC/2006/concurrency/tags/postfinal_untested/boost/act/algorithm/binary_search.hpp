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

#ifndef BOOST_ACT_ALGORITHM_BINARY_SEARCH_HPP
#define BOOST_ACT_ALGORITHM_BINARY_SEARCH_HPP

#include "detail/make_algo.hpp"

#include "binary_search/binary_search_fwd.hpp"

#include <boost/iterator/iterator_traits.hpp>

#include <algorithm>
#include <functional>

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  binary_search
, 2
, ( ( ((typename),ForwardIterator)((typename),T)
    , (bool)
    , ((ForwardIterator),first)((ForwardIterator),last)
      ((T const&),value)
    )
  , ( ((typename),ForwardIterator)((typename),T)
      ((typename),Compare)
    , (bool)
    , ((ForwardIterator),first)((ForwardIterator),last)
      ((T const&),value)((Compare),comp)
    )
  )
)

template< typename AlgoModel >
struct binary_search_base_impl
{
  template< typename ExtendedParamsType
          , typename ForwardIterator, typename T, typename Compare
          >
  static
  bool execute( ExtendedParamsType const& extended_params
              , ForwardIterator first, ForwardIterator last
              , T const& value, Compare comp
              )
  {
    return ::std::binary_search( first, last, value, comp );
  }
};

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER( ((typename),ForwardIterator)((typename),T)
                                    , (bool)
                                    , binary_search
                                    , ((ForwardIterator),first)
                                      ((ForwardIterator),last)
                                      ((T const&),value)
                                    )
{
  typedef typename iterator_value< ForwardIterator >::type value_type value_type;

  return binary_search_base_impl< AlgoModel >( extended_params
                                             , first, last, value
                                             , ::std::less< value_type >()
                                             );
}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER( ((typename),ForwardIterator)((typename),T)
                                      ((typename),Compare)
                                    , (bool)
                                    , binary_search
                                    , ((ForwardIterator),first)
                                      ((ForwardIterator),last)
                                      ((T const&),value)((Compare),comp)
                                    )
{
  return binary_search_base_impl< AlgoModel >( extended_params
                                             , first, last, value, comp
                                             );
}

}
}

#endif
