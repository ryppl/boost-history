//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACT_MODEL_BASE_HPP
#define BOOST_ACT_DETAIL_ACT_MODEL_BASE_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/cat.hpp>

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

struct act_model_base
{
  struct action_constructor_active_call_tag
  {
  };

  struct action_constructor_queued_call_tag
  {
  };
};

}
}
}

#endif
