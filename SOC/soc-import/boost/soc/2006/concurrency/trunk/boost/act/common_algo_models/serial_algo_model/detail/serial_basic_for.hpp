//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_SERIAL_ALGO_MODEL_DETAIL_SERIAL_BASIC_FOR_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_SERIAL_ALGO_MODEL_DETAIL_SERIAL_BASIC_FOR_HPP

namespace boost
{
namespace act
{
namespace detail
{

template< typename AssignType
        , typename ConditionType
        , typename StepType
        , typename FunctionType
        >
FunctionType serial_basic_for( AssignType assign
                             , ConditionType condition
                             , StepType step
                             , FunctionType& body
                             )
{
  typedef typename AssignType::value_type for_var_type;

  for( for_var_type var = assign()
     ; condition( var )
     ; step( var )
     )
  {
    for_var_type const& const_var = var;

    body( const_var );
  }

  return body;
}

}
}
}

#endif
