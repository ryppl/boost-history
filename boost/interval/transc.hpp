/* boost interval/transc.hpp template implementation file
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
 * $Id$
 *
 * Revision History
 *  2000-09-24  separated from interval.hpp
 */

#ifndef BOOST_INTERVAL_TRANSC_HPP
#define BOOST_INTERVAL_TRANSC_HPP

#include <cmath>

#if defined(__GLIBC__) && !defined(__GLIBCPP__) && (defined(__USE_MISC) || defined(__USE_XOPEN_EXTENDED) || defined(__USE_ISOC99))
#define BOOST_HAVE_INV_HYPERBOLIC
#endif

namespace boost {

template<class T, class Traits> inline
interval<T, Traits> fmod(const interval<T, Traits>& x,
			 const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  const T& yb = detail::sign(x.lower()) ? y.lower() : y.upper();
  T n = rnd.int_down(rnd.div_down(x.lower(), yb));
  return (const I&)x - n * (const I&)y;
}

template<class T, class Traits> inline
interval<T, Traits> fmod(const interval<T, Traits>& x, const T& y)
{
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  T n = rnd.int_down(rnd.div_down(x.lower(), y));
  return (const I&)x - n * I(y);
}

template<class T, class Traits> inline
interval<T, Traits> fmod(const T& x, const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  const T& yb = detail::sign(x) ? y.lower() : y.upper();
  T n = rnd.int_down(rnd.div_down(x, yb));
  return x - n * (const I&)y;
}

template<class T, class Traits> inline
interval<T, Traits> exp(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.exp_down(x.lower()),
			     rnd.exp_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> log(const interval<T, Traits>& x)
{
  typedef typename Traits::checking checking;
  if (x.upper() <= T(0)) {
    checking::logarithmic_nan();
    return interval<T, Traits>::empty();
  }
  typename Traits::rounding rnd;
  T l;
  if (x.lower() <= T(0)) {
    checking::logarithmic_inf();
    l = -std::numeric_limits<T>::infinity();
  } else {
    l = rnd.log_down(x.lower());
  }
  return interval<T, Traits>(l, rnd.log_up(x.upper()), true);
}

  namespace interval_lib {
    namespace detail {

template<class T, class Rounding> inline
T pow_aux(T x, int pwr, Rounding& rnd) // x and pwr are positive
{
  T y = 1;
  while (pwr > 0) {
    if (pwr & 1) y = rnd.mul_up(x, y);
    pwr >>= 1;
    if (pwr > 0) x = rnd.mul_up(x, x);
  }
  return y;
}

    } // namespace detail
  } // namespace interval_lib

template<class T, class Traits> inline
interval<T, Traits> pow(const interval<T, Traits>& x, int pwr)
{
  if (pwr == 0) {
    return interval<T, Traits>(T(1));
  } else if (pwr < 0) {
    return T(1) / pow(x, -pwr);
  }

  using interval_lib::detail::pow_aux;

  typename Traits::rounding rnd;
  bool sgnl = detail::sign(x.lower());
  bool sgnu = detail::sign(x.upper());
  bool odd_pwr = pwr & 1;
  
  if (sgnl && !sgnu && !odd_pwr) {
    T y = pow_aux(std::max(-x.lower(), x.upper()), pwr, rnd);
    return interval<T, Traits>(0, y, true);
  } else if (sgnu) {
    T yl = pow_aux(-x.upper(), pwr, rnd);
    T yu = pow_aux(-x.lower(), pwr, rnd);
    if (odd_pwr)
      return interval<T, Traits>(-yu, -yl, true);
    else
      return interval<T, Traits>(yl, yu, true);
  } else {
    T y = sgnl ? -pow_aux(-x.lower(), pwr, rnd) // since odd_pwr
               : pow_aux(x.lower(), pwr, rnd);
    return interval<T, Traits>(y, pow_aux(x.upper(), pwr, rnd), true);
  }
}

template<class T, class Traits> inline
interval<T, Traits> cos(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;

  // get lower bound within [0, pi]
  const I pi2 = interval_lib::pi_2_1<I>();
  I tmp = fmod((const I&)x, pi2);
  if (width(tmp) >= pi2.lower())
    return interval<T, Traits>(-1, 1, true);     // we are covering a full period
  if (tmp.lower() >= rnd.pi_up())
    return -cos(tmp - interval_lib::pi<I>());
  T l = tmp.lower();
  T u = tmp.upper();

  // separate into monotone subintervals
  if (u <= rnd.pi_down())
    return interval<T, Traits>(rnd.cos_down(u), rnd.cos_up(l), true);
  else if (u <= pi2.lower()) {
    T cu = rnd.cos_up(std::min(rnd.sub_down(pi2.lower(), u), l));
    return interval<T, Traits>(-1, cu, true);
  } else
    return interval<T, Traits>(-1, 1, true);
}

template<class T, class Traits> inline
interval<T, Traits> sin(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  return cos((const I&)x - interval_lib::pi_1_2<I>());
}

template<class T, class Traits> inline
interval<T, Traits> tan(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;

  // get lower bound within [-pi/2, pi/2]
  const I pi = interval_lib::pi<I>();
  I tmp = fmod((const I&)x, pi);
  T pi_1_2_d = rnd.pi_1_2_down();
  if (tmp.lower() >= pi_1_2_d)
    tmp -= pi;
  if (tmp.lower() <= -pi_1_2_d || tmp.upper() >= pi_1_2_d) {
    typedef typename Traits::checking checking;
    checking::trigonometric_inf();
    return interval<T, Traits>::whole();
  }
  return interval<T, Traits>(rnd.tan_down(tmp.lower()),
			     rnd.tan_up(tmp.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> asin(const interval<T, Traits>& x)
{
  if (x.upper() < T(-1) || x.lower() > T(1)) {
    typedef typename Traits::checking checking;
    checking::trigonometric_nan();
    return interval<T, Traits>::empty();
  }
  typename Traits::rounding rnd;
  T l = x.lower() <= T(-1) ? -rnd.pi_1_2_up() : rnd.asin_down(x.lower());
  T u = x.upper() >= T(1) ? rnd.pi_1_2_up() : rnd.asin_up(x.upper());
  return interval<T, Traits>(l, u, true);
}

template<class T, class Traits> inline
interval<T, Traits> acos(const interval<T, Traits>& x)
{
  if (x.upper() < T(-1) || x.lower() > T(1)) {
    typedef typename Traits::checking checking;
    checking::trigonometric_nan();
    return interval<T, Traits>::empty();
  }
  typename Traits::rounding rnd;
  T l = x.upper() >= T(1) ? 0 : rnd.acos_down(x.upper());
  T u = x.lower() <= T(-1) ? rnd.pi_up() : rnd.acos_up(x.lower());
  return interval<T, Traits>(l, u, true);
}

template<class T, class Traits> inline
interval<T, Traits> atan(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.atan_down(x.lower()),
			     rnd.atan_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> sinh(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.sinh_down(x.lower()),
			     rnd.sinh_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> cosh(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  typename Traits::rounding rnd;
  if (detail::sign(x.upper()))
    return I(rnd.cosh_down(x.upper()), rnd.cosh_up(x.lower()), true);
  else if (!detail::sign(x.lower()))
    return I(rnd.cosh_down(x.lower()), rnd.cosh_up(x.upper()), true);
  else
    return I(0, rnd.cosh_up(-x.lower() > x.upper() ? x.lower() : x.upper()),
	     true);
}

template<class T, class Traits> inline
interval<T, Traits> tanh(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.tanh_down(x.lower()),
			     rnd.tanh_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> asinh(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.asinh_down(x.lower()),
			     rnd.asinh_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> acosh(const interval<T, Traits>& x)
{
  if (x.upper() < T(1)) {
    typedef typename Traits::checking checking;
    checking::hyperbolic_nan();
    return interval<T, Traits>::empty();
  }
  typename Traits::rounding rnd;
  T l = x.lower() <= T(1) ? 0 : rnd.acosh_down(x.lower());
  return interval<T, Traits>(l, rnd.acosh_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> atanh(const interval<T, Traits>& x)
{
  typedef typename Traits::checking checking;
  if (x.upper() < T(-1) || x.lower() > T(1)) {
    checking::hyperbolic_nan();
    return interval<T, Traits>::empty();
  }
  typename Traits::rounding rnd;
  T l, u;
  if (x.lower() <= T(-1)) {
    checking::hyperbolic_inf();
    l = -std::numeric_limits<T>::infinity();
  } else {
    l = rnd.atanh_down(x.lower());
  }
  if (x.upper() >= T(1)) {
    checking::hyperbolic_inf();
    u = std::numeric_limits<T>::infinity();
  } else {
    u = rnd.atanh_up(x.upper());
  }
  return interval<T, Traits>(l, u, true);
}

} // namespace boost

#endif // BOOST_INTERVAL_TRANSC_HPP
