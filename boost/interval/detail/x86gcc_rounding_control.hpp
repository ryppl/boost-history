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

struct x86_rounding
{
  typedef unsigned short rounding_mode;

  static void set_rounding_mode(const rounding_mode& mode)
  { __asm__ __volatile__ ("fldcw %0" : : "m"(mode)); }

  static void get_rounding_mode(rounding_mode& mode)
  { __asm__ __volatile__ ("fnstcw %0" : "=m"(mode)); }

  template<class T>
  static T to_int(T r)
  {
    T r_;
    __asm__ ("frndint" : "=&t"(r_) : "0"(r));
    return r_;
  }
};

    } // namespace detail
  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_X86GCC_ROUNDING_CONTROL_HPP */
