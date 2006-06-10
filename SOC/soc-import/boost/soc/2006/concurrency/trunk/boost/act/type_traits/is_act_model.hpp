//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_IS_ACT_MODEL_HPP
#define BOOST_ACT_TYPE_TRAITS_IS_ACT_MODEL_HPP

#include <boost/type_traits/is_convertible.hpp>

#include "../detail/act_model_base.hpp"

namespace boost
{
namespace act
{

template< typename Type >
struct is_act_model
  : ::boost::is_convertible< Type*, detail::act_model_base const volatile* >
{
};

}
}

#endif
