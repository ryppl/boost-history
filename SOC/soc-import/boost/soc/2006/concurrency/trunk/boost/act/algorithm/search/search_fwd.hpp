//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_SEARCH_SEARCH_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_SEARCH_SEARCH_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS
(
  search
, 2
, ( ( ((typename),ForwardIterator1)((typename),ForwardIterator2)
    , (ForwardIterator1)
    , ((ForwardIterator1),first1)((ForwardIterator1),last1)
      ((ForwardIterator2),first2)((ForwardIterator2),last2)
    )
  , ( ((typename),ForwardIterator1)((typename),ForwardIterator2)
      ((typename),BinaryPredicate)
    , (ForwardIterator1)
    , ((ForwardIterator1),first1)((ForwardIterator1),last1)
      ((ForwardIterator2),first2)((ForwardIterator2),last2)
      ((BinaryPredicate),pred)
    )
  )
)

}
}

#endif
