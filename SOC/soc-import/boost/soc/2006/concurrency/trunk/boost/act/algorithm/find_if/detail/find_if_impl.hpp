//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_DETAIL_FIND_IF_IMPL_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_FIND_IF_IMPL_HPP

#include <algorithm>

#include "../../../act_fwd.hpp"

#include "../../basic_for.hpp"

#include "../../../parallel_safe.hpp"

namespace boost
{
namespace act
{

template< typename AlgoModel >
struct find_if_safe_random_impl
{
  template< typename IteratorType
          , typename PredicateType
          >
  static IteratorType execute( IteratorType begin
                             , IteratorType end
                             , PredicateType predicate
                             )
  {
    // ToDo: Change to spawn active objects which queue functions
    return ::std::find_if( begin, end, predicate );
  }
};

template< typename AlgoModel >
struct find_if_safe_forward_impl
{
  template< typename IteratorType
          , typename PredicateType
          >
  static IteratorType execute( IteratorType begin
                             , IteratorType end
                             , PredicateType predicate
                             )
  {
    // ToDo: Change to spawn active objects which queue functions
    return ::std::find_if( begin, end, predicate );
  }
};

template< typename AlgoModel >
struct find_if_unsafe_impl
{
  template< typename IteratorType
          , typename PredicateType
          >
  static IteratorType execute( IteratorType begin
                             , IteratorType end
                             , PredicateType predicate
                             )
  {
    return ::std::find_if( begin, end, predicate );
  }
};

}
}


#endif
