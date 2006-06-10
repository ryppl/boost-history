//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_PARAMS_TO_STORED_PARAMS_HPP
#define BOOST_ACT_DETAIL_PARAMS_TO_STORED_PARAMS_HPP

#include <boost/mpl/transform.hpp>
#include <boost/mpl/lambda.hpp>

#include "act_ref.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename ActModel, typename SourceParam >
struct param_to_stored_param
  : ActModel::template param
    <
      typename remove_act_ref< SourceParam >::type
    >
{
};

template< typename ActModel, typename ParamSequence >
struct params_to_stored_params
  : ::boost::mpl::transform
    <
      ParamSequence
    , param_to_stored_param< ActModel, ::boost::mpl::_1 >
    >
{
};

}
}
}

#endif
