//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_DECAY_IF_ARRAY_HPP
#define BOOST_ACT_DETAIL_DECAY_IF_ARRAY_HPP

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_extent.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Check const correctness

template< typename Type >
struct decay_impl
{
  typedef typename ::boost::remove_extent< Type >::type* const type;
};

template< typename Type >
struct decay_if_array_meta
  : ::boost::mpl::eval_if< ::boost::is_array< Type >
                         , decay_impl< Type >
                         , ::boost::mpl::identity< Type >
                         >
{
};

template< typename Type >
typename ::boost::lazy_enable_if
<
  ::boost::is_array< Type >
, ::boost::remove_extent< Type >
>
::type*
decay_if_array( Type const& source )
{
  return source;
}

template< typename Type >
typename ::boost::disable_if
<
  ::boost::is_array< Type >
, Type const&
>
::type
decay_if_array( Type const& source )
{
  return source;
}

}
}
}

#endif
