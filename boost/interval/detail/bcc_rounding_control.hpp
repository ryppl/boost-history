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

#ifndef BOOST_INTERVAL_DETAIL_BCC_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_BCC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#  error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef __BORLANDC__
#  error This header is only intended for Borland C++.
#endif

#ifndef _M_IX86
#  error This header only works on x86 CPUs.
#endif

#include <float.h>      // Borland C++ rounding control

namespace boost {
  namespace interval_lib {
    namespace detail {

extern "C" { double rint(double); }

/*
 * If __STDC__ is not defined, <float.h> defines some constants for
 * use with _control87.  These macros do not start with an underscore
 * and thus pollute the user's namespace.  If __STDC__ is defined, no
 * such macros are defined at all.  We use our own constants in all
 * cases and check for a x86 CPU (see above) to be on the safe side.
 */
struct bcc_rounding_control
{
private:
  static const unsigned int mask_rc = 0x0c00;
  static const unsigned int rc_down = 0x0400;
  static const unsigned int rc_up   = 0x0800;
  static const unsigned int rc_near = 0x0000;
  static const unsigned int rc_chop = 0x0c00;
public:
  typedef unsigned int rounding_mode;
  static rounding_mode get_rounding_mode() { return _controlfp(0,0); }
  static void set_rounding_mode(const rounding_mode mode)
  { _controlfp(mode, mask_rc); } 
  static void downward()   { _control87(rc_down, mask_rc); }
  static void upward()     { _control87(rc_up,   mask_rc); }
  static void tonearest()  { _control87(rc_near, mask_rc); }
  static void towardzero() { _control87(rc_chop, mask_rc); }
  static double to_int(const double& x) { return rint(x); }
};

    } // namespace detail
  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_BCC_ROUNDING_CONTROL_HPP */
