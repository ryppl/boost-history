/* boost x86gcc_rounding_control.hpp header file
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

#ifndef BOOST_INTERVAL_DETAIL_X86GCC_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_X86GCC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef __GNUC__
#error This header only works with GNU CC.
#endif

#ifndef __i386__
#error This header only works on x86 CPUs.
#endif

namespace boost {
  namespace interval_lib {
    namespace detail {

/*
 * We could use immediate constants for the FPU control word values.
 * However, the fldcw assember instruction requires a memory
 * reference, so gcc 2.95.2 always copies the immediate to a temporary
 * on the stack.  This seems to incur a 5% performance penalty for the
 * basic interval arithmetic operations, thus we help the compiler by
 * defining these constants as global variables.  It is important that
 * the definition of the constants comes after the definition of class
 * rounding_control to prevent constant folding in the latter.  Using
 * a struct instead of separate variables makes sure that all values
 * are adjacent, thereby fitting in one line of the CPU cache.
 */

// namespace {
  struct fpu_rounding_modes
  {
    unsigned short default_ieee;
    unsigned short mode_downward;
    unsigned short mode_upward;
    unsigned short mode_towardzero;
  };
  extern const fpu_rounding_modes rounding_mode_data /*__attribute__((weak))*/;
// }

struct x86gcc_rounding_control
{
  // gcc requires the definition of set_round_mode before it is called
  // for proper optimization
  void set_rounding_mode(const unsigned short& mode) const
  {
    __asm__ __volatile__
      ("fldcw %0" : : "m" (*&mode) :
       "st","st(1)","st(2)","st(3)", "st(4)","st(5)","st(6)","st(7)");
  }
  unsigned short get_rounding_mode() const
  {
    unsigned short cw;
    __asm__ __volatile__ ("fnstcw %0" : "=m" (*&cw));
    return cw;
  }
  void downward() { set_rounding_mode(rounding_mode_data.mode_downward); }
  void upward() { set_rounding_mode(rounding_mode_data.mode_upward); }
  void tonearest() { set_rounding_mode(rounding_mode_data.default_ieee); }
  void towardzero() { set_rounding_mode(rounding_mode_data.mode_towardzero); }

  typedef unsigned short rounding_mode;
};

// namespace {
  // exceptions masked, extended precision
const fpu_rounding_modes rounding_mode_data = 
  { 0x137f, 0x177f, 0x1b7f, 0x1f7f };
// }

    } // namespace detail

template<>
struct rounding_control<float>: detail::x86gcc_rounding_control
{
  static float force_rounding(const float& r) 
  { volatile float r_ = r; return r_; }
};

template<>
struct rounding_control<double>: detail::x86gcc_rounding_control
{
  static double force_rounding(const double& r) 
  { volatile double r_ = r; return r_; }
};

template<>
struct rounding_control<long double>: detail::x86gcc_rounding_control
{
  static long double force_rounding(const long double& r) { return r; }
};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_X86GCC_ROUNDING_CONTROL_HPP */
