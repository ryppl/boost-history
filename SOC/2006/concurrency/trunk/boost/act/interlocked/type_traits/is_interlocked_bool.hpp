/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_INTERLOCKED_BOOL_HPP
#define BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_INTERLOCKED_BOOL_HPP

#include <boost/act/config/interlocked/has_int.hpp>

#if BOOST_ACT_INTERLOCKED_HAS_INT( 1 )

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type >
struct is_interlocked_bool
  : mpl::or_< is_same< Type, bool_least_t >
            , is_same< Type, bool_fast_t >
            > {};

} } }

#else

#include <boost/mpl/bool.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type > struct is_interlocked_bool : mpl::false_ {};

} } }

#endif

#endif
