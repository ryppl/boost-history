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
#  error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef __GNUC__
#  error This header only works with GNU CC.
#endif

#ifndef __i386__
#  error This header only works on x86 CPUs.
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
    unsigned short tonearest;
    unsigned short downward;
    unsigned short upward;
    unsigned short towardzero;
  };
  extern const fpu_rounding_modes rnd_mode /*__attribute__((weak))*/;
// }

struct x86_rounding_control
{
  typedef unsigned short rounding_mode;

  static void set_rounding_mode(const rounding_mode& mode)
  {
    __asm__ __volatile__ ("fldcw %0" : : "m"(mode));
  }

  static rounding_mode get_rounding_mode()
  {
    rounding_mode tmp;
    __asm__ __volatile__ ("fnstcw %0" : "=m"(tmp));
    return tmp;
  }

  static void downward()   { set_rounding_mode(rnd_mode.downward);   }
  static void upward()     { set_rounding_mode(rnd_mode.upward);     }
  static void tonearest()  { set_rounding_mode(rnd_mode.tonearest);  }
  static void towardzero() { set_rounding_mode(rnd_mode.towardzero); }

  template<class T>
  static T to_int(T r)
  {
    T r_;
    __asm__ ("frndint" : "=&t"(r_) : "0"(r));
    return r_;
  }
};

// exceptions masked, extended precision
const fpu_rounding_modes rnd_mode = { 0x137f, 0x177f, 0x1b7f, 0x1f7f };

    } // namespace detail
  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_X86GCC_ROUNDING_CONTROL_HPP */
