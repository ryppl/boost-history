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

#ifndef BOOST_DETAIL_ISOC99_ROUNDING_CONTROL_HPP
#define BOOST_DETAIL_ISOC99_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#include <cassert>
#include <fenv.h>      // ISO C 99 rounding mode control

namespace boost {
namespace detail {

class isoc99_rounding_control
{
  void set_round_mode(int mode) const
  {
    int ret = fesetround(mode);
    assert(ret == 0);   // do not fold this with the fesetround()
    (void) &ret;        // avoid "unused variable" warning if NDEBUG
  }
public:
  isoc99_rounding_control() : old_rm(fegetround()) { }
  ~isoc99_rounding_control() { fesetround(old_rm); }
#ifdef FE_DOWNWARD
  void downward() { set_round_mode(FE_DOWNWARD); }
#endif
#ifdef FE_UPWARD
  void upward() { set_round_mode(FE_UPWARD); }
#endif
#ifdef FE_TONEAREST
  void tonearest() { set_round_mode(FE_TONEAREST); }
#endif
#ifdef FE_TOWARDZERO
  void towardzero() { set_round_mode(FE_TOWARDZERO); }
#endif
private:
  int old_rm;
};

template<> struct rounding_control<float> : isoc99_rounding_control { };
template<> struct rounding_control<double> : isoc99_rounding_control { };
template<> struct rounding_control<long double> : isoc99_rounding_control { };

// signbit() and isnan() are macros in ISO C99.  However, at least the
// GNU C++ library has these as functions in namespace std.
#ifdef signbit
inline bool sign(float x) { return signbit(x); }
inline bool sign(double x) { return signbit(x); }
inline bool sign(long double x) { return signbit(x); }
#else
inline bool sign(float x) { return std::signbit(x); }
inline bool sign(double x) { return std::signbit(x); }
inline bool sign(long double x) { return std::signbit(x); }
#endif
inline bool is_nan(float x) { return isnan(x); }
inline bool is_nan(double x) { return isnan(x); }
inline bool is_nan(long double x) { return isnan(x); }

} // namespace detail
} // namespace boost

#endif /* BOOST_DETAIL_ISOC99_ROUBDING_CONTROL_HPP */

