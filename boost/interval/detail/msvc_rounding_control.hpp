/* boost msvc_rounding_control.hpp header file
 *
 * Copyright Maarten Keijzer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without free provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Maarten Keijzer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * See http://www.boost.org/ for most recent version including documentation.
 *
 */

#ifndef BOOST_INTERVAL_DETAIL_MSVC_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_MSVC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef _MSC_VER
#error This header is only intended for MSVC, but might work for Borland as well
#endif

#include <float.h>      // MSVC rounding control

namespace boost {
  namespace interval_lib {
    namespace detail {

struct msvc_rounding_control
{
  typedef unsigned int rounding_mode;
  static rounding_mode get_rounding_mode() { return _controlfp(0,0); }
  static void set_rounding_mode(const rounding_mode mode)
  { _controlfp(mode, _MCW_RC); } 
  static void downward()       { _controlfp(_RC_DOWN, _MCW_RC); }
  static void upward()         { _controlfp(_RC_UP,   _MCW_RC); }
  static void tonearest()      { _controlfp(_RC_NEAR, _MCW_RC); }
  static void towardzero()     { _controlfp(_RC_CHOP, _MCW_RC); }
};

    } // namespace detail

template<>
struct rounding_control<float>: detail::msvc_rounding_control { };
template<>
struct rounding_control<double>: detail::msvc_rounding_control { };
template<>
struct rounding_control<long double>: detail::msvc_rounding_control { };

  } // namespace interval_lib
} // namespace boost

// force_rounding and to_int needed
#error Please adapt

#endif /* BOOST_INTERVAL_DETAIL_MSVC_ROUNDING_CONTROL_HPP */
