//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_PARALLEL_ALGO_MODEL_HPP

#include "../algo_model.hpp"

#include "parallel_algo_model/detail/parallel_basic_for.hpp"
#include "serial_algo_model/detail/serial_basic_for.hpp"

#include <boost/utility/enable_if.hpp>

namespace boost
{
namespace act
{
namespace detail
{
struct parallel_algo_model_tag;
}

struct parallel_algo_model
  : algo_model< detail::parallel_algo_model_tag >
{
  template< typename AssignType
          , typename ConditionType
          , typename StepType
          , typename FunctionType
          >
  static typename
  ::boost::enable_if
  <
    is_parallel_safe< FunctionType >
  , FunctionType
  >
  ::type
  basic_for_impl( AssignType assign
                , ConditionType condition
                , StepType step
                , FunctionType& body
                )
  {
    return detail::parallel_basic_for( assign, condition, step, body );
  }

  template< typename AssignType
          , typename ConditionType
          , typename StepType
          , typename FunctionType
          >
  static typename
  ::boost::disable_if
  <
    is_parallel_safe< FunctionType >
  , FunctionType
  >
  ::type
  basic_for_impl( AssignType assign
                , ConditionType condition
                , StepType step
                , FunctionType& body
                , void** dummy = 0 // Workaround for GCC
                )
  {
    return detail::serial_basic_for( assign, condition, step, body );
  }
};

}
}

#endif
