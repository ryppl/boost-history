/*=============================================================================
    Copyright (c) 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_FLAG_HPP
#define BOOST_ACT_INTERLOCKED_FLAG_HPP

#include <boost/act/config/interlocked/has.hpp>

// Too: Don't rely on bool when direct support is available (or bool isn't).

#include <boost/act/interlocked/integer/types.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/act/interlocked/assign_if_was.hpp>
#include <boost/act/interlocked/type_traits/is_semantics.hpp>

namespace boost { namespace act { namespace interlocked {

typedef bool_least_t flag_least_t;
typedef bool_fast_t  flag_fast_t;

// Note: Returns old value
template< typename Semantics, typename Type >
typename lazy_enable_if
<
  mpl::and_
  <
    is_semantics< Semantics >
  , mpl::or_< is_same< typename remove_volatile< Type >::type, flag_least_t >
            , is_same< typename remove_volatile< Type >::type, flag_fast_t >
            >
  >
, remove_volatile< Type >
>
set_if_was_clear( Type& target )
{
  return assign_if_was< Semantics >( target, true, false );
}

template< typename Semantics, typename Type >
typename lazy_enable_if
<
  mpl::and_
  <
    is_semantics< Semantics >
  , mpl::or_< is_same< typename remove_volatile< Type >::type, flag_least_t >
            , is_same< typename remove_volatile< Type >::type, flag_fast_t >
            >
  >
, void
>
clear( Type& target )
{
  assign_if_was< Semantics >( target, false, true );
}

} } }

#endif
