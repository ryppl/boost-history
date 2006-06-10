//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_IS_ALGO_MODEL_HPP
#define BOOST_ACT_TYPE_TRAITS_IS_ALGO_MODEL_HPP

#include <boost/type_traits/is_base_of.hpp>

#include "../algo_model.hpp"

namespace boost
{
namespace act
{

template< typename Type >
struct is_algo_model
  : ::boost::is_base_of< detail::algo_model_base, Type >
{
};

}
}

#endif
