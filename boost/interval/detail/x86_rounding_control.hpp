/* Boost interval/detail/x86_rounding_control.hpp file
 *
 * Copyright Jens Maurer 2000
 * Copyright Hervé Brönnimann, Guillaume Melquiond, Sylvain Pion 2002
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * None of the above authors nor Polytechnic University make any
 * representation about the suitability of this software for any
 * purpose. It is provided "as is" without express or implied warranty.
 *
 * $Id$
 *
 * Revision history:
 *   2002-08-31	 Prepared for boost formal review
 *   2000-09-24	 Separated from interval.hpp
 */

#ifndef BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#  error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifdef __GNUC__
#  include <boost/interval/detail/x86gcc_rounding_control.hpp>
#elif __BORLANDC__
#  include <boost/interval/detail/bcc_rounding_control.hpp>
#elif _MSC_VER
#  include <boost/interval/detail/msvc_rounding_control.hpp>
#else
#  error Unsupported C++ compiler.
#endif

namespace boost {
  namespace interval_lib {

    namespace detail {

struct fpu_rounding_modes
{
  unsigned short to_nearest;
  unsigned short downward;
  unsigned short upward;
  unsigned short toward_zero;
};

// exceptions masked, extended precision
// hardware default is 0x037f (0x1000 only has a meaning on 287)
static const fpu_rounding_modes rnd_mode = { 0x137f, 0x177f, 0x1b7f, 0x1f7f };

struct x86_rounding_control: x86_rounding
{
  static void to_nearest()  { set_rounding_mode(rnd_mode.to_nearest);  }
  static void downward()    { set_rounding_mode(rnd_mode.downward);    }
  static void upward()      { set_rounding_mode(rnd_mode.upward);      }
  static void toward_zero() { set_rounding_mode(rnd_mode.toward_zero); }
};

    } // namespace detail

template<>
struct rounding_control<float>: detail::x86_rounding_control
{
  static float force_rounding(const float& r) 
  { volatile float r_ = r; return r_; }
};

template<>
struct rounding_control<double>: detail::x86_rounding_control
{
  static double force_rounding(const double& r) 
  { volatile double r_ = r; return r_; }
};

template<>
struct rounding_control<long double>: detail::x86_rounding_control
{
  static const long double& force_rounding(const long double& r) { return r; }
};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP */
