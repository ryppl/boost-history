//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TYPE_TRAITS_IS_ACTIVE_HPP
#define BOOST_ACT_TYPE_TRAITS_IS_ACTIVE_HPP

// ToDo: Use type_traits bool types instead
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename UnqualifiedType, typename ActModel >
class active_type;

template< typename Type >
struct is_active_impl
  : ::boost::mpl::false_
{
};

template< typename UnqualifiedType, typename ActModel >
struct is_active_impl< active_type< UnqualifiedType, ActModel > >
  : ::boost::mpl::true_
{
};

}

template< typename Type >
struct is_active
  : detail::is_active_impl< typename ::boost::remove_cv< Type >::type >
{
};

}
}

#endif
