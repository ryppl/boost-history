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
 * The basic code in this file was kindly provided by Jeremy Siek.
 *
 * See http://www.boost.org/ for most recent version including documentation.
 *
 * $Id$
 */

#ifndef BOOST_INTERVAL_DETAIL_SPARC_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_SPARC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#if !defined(sparc) && !defined(__sparc__)
#error This header is only intended for SPARC CPUs.
#endif


namespace boost {
  namespace interval_lib {
    namespace detail {

struct sparc_rounding_control
{
  // gcc requires this before the calls below for proper optimization
  void set_rounding_mode(unsigned int mode)
  {
#if defined(__GNUC__)
    __asm__ __volatile__("ld %0, %%fsr" : : "m" (mode));
#elif defined(__KCC)
    asm("sethi %hi(mode), %o1");
    asm("ld [%o1+%lo(mode)], %fsr");
#else
#error Unsupported compiler for Sun Solaris rounding control.
#endif
  }
  unsigned int get_rounding_mode()
  {
    unsigned int mode;
#if defined(__GNUC__)
    __asm__ __volatile__("st %%fsr, %0" : "=m" (mode));
#elif defined(__KCC)
#error KCC on Sun SPARC get_round_mode: please fix me
    asm("st %fsr, [mode]");
#else
#error Unsupported compiler for Sun Solaris rounding control.
#endif
    return mode;
  }
  void downward() { set_rounding_mode(0xc0000000); }
  void upward() { set_rounding_mode(0x80000000); }
  void tonearest() { set_rounding_mode(0x00000000); }
  void towardzero() { set_rounding_mode(0x40000000); }
  static double force_rounding(const double& x) { return x; }
  typedef unsigned int rounding_mode;
};

    } // namespace detail

template<>
struct rounding_control<float>: detail::sparc_rounding_control
{
private:
  rounding_control() {}
};

template<>
struct rounding_control<double>: detail::sparc_rounding_control
{};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_SPARC_ROUNDING_CONTROL_HPP */
