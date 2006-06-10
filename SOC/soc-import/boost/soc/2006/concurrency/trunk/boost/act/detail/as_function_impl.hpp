//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_AS_FUNCTION_IMPL_HPP
#define BOOST_ACT_DETAIL_AS_FUNCTION_IMPL_HPP

#include "../parallel_safe.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct as_function_impl_type
{
  explicit as_function_impl_type( Type const& target_init )
    : target( target_init )
  {
  }

  Type target;
};
/*
// ToDo: Account for explicit result type
//       Make less copies
template< typename Type >
struct as_safe_function_impl_type
{
  explicit as_safe_function_impl_type( Type const& target_init )
    : target( target_init )
  {
  }

  parallel_safe_fun< Type > target;
};
*/
struct as_function_type
{
  as_function_type()
  {
  }

  as_function_type operator ()() const
  {
    return as_function_type();
  }

  // ToDo: Possibly make const version (check semantics)
  template< typename Type >
  as_function_impl_type< Type >
  operator ()( Type const& target_init ) const
  {
    return as_function_impl_type< Type >( target_init );
  }
};

struct as_safe_function_type
{
  as_safe_function_type()
  {
  }

  as_safe_function_type operator ()() const
  {
    return as_safe_function_type();
  }

  // ToDo: Possibly make const version (check semantics)
  template< typename Type >
  as_function_impl_type< parallel_safe_fun< Type > >
  operator ()( Type const& target_init ) const
  {
    return as_function_impl_type< parallel_safe_fun< Type > >( target_init );
  }
};

}
}
}

#endif
