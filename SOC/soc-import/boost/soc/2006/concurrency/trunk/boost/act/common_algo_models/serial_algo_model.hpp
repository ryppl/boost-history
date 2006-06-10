//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_SERIAL_ALGO_MODEL_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_SERIAL_ALGO_MODEL_HPP

#include "../algo_model.hpp"

#include "serial_algo_model/detail/serial_basic_for.hpp"

#include "../act_fwd.hpp"

namespace boost
{
namespace act
{
namespace detail
{
  struct serial_algo_model_tag;
}

struct serial_algo_model
  : algo_model< detail::serial_algo_model_tag >
{
  template< typename AssignType
          , typename ConditionType
          , typename StepType
          , typename FunctionType
          >
  static FunctionType basic_for_impl( AssignType assign
                                    , ConditionType condition
                                    , StepType step
                                    , FunctionType& body
                                    )
  {
    return detail::serial_basic_for( assign, condition, step, body );
  }
};

// ToDo: Move to another file
template<>
struct for_each_impl< serial_algo_model >
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
