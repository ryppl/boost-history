/* boost bcc_rounding_control.hpp header file
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
 */

#ifndef BOOST_DETAIL_BCC_ROUNDING_CONTROL_HPP
#define BOOST_DETAIL_BCC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef __BORLANDC__
#error This header is only intended for Borland C++.
#endif

#ifndef _M_IX86
#error This header only works on x86 CPUs.
#endif

#include <float.h>      // Borland C++ rounding control

namespace boost {
namespace detail {

/*
 * If __STDC__ is not defined, <float.h> defines some constants for
 * use with _control87.  These macros do not start with an underscore
 * and thus pollute the user's namespace.  If __STDC__ is defined, no
 * such macros are defined at all.  We use our own constants in all
 * cases and check for a x86 CPU (see above) to be on the safe side.
 */
class bcc_rounding_control
{
  static const unsigned int mask_rc = 0x0c00;
  static const unsigned int rc_down = 0x0400;
  static const unsigned int rc_up = 0x0800;
  static const unsigned int rc_near = 0x0000;
  static const unsigned int rc_chop = 0x0c00;
public:
  bcc_rounding_control() { old_rm = _control87(0,0); }
  ~bcc_rounding_control() { _control87(old_rm, mask_rc); }
  void downward()       { _control87(rc_down, mask_rc); }
  void upward()         { _control87(rc_up, mask_rc); }
  void tonearest()      { _control87(rc_near, mask_rc); }
  void towardzero()     { _control87(rc_chop, mask_rc); }
private:
  unsigned int old_rm;
};

template<> struct rounding_control<float> : bcc_rounding_control { };
template<> struct rounding_control<double> : bcc_rounding_control { };
template<> struct rounding_control<long double> : bcc_rounding_control { };

inline bool is_nan(double x) { return _isnan(x); }
inline bool is_nan(long double x) { return _isnanl(x); }

} // namespace detail
} // namespace boost

#endif /* BOOST_DETAIL_BCC_ROUNDING_CONTROL_HPP */
