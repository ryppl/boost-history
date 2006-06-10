//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_FOR_EACH_DETAIL_FOR_EACH_BASIC_HPP
#define BOOST_ACT_ALGORITHM_FOR_EACH_DETAIL_FOR_EACH_BASIC_HPP

#include <boost/iterator/iterator_traits.hpp>

#include "../../../type_traits/is_algo_model.hpp"
#include "../../../type_traits/is_parallel_safe.hpp"
#include "../../../parallel_safe.hpp"
#include "../../basic_for.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename IteratorType, typename FunctionType >
class for_each_basic_applier
  : public parallel_safe_if< is_parallel_safe< FunctionType > >
{
public:
  for_each_basic_applier( IteratorType const& begin_init
                        , FunctionType const& function_init
                        )
    : begin_m( begin_init )
    , function_m( function_init )
  {
  }
private:
  typedef typename ::boost::iterator_difference< IteratorType >::type
            difference_type;
public:
  // ToDo: Possibly use a calculated size type
  void operator ()( difference_type index ) const
  {
    function_m( *( begin_m + index ) );
  }
public:
  operator FunctionType() const
  {
    return function_m;
  }
private:
  IteratorType begin_m;
  FunctionType function_m;
};

template< typename IteratorType, typename FunctionType >
for_each_basic_applier< IteratorType, FunctionType >
make_for_each_basic_applier( IteratorType const& begin
                           , FunctionType const& function
                           )
{
  return for_each_basic_applier< IteratorType, FunctionType >
           ( begin, function );
}

template< typename AlgoModel, typename IteratorType, typename FunctionType >
FunctionType for_each_basic( IteratorType begin
                           , IteratorType end
                           , FunctionType const& function
                           )
{
  typedef typename ::boost::iterator_difference< IteratorType >::type
            difference_type;

  return basic_for[ AlgoModel() ]( for_var = static_cast< difference_type >( 0 )
                                 , for_var < end - begin
                                 , ++for_var
                                 )
         [
           make_for_each_basic_applier( begin, function )
         ];
}

}
}
}

#endif
