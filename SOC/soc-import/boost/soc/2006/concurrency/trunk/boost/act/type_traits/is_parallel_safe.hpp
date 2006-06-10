//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_IS_PARALLEL_SAFE_HPP
#define BOOST_ACT_TYPE_TRAITS_IS_PARALLEL_SAFE_HPP

#include "../parallel_safe.hpp"

#include <boost/type_traits/is_convertible.hpp>

namespace boost
{
namespace act
{

template< typename Type >
struct is_parallel_safe
  : ::boost::is_convertible< Type const volatile*
                           , parallel_safe const volatile*
                           >
{
};

}
}

#endif
