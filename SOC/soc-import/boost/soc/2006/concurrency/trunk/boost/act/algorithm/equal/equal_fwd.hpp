//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_EQUAL_EQUAL_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_EQUAL_EQUAL_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS
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

}
}

#endif
