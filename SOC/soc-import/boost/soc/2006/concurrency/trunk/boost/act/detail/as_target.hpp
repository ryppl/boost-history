//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_AS_TARGET_HPP
#define BOOST_ACT_DETAIL_AS_TARGET_HPP

#include <boost/utility/enable_if.hpp>

// ToDo: Remove this and include fwd
#include "../type_traits/is_active.hpp"

namespace boost
{
namespace act
{

namespace detail
{

template< typename Type >
struct as_target_impl_type
{
  explicit as_target_impl_type( Type& target_init )
    : target( target_init )
  {
  }

  Type& target;
};

struct as_target_type
{
  as_target_type()
  {
  }

  as_target_type operator ()() const
  {
    return as_target_type();
  }

  // ToDo: Enable only for active types
  // ToDo: Possibly make const version (check semantics)
  // ToDo: Make version for inactive type which MPL asserts message
  template< typename Type >/*
  typename ::boost::enable_if
  <
    is_active< Type >
  , as_target_impl_type< Type >
  >
  ::type*/
  as_target_impl_type< Type >
  operator ()( Type& target_init ) const
  {
    return as_target_impl_type< Type >( target_init );
  }
};

}
}
}

#endif
