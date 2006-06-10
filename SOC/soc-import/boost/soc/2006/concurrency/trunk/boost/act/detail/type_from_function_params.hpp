//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_TYPE_FROM_FUNCTION_PARAMS_HPP
#define BOOST_ACT_DETAIL_TYPE_FROM_FUNCTION_PARAMS_HPP

#include <boost/type_traits/function_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType >
struct first_function_param_type
{
  typedef typename ::boost::function_traits< FunctionType >::arg1_type type;
};

template< typename FunctionType, typename Dummy = void >
struct type_from_function_params
  : ::boost::mpl::eval_if_c
    <
      ( ::boost::function_traits< FunctionType >::arity == 0 )
    , ::boost::mpl::identity< void >
    , first_function_param_type< FunctionType >
    >
{
};

}
}
}

#endif
