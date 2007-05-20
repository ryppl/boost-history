/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_SAFE_GET_HPP
#define BOOST_ACT_INTERLOCKED_SAFE_GET_HPP

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>

#ifdef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS

#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/utility/enable_if.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type >
typename enable_if
<
  has_interlocked_operations< Type >
, Type
>
::type
safe_get( Type volatile& target ) { return target; }

} } }

#endif

#endif

#endif
