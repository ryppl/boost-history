//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_FIND_FIRST_OF_FIND_FIRST_OF_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_FIND_FIRST_OF_FIND_FIRST_OF_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS
(
  find_first_of
, 2
, ( ( ((typename),IteratorType1)
      ((typename),IteratorType2)
    , (IteratorType1)
    , ((IteratorType1),begin1)
      ((IteratorType1),end1)
      ((IteratorType2),begin2)
      ((IteratorType2),end2)
    )
  , ( ((typename),IteratorType1)
      ((typename),IteratorType2)
      ((typename),PredicateType)
    , (IteratorType1)
    , ((IteratorType1),begin1)
      ((IteratorType1),end1)
      ((IteratorType2),begin2)
      ((IteratorType2),end2)
      ((PredicateType),predicate)
    )
  )
)

}
}

#endif
