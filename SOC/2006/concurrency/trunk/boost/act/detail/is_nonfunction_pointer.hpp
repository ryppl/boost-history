/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_IS_NONFUNCTION_POINTER_HPP
#define BOOST_ACT_DETAIL_IS_NONFUNCTION_POINTER_HPP

#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace boost { namespace act { namespace detail {

template< typename Type >
struct is_nonfunction_pointer_impl
  : mpl::not_< is_function< typename remove_pointer< Type >::type > > {};

template< typename Type >
struct is_nonfunction_pointer
  : mpl::and_< is_pointer< Type >, is_nonfunction_pointer_impl< Type > > {};

} } }

#endif
