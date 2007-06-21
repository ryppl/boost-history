/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_TYPES_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_TYPES_HPP

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>

#ifdef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS

#include <boost/config.hpp>

// ToDo: Add in toggles for 64-bit

//#ifdef BOOST_WINDOWS

#include <windows.h>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <cstddef>

namespace boost { namespace act { namespace interlocked {

typedef long int_fast8_t;
typedef long int_fast16_t;
typedef long int_fast32_t;

typedef unsigned long uint_fast8_t;
typedef unsigned long uint_fast16_t;
typedef unsigned long uint_fast32_t;

typedef long short_t;
typedef long int_t;
typedef long long_t;

typedef unsigned long ushort_t;
typedef unsigned long uint_t;
typedef unsigned long ulong_t;

typedef ::std::size_t    size_t;
typedef ::std::ptrdiff_t ptrdiff_t;

//#if WINVER < 0x0600
#if 1

typedef unsigned long bits_least8_t;
typedef unsigned long bits_least16_t;
typedef unsigned long bits_least32_t;

typedef unsigned long bits_fast8_t;
typedef unsigned long bits_fast16_t;
typedef unsigned long bits_fast32_t;

typedef long int_least8_t;
typedef long int_least16_t;
typedef long int_least32_t;

typedef unsigned long uint_least8_t;
typedef unsigned long uint_least16_t;
typedef unsigned long uint_least32_t;

typedef long          intmax_t;
typedef unsigned long uintmax_t;

typedef detail::interlocked_bool< long > bool_t;
typedef detail::interlocked_bool< long > bool_fast_t;

#else

typedef unsigned long      bits_least8_t;
typedef unsigned long      bits_least16_t;
typedef unsigned long      bits_least32_t;
typedef unsigned long long bits_least64_t;

typedef unsigned long      bits_fast8_t;
typedef unsigned long      bits_fast16_t;
typedef unsigned long      bits_fast32_t;
typedef unsigned long long bits_fast64_t;

typedef long      int_least8_t;
typedef long      int_least16_t;
typedef long      int_least32_t;
typedef long long int_least64_t;

typedef unsigned long      uint_least8_t;
typedef unsigned long      uint_least16_t;
typedef unsigned long      uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef long long          int_fast64_t;
typedef unsigned long long uint_fast64_t;

typedef long long          intmax_t;
typedef unsigned long long uintmax_t;

typedef detail::interlocked_bool< long > bool_t;
typedef detail::interlocked_bool< long > bool_fast_t;

#endif

} } }  // namespace boost

//#endif

#endif

#endif
