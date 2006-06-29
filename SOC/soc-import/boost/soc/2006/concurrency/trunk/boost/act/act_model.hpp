//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACT_MODEL_HPP
#define BOOST_ACT_ACT_MODEL_HPP

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/cat.hpp>

#include "detail/act_model_base.hpp"

// ToDo: Account for active refs

namespace boost
{
namespace act
{

struct active_impl_constructor_tag {};

struct act_model
  : detail::act_model_base
{
};

}
}

#endif
