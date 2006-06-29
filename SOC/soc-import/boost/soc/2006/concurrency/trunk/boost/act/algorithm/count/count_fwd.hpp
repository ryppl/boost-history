//  COUNTright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_COUNT_COUNT_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_COUNT_COUNT_FWD_HPP

#include "../detail/make_algo.hpp"

#include <iterator>

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO
(
  ((typename),InputIterator)((typename),T)
, (typename ::std::iterator_traits< InputIterator >::difference_type)
, count
, ((InputIterator),first)
  ((InputIterator),last)
  ((const T&),value)
)

}
}

#endif
