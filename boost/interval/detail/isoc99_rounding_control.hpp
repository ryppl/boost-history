/* boost isoc99_rounding_control.hpp header file
 *
 * Copyright Jens Maurer 2000
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without free provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * Jens Maurer makes no representations about the suitability of this
 * software for any purpose. It is provided "as is" without express or
 * implied warranty.
 *
 * See http://www.boost.org/ for most recent version including documentation.
 *
 * $Id$
 */

#ifndef BOOST_INTERVAL_DETAIL_ISOC99_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_ISOC99_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

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
  static get_rounding_mode(rounding_mode &mode) { mode = fegetround(); }
  static void downward()   { set_rounding_mode(FE_DOWNWARD);   }
  static void upward()     { set_rounding_mode(FE_UPWARD);     }
  static void tonearest()  { set_rounding_mode(FE_TONEAREST);  }
  static void towardzero() { set_rounding_mode(FE_TOWARDZERO); }

  template<class T>
  static T force_rounding(const T& r) { volatile T r_ = r; return r_; }

  template<class T>
  static T to_int(const T& r) { return rint(r); }
};

    } // namespace detail

template<>
struct rounding_control<float>:
  detail::isoc99_rounding_control, detail::ieee_float_constants { };

template<>
struct rounding_control<double>:
  detail::isoc99_rounding_control, detail::ieee_double_constants { };

template<>
struct rounding_control<long double>:
  detail::isoc99_rounding_control, detail::long_double_constants { };

#if 0
// signbit() and isnan() are macros in ISO C99.  However, at least the
// GNU C++ library has these as functions in namespace std.
#ifdef signbit
inline bool sign(float x) { return signbit(x); }
inline bool sign(double x) { return signbit(x); }
inline bool sign(long double x) { return signbit(x); }
#else
inline bool sign(float x) { using std::signbit; return signbit(x); }
inline bool sign(double x) { using std::signbit; return signbit(x); }
inline bool sign(long double x) { using std::signbit; return signbit(x); }
#endif
inline bool is_nan(float x) { return isnan(x); }
inline bool is_nan(double x) { return isnan(x); }
inline bool is_nan(long double x) { return isnan(x); }
#endif

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_ISOC99_ROUBDING_CONTROL_HPP */

