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

#ifndef BOOST_DETAIL_MSVC_ROUNDING_CONTROL_HPP
#define BOOST_DETAIL_MSVC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef _MSC_VER
#error This header is only intended for MSVC, but might work for Borland as well
#endif

#include <float.h>      // MSVC rounding control

namespace boost {
namespace detail {

class msvc_rounding_control
{
public:
  msvc_rounding_control() { old_rm = _controlfp(0,0); }
  ~msvc_rounding_control() { _controlfp(old_rm, _MCW_RC); } 
  void downward()       { _controlfp(_RC_DOWN, _MCW_RC); }
  void upward()         { _controlfp(_RC_UP, _MCW_RC); }
  void tonearest()      { _controlfp(_RC_NEAR, _MCW_RC); }
  void towardzero()     { _controlfp(_RC_CHOP, _MCW_RC); }
private:
  unsigned int old_rm;
};

template<> struct rounding_control<float> : msvc_rounding_control { };
template<> struct rounding_control<double> : msvc_rounding_control { };
template<> struct rounding_control<long double> : msvc_rounding_control { };

inline bool is_nan(double x) { return _isnan(x); }

} // namespace detail
} // namespace boost

#endif /* BOOST_DETAIL_MSVC_ROUNDING_CONTROL_HPP */
