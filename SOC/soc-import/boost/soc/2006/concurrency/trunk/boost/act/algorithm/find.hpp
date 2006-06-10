//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)


#ifndef BOOST_ACT_ALGORITHM_FIND_HPP
#define BOOST_ACT_ALGORITHM_FIND_HPP

#include "detail/make_algo.hpp"

#include "../parallel_safe.hpp"

#include "find_if.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename LeftType >
class safe_equal_to
  : public parallel_safe
{
public:
  safe_equal_to( LeftType const& left_init )
    : left_m( left_init )
  {
  }
public:
  template< typename RightType >
  bool operator ()( RightType const& right ) const
  {
    return left_m == right;
  }
private:
  LeftType const left_m;
};

template< typename LeftType >
safe_equal_to< LeftType > make_safe_equal_to( LeftType const& left_init )
{
  return safe_equal_to< LeftType >( left_init );
}

}

BOOST_ACT_DETAIL_MAKE_ALGO_IMPL( ((typename),IteratorType)
                                 ((typename),ValueType)
                               , (IteratorType)
                               , find
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((ValueType const&),value)
                               )
{
  return find_if[ AlgoModel() ]( begin
                               , end
                               , detail::make_safe_equal_to( value )
                               );
}

}
}

#include "../config/default_algo_model.hpp"

#endif
