//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_SEARCH_N_SEARCH_N_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_SEARCH_N_SEARCH_N_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS
(
  search_n
, 2
, ( ( ((typename),ForwardIterator)((typename),Size)
      ((typename),T)
    , (ForwardIterator)
    , ((ForwardIterator),first)((ForwardIterator),last)
      ((Size),count)
      ((const T&),value)
    )
  , ( ((typename),ForwardIterator)((typename),Size)
      ((typename),T)((typename),BinaryPredicate)
    , (ForwardIterator)
    , ((ForwardIterator),first)((ForwardIterator),last)
      ((Size),count)
      ((const T&),value)((BinaryPredicate),pred)
    )
  )
)

}
}

#endif
