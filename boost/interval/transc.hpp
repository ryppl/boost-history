/* Boost interval/transc.hpp template implementation file
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

#ifndef BOOST_INTERVAL_TRANSC_HPP
#define BOOST_INTERVAL_TRANSC_HPP

#include <boost/interval/detail/interval_prototype.hpp>
#include <boost/interval/detail/bugs.hpp>
#include <boost/interval/detail/test_input.hpp>
#include <boost/interval/rounding.hpp>
#include <boost/interval/constants.hpp>
#include <boost/interval/arith.hpp>
#include <boost/interval/arith2.hpp>
#include <algorithm>

namespace boost {

template<class T, class Traits> inline
interval<T, Traits> exp(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.exp_down(x.lower()),
			     rnd.exp_up  (x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> log(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x) || x.upper() <= T(0))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename Traits::checking checking;
  T l = (x.lower() <= T(0)) ? -checking::inf() : rnd.log_down(x.lower());
  return interval<T, Traits>(l, rnd.log_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> cos(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;

  // get lower bound within [0, pi]
  const I pi2 = interval_lib::pi_twice<I>();
  I tmp = fmod((const I&)x, pi2);
  if (width(tmp) >= pi2.lower())
    return interval<T, Traits>(-1, 1, true);   // we are covering a full period
  if (tmp.lower() >= interval_lib::constants::pi_upper<T>())
    return -cos(tmp - interval_lib::pi<I>());
  T l = tmp.lower();
  T u = tmp.upper();

  using std::min;

  // separate into monotone subintervals
  if (u <= interval_lib::constants::pi_lower<T>())
    return interval<T, Traits>(rnd.cos_down(u), rnd.cos_up(l), true);
  else if (u <= pi2.lower()) {
    T cu = rnd.cos_up(min(rnd.sub_down(pi2.lower(), u), l));
    return interval<T, Traits>(-1, cu, true);
  } else
    return interval<T, Traits>(-1, 1, true);
}

template<class T, class Traits> inline
interval<T, Traits> sin(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  interval<T, Traits> r = cos((const I&)x - interval_lib::pi_half<I>());
  (void)&rnd;
  return r;
}

template<class T, class Traits> inline
interval<T, Traits> tan(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;

  // get lower bound within [-pi/2, pi/2]
  const I pi = interval_lib::pi<I>();
  I tmp = fmod((const I&)x, pi);
  const T pi_half_d = interval_lib::constants::pi_half_lower<T>();
  if (tmp.lower() >= pi_half_d)
    tmp -= pi;
  if (tmp.lower() <= -pi_half_d || tmp.upper() >= pi_half_d)
    return interval<T, Traits>::whole();
  return interval<T, Traits>(rnd.tan_down(tmp.lower()),
			     rnd.tan_up  (tmp.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> asin(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x)
     || x.upper() < T(-1) || x.lower() > T(1))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  T l = (x.lower() <= T(-1))
	     ? -interval_lib::constants::pi_half_upper<T>()
	     : rnd.asin_down(x.lower());
  T u = (x.upper() >= T(1) )
	     ?  interval_lib::constants::pi_half_upper<T>()
	     : rnd.asin_up  (x.upper());
  return interval<T, Traits>(l, u, true);
}

template<class T, class Traits> inline
interval<T, Traits> acos(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x)
     || x.upper() < T(-1) || x.lower() > T(1))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  T l = (x.upper() >= T(1) )
	  ? 0
	  : rnd.acos_down(x.upper());
  T u = (x.lower() <= T(-1))
	  ? interval_lib::constants::pi_upper<T>()
	  : rnd.acos_up  (x.lower());
  return interval<T, Traits>(l, u, true);
}

template<class T, class Traits> inline
interval<T, Traits> atan(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.atan_down(x.lower()),
			     rnd.atan_up  (x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> sinh(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.sinh_down(x.lower()),
			     rnd.sinh_up  (x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> cosh(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typedef interval<T, Traits> I;
  typename Traits::rounding rnd;
  if (detail::is_neg(x.upper()))
    return I(rnd.cosh_down(x.upper()), rnd.cosh_up(x.lower()), true);
  else if (!detail::is_neg(x.lower()))
    return I(rnd.cosh_down(x.lower()), rnd.cosh_up(x.upper()), true);
  else
    return I(0,
             rnd.cosh_up(-x.lower() > x.upper() ? x.lower() : x.upper()),
	     true);
}

template<class T, class Traits> inline
interval<T, Traits> tanh(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.tanh_down(x.lower()),
			     rnd.tanh_up  (x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> asinh(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.asinh_down(x.lower()),
			     rnd.asinh_up  (x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> acosh(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x) || x.upper() < T(1))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  T l = x.lower() <= T(1) ? 0 : rnd.acosh_down(x.lower());
  return interval<T, Traits>(l, rnd.acosh_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> atanh(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x)
     || x.upper() < T(-1) || x.lower() > T(1))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename Traits::checking checking;
  T l = (x.lower() <= T(-1)) ? -checking::inf() : rnd.atanh_down(x.lower());
  T u = (x.upper() >= T(1) ) ?  checking::inf() : rnd.atanh_up  (x.upper());
  return interval<T, Traits>(l, u, true);
}

} // namespace boost

#endif // BOOST_INTERVAL_TRANSC_HPP
