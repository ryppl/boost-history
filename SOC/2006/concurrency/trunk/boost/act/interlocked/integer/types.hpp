/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_TYPES_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_TYPES_HPP

#include <boost/act/config/interlocked/has_int.hpp>

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 1 )

#include <boost/act/interlocked/integer/selection.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#endif

namespace boost { namespace act { namespace interlocked {

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 1 )

// ToDo: Use native bool when supported.
typedef detail::interlocked_bool< uint_t< 1 >::least > bool_least_t;
typedef detail::interlocked_bool< uint_t< 1 >::fast >  bool_fast_t;

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 8 )

typedef int_t< 8 >::fast   int_fast8_t;
typedef uint_t< 8 >::fast  uint_fast8_t;
typedef int_t< 8 >::least  int_least8_t;
typedef uint_t< 8 >::least uint_least8_t;

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 16 )

typedef int_t< 16 >::fast   int_fast16_t;
typedef uint_t< 16 >::fast  uint_fast16_t;
typedef int_t< 16 >::least  int_least16_t;
typedef uint_t< 16 >::least uint_least16_t;

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 32 )

typedef int_t< 32 >::fast   int_fast32_t;
typedef uint_t< 32 >::fast  uint_fast32_t;
typedef int_t< 32 >::least  int_least32_t;
typedef uint_t< 32 >::least uint_least32_t;

#if BOOST_ACT_INTERLOCKED_HAS_INT_LEAST( 64 )

typedef int_t< 64 >::fast   int_fast64_t;
typedef uint_t< 64 >::fast  uint_fast64_t;
typedef int_t< 64 >::least  int_least64_t;
typedef uint_t< 64 >::least uint_least64_t;

typedef uint_fast64_t size_t; // ToDo: Change

#else

typedef uint_fast32_t size_t; // ToDo: Change

#endif // End 64-bit

#endif // End 32-bit

#endif // End 16-bit

#endif // End 8-bit

#endif // End 1-bit

} } }

#endif
