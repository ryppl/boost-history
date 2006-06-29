//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_ADJACENT_FIND_ADJACENT_FIND_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_ADJACENT_FIND_ADJACENT_FIND_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS
(
  adjacent_find
, 2
, ( ( ((typename),ForwardIterator)
    , (ForwardIterator)
    , ((ForwardIterator),first)
      ((ForwardIterator),last)
    )
  , ( ((typename),ForwardIterator)
      ((typename),BinaryPredicate)
    , (ForwardIterator)
    , ((ForwardIterator),first)
      ((ForwardIterator),last)
      ((BinaryPredicate),pred)
    )
  )
)

}
}

#endif
