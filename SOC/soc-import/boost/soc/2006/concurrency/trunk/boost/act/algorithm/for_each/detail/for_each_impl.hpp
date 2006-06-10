//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_DETAIL_FOR_EACH_IMPL_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_FOR_EACH_IMPL_HPP

#include "for_each_basic.hpp"

#include <algorithm>

#include "../../../act_fwd.hpp"

#include "../../basic_for.hpp"

namespace boost
{
namespace act
{

template< typename AlgoModel >
struct for_each_safe_random_impl
{
  template< typename IteratorType
          , typename FunctionType
          >
  static FunctionType execute( IteratorType begin
                             , IteratorType end
                             , FunctionType const& function
                             )
  {
    return detail::for_each_basic< AlgoModel >
             ( begin, end, function );
  }
};

template< typename AlgoModel >
struct for_each_safe_forward_impl
{
  template< typename IteratorType
          , typename FunctionType
          >
  static FunctionType execute( IteratorType begin
                             , IteratorType end
                             , FunctionType const& function
                             )
  {
    // ToDo: Change to spawn active objects which queue functions
    return ::std::for_each( begin, end, function );
  }
};

template< typename AlgoModel >
struct for_each_unsafe_impl
{
  template< typename IteratorType
          , typename FunctionType
          >
  static FunctionType execute( IteratorType begin
                             , IteratorType end
                             , FunctionType const& function
                             )
  {
    return ::std::for_each( begin, end, function );
  }
};

}
}

#endif
