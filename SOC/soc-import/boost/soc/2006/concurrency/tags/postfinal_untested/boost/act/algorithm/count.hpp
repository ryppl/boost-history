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

#ifndef BOOST_ACT_ALGORITHM_COUNT_HPP
#define BOOST_ACT_ALGORITHM_COUNT_HPP

#include "count/count_fwd.hpp"

#include <boost/iterator/iterator_traits.hpp>

#include "count_if.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename TargetType >
class is_same_checker
{
public:
  is_same_checker( TargetType const& target_init )
    : target_m( target_init )
  {
  }
public:
  template< typename OperandType >
  bool operator ()( OperandType const& operand ) const
  {
    return target_m == operand;
  }
private:
  TargetType const& target_m;
};

template< typename TargetType >
is_same_checker< TargetType >
make_is_same_checker( TargetType const& target )
{
  return is_same_checker< TargetType >( target );
}

}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO
(
  ((typename),InputIterator)((typename),T)
, (lazy_type< iterator_difference< InputIterator > >)
, count
, ((InputIterator),first)
  ((InputIterator),last)
  ((const T&),value)
)
{
  return count_if[ extended_params ]( first, last
                                    , detail::make_is_same_checker( value )
                                    );
}

}
}

#endif
