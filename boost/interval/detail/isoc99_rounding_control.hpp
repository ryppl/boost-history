/* Boost interval/detail/isoc99_rounding_control.hpp file
 *
 * Copyright Jens Maurer 2000
 * Copyright Hervé Brönnimann, Guillaume Melquiond, Sylvain Pion 2002
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * None of the above authors nor Polytechnic University make any
 * representation about the suitability of this software for any
 * purpose. It is provided "as is" without express or implied warranty.
 *
 * $Id$
 *
 * Revision history:
 *   2002-08-31	 Prepared for boost formal review
 *   2000-09-24	 Separated from interval.hpp
 */

#ifndef BOOST_INTERVAL_DETAIL_ISOC99_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_ISOC99_ROUNDING_CONTROL_HPP

#include <cassert>
#include <fenv.h>      // ISO C 99 rounding mode control

namespace boost {
  namespace interval_lib {
    namespace detail {

extern "C" { double rint(double); }

struct isoc99_rounding_control
{
  typedef int rounding_mode;

  static void set_rounding_mode(const rounding_mode mode)
  {
    int ret = fesetround(mode);
    assert(ret == 0);   // do not fold this with the fesetround()
    (void) &ret;        // avoid "unused variable" warning if NDEBUG
  }
  static void get_rounding_mode(rounding_mode &mode) { mode = fegetround(); }
  static void downward()    { set_rounding_mode(FE_DOWNWARD);   }
  static void upward()      { set_rounding_mode(FE_UPWARD);     }
  static void to_nearest()  { set_rounding_mode(FE_TONEAREST);  }
  static void toward_zero() { set_rounding_mode(FE_TOWARDZERO); }

  template<class T>
  static T force_rounding(const T& r) { volatile T r_ = r; return r_; }

  template<class T>
  static T to_int(const T& r) { return rint(r); }
};

    } // namespace detail

template<>
struct rounding_control<float>:
  detail::isoc99_rounding_control { };

template<>
struct rounding_control<double>:
  detail::isoc99_rounding_control { };

template<>
struct rounding_control<long double>:
  detail::isoc99_rounding_control { };

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_ISOC99_ROUBDING_CONTROL_HPP */

