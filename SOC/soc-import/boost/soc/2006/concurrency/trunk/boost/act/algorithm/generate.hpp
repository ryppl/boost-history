//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_GENERATE_HPP
#define BOOST_ACT_ALGORITHM_GENERATE_HPP

#include "detail/make_algo.hpp"

#include "algorithm_fwd.hpp"

#include "../parallel_safe.hpp"

#include "for_each.hpp"

#include "generate/generate_fwd.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType >
class safe_generator
  : public parallel_safe_if< is_parallel_safe< FunctionType > >
{
public:
  safe_generator( FunctionType function_init )
    : function_m( function_init )
  {
  }
public:
  template< typename TargetType >
  void operator ()( TargetType& target ) const
  {
    target = function_m();
  }
private:
  mutable FunctionType function_m;
};

template< typename FunctionType >
safe_generator< FunctionType >
make_safe_generator( FunctionType function_init )
{
  return safe_generator< FunctionType >( function_init );
}

}

BOOST_ACT_DETAIL_IMPLEMENT_ALGO( ((typename),IteratorType)
                                 ((typename),FunctionType)
                               , (void)
                               , generate
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((FunctionType),function)
                               )
{
  for_each[ AlgoModel() ]( begin, end
                         , detail::make_safe_generator( function )
                         );
}

}
}

#endif
