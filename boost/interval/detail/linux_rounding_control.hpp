/* boost linux_rounding_control.hpp header file
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

#ifndef BOOST_INTERVAL_DETAIL_LINUX_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_LINUX_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

/*
 * Note that the Linux glibc 2.1.2 _FPU_SETCW macro provided in the
 * <fpu_control.h> header does not seem to prevent instruction
 * reordering across rounding mode changes while optimizing.  Thus, we
 * use our own assembler code here which tells gcc to spill all FPU
 * registers to main memory across rounding mode changes.
 *
 * Since we do it ourselves anyway, this is the generic x86/gcc
 * rounding mode control (not Linux-specific).
 */

#ifdef __GNUC__

#ifdef __i386__
#include <boost/interval/detail/x86gcc_rounding_control.hpp>
#elif __powerpc__
#include <boost/interval/detail/ppc_rounding_control.hpp>
#else
#error Unsupported CPU type.
#endif

#else
#error Unsupported C++ compiler.
#endif

namespace boost {
  namespace detail {

#ifndef __STRICT_ANSI__
// use Linux' BSD 4.3 emulation
inline bool is_nan(float x) { return isnanf(x); }
inline bool is_nan(double x) { return isnan(x); }
inline bool is_nan(long double x) { return isnanl(x); }
#endif

  } // namespace detail
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_LINUX_ROUNDING_CONTROL_HPP */
