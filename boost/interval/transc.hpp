/* boost detail/interval_transc.hpp template implementation file
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
#include <boost/cast.hpp>         // boost::numeric_cast<>

#if defined(__GLIBC__) && !defined(__GLIBCPP__) && (defined(__USE_MISC) || defined(__USE_XOPEN_EXTENDED) || defined(__USE_ISOC99))
#define BOOST_HAVE_INV_HYPERBOLIC
#endif

namespace std {
#ifdef BOOST_NO_STDC_NAMESPACE
  using ::sqrt;
  using ::pow;
  using ::exp;
  using ::log;
  using ::log10;
  using ::sin;
  using ::tan;
  using ::asin;
  using ::acos;
  using ::atan;
  using ::ceil;
  using ::floor;
  using ::sinh;
  using ::cosh;
  using ::tanh;
#endif

#if defined(BOOST_HAVE_INV_HYPERBOLIC) && (defined(BOOST_NO_STDC_NAMESPACE) || defined(__SGI_STL_PORT))
  using ::asinh;
  using ::acosh;
  using ::atanh;
#endif
} // namespace std


namespace boost {

#ifdef __GNUC__
// Special gcc workaround: gcc does not yet support using-declarations
// in template classes (confirmed by gcc author Martin v. Loewis)
using std::sqrt;
using std::pow;
using std::exp;
using std::log;
using std::log10;
using std::sin;
using std::tan;
using std::asin;
using std::acos;
using std::atan;
using std::ceil;
using std::floor;
using std::sinh;
using std::cosh;
using std::tanh;
#if defined(BOOST_HAVE_INV_HYPERBOLIC)
using std::asinh;
using std::acosh;
using std::atanh;
#endif
#endif


template<class T, class Traits>
// fmod() is crucial for the well-behavior of sin, cos, tan:
// We have only an imprecise pi for large n.
inline interval<T, Traits> fmod(const interval<T, Traits>& x,
				const interval<T, Traits>& y)
{
  using std::ceil;
  using std::floor;
  interval<T, Traits> q = x/y;
  T n = 0;       // default if in(0,q)
  if(detail::sign(q.upper()))
    n = ceil(q.upper());
  else if(!detail::sign(q.lower()))
    n = floor(q.lower());
  return x - interval<T,Traits>(n) * y;
  //return x - n * y;
}

template<class T, class Traits>
interval<T, Traits> fmod(const interval<T, Traits>& x, T y)
{
  using std::ceil;
  using std::floor;
  interval<T, Traits> q = x/y;
  T n = 0;       // default if in(0,q)
  if(detail::sign(q.upper()))
    n = ceil(q.upper());
  else if(!detail::sign(q.lower()))
    n = floor(q.lower());
  return x - interval<T,Traits>(n) * y;
  //return x - n * y;
}

template<class T, class Traits>
interval<T, Traits> exp(const interval<T, Traits>& x)
{
  using std::exp;
  typename Traits::rounding rnd;
  rnd.downward();
  T l = exp(x.lower());
  rnd.upward();
  return interval<T, Traits>(l,exp(x.upper()), true);
}

// Borland C++ requires this work-around (but only for log!)
namespace detail {
  template<class T>
  inline T compute_log(const T & x)
  {
    using std::log;
    return log(x);
  }
} // namespace detail

template<class T, class Traits>
interval<T, Traits> log(const interval<T, Traits>& x)
{
  typedef typename Traits::checking checking;
  if (x.lower() < T(0)) checking::logarithmic_nan();
  if (x.lower() == T(0)) checking::logarithmic_inf();
  if(x.upper() < T(0))
    return interval<T, Traits>::empty();

  using std::log;
  typename Traits::rounding rnd;
  T l;
  if(x.lower() <= T(0)) {
      l = -std::numeric_limits<T>::infinity();
  } else {
    rnd.downward();
    l = detail::compute_log(x.lower());
  }
  rnd.upward();
  return interval<T, Traits>(l,detail::compute_log(x.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> log10(const interval<T, Traits>& x)
{
  typedef typename Traits::checking checking;
  if (x.lower() < T(0)) checking::logarithmic_nan();
  if (x.lower() == T(0)) checking::logarithmic_inf();
  if(detail::sign(x.upper()))
    return interval<T, Traits>::empty();

  using std::log10;
  typename Traits::rounding rnd;
  T l;
  if(x.lower() <= T(0)) {
    l = -std::numeric_limits<T>::infinity();
  } else {
    rnd.downward();
    l = log10(x.lower());
  }
  rnd.upward();
  return interval<T, Traits>(l,log10(x.upper()), true);
}

template<class T>
T pow(T x, long y)
{
  bool sign = false;
  if(y < 0) {
    sign = true;
    y = -y;
  }
  // Russian peasant's rule
  T result = 1;
  while(y > 0) {
    if(y % 2 == 0) {
      x = square(x);
      y /= 2;
    } else {
      result *= x;
      --y;
    }
  }
  return (sign ? T(1)/result : result);
}

template<class T, class Traits>
interval<T, Traits> pow(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  if (in_zero(x) && detail::sign(y.lower())) {
    typedef typename Traits::checking checking;
    checking::logarithmic_nan();
  }
  // TODO: nan_exception etc.
  typedef interval<T, Traits> I;
  if(empty(x) || empty(y))
    return I::empty();
  if(y.lower() == 0 && y.upper() == 0) {                // special case S0
    if(in(0, x))
      return I::entire();
    else if (detail::sign(x.upper()))
      return I(-1,1, true);
    else
      return I(1,1, true);
  } else if(x.lower() == 0 && x.upper() == 0)
    return I(0,0, true);
  
  using std::pow;
  if (detail::sign(x.lower())) {
    if (y.lower() == y.upper() && ceil(y.lower()) == y.lower()) {
      int yint = numeric_cast<long>(ceil(y.upper()));
      if (!detail::sign(x.upper()) && yint <= -1) {    // special case S1
	if (yint % 2 == 0) {                    // yint is even
	  typename Traits::rounding rnd;
	  rnd.downward();
	  return I(pow(x.upper() == 0 ? x.lower() : 
		       std::min(-x.lower(), x.upper()), yint),
		   std::numeric_limits<T>::infinity(), true);
	} else if (x.upper() == 0) {
	  typename Traits::rounding rnd;
	  rnd.upward();
	  return I(-std::numeric_limits<T>::infinity(),
		   pow(x.lower(), yint), true);
	} else {
	  return I::entire();
	}
      } else {
	return -exp(log(-x)*y);
      }
    } else if (y.lower() < y.upper()) {                // special case S2
      if (detail::sign(x.upper())) {
	I res = exp(log(-x)*y);
	return I(-res.upper(), res.upper(), true);
      } else if (!detail::sign(y.lower())) {
	using std::pow;
	typename Traits::rounding rnd;
	rnd.downward();
	T l = std::min(-pow(-x.lower(), y.lower()),
		       pow(-x.lower(), y.upper()));
	rnd.upward();
	return I(l, std::max(-l, std::max(pow(x.upper(), y.lower()),
					  pow(x.upper(), y.upper()))), true);
      } else {
	return I::entire();
      }
    } else {
      return I::entire();
    }
  } else {
    return exp(log(x)*y);
  }
}


// trigonometric functions

template<class T, class Traits>
interval<T, Traits> sin(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  if(width(x) >= 2*math::pi)
    return I(-1,1, true);     // we are covering a full period

  // get us within [-pi/2, 5/2*pi]
  I tmp = fmod(x, I(2.0*math::pi));
  if(tmp.lower() >= 3.0*math::pi/2.0)
    return -sin(tmp - T(math::pi));
  while(tmp.lower() <= -math::pi/2.0)
    tmp += T(2*math::pi);
  T l = tmp.lower();
  T u = tmp.upper();

  using std::sin;
  typename Traits::rounding rnd;

  // separate into monotone subintervals
  if(l <= math::pi/2.0) {
    if(u <= math::pi/2.0) {
      rnd.downward();
      T sl = sin(l);
      rnd.upward();
      return I(sl, sin(u), true);
    } else if(u < 3.0*math::pi/2.0) {
      rnd.downward();
      if(math::pi-u <= l) 
	return I(sin(u), 1, true);
      else
	return I(sin(l), 1, true);
    } else {
      return I(-1, 1, true);
    }
  } else if(l <= 3.0*math::pi/2.0) {
    if(u <= 3*math::pi/2) {
      rnd.downward();
      T su = sin(u);
      rnd.upward();
      return I(su, sin(l), true);
    } else if(u < 5.0*math::pi/2.0) {
      rnd.upward();
      if(3.0*math::pi-u < l) 
	return I(-1, sin(u), true);
      else
	return I(-1, sin(l), true);
    } else {
      return I(-1, 1, true);
    }
  } else {
    // should never happen
    assert(0);
    return 0;
  }
}

template<class T, class Traits>
inline interval<T, Traits> cos(const interval<T, Traits>& x)
{
  return sin(x+T(math::pi/2));
}

template<class T, class Traits>
interval<T, Traits> tan(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  if(width(x) >= math::pi) {
     // we are covering a full period
    typedef typename Traits::checking checking;
    checking::trigonometric_inf();
    return I::entire();
  }

  // get us within [-pi/2, 3/2*pi]
  I tmp = fmod(x, T(math::pi));
  while (tmp.lower() <= -math::pi/2)
    tmp += math::pi;
  if (in(math::pi/2, tmp) || in(3*math::pi/2, tmp)) {
    typedef typename Traits::checking checking;
    checking::trigonometric_inf();
    return I::entire();
  }

  using std::tan;
  typename Traits::rounding rnd;
  rnd.downward();
  T tl = tan(tmp.lower());
  rnd.upward();
  return I(tl, tan(tmp.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> asin(const interval<T, Traits>& x)
{
  if (x.lower() < T(-1) || x.upper() > T(1)) {
    typedef typename Traits::checking checking;
    checking::trigonometric_nan();
  }
  using std::asin;
  typename Traits::rounding rnd;
  rnd.downward();
  T asl = x.lower() <= T(-1) ? -math::pi/2 : asin(x.lower());
  rnd.upward();
  T asu = x.upper() >= T(1) ? math::pi/2 : asin(x.upper());
  return interval<T, Traits>(asl, asu, true);
}

template<class T, class Traits>
interval<T, Traits> acos(const interval<T, Traits>& x)
{
  if (x.lower() < T(-1) || x.upper() > T(1)) {
    typedef typename Traits::checking checking;
    checking::trigonometric_nan();
  }
  using std::acos;
  typename Traits::rounding rnd;
  rnd.downward();
  T acu = x.upper() >= T(1) ? T(0) : acos(x.upper());
  rnd.upward();
  T acl = x.lower() <= T(-1) ? math::pi : acos(x.lower());
  return interval<T, Traits>(acu, acl, true);
}

template<class T, class Traits>
interval<T, Traits> atan(const interval<T, Traits>& x)
{
  using std::atan;
  typename Traits::rounding rnd;
  rnd.downward();
  T atl = atan(x.lower());
  rnd.upward();
  return interval<T, Traits>(atl, atan(x.upper()), true); 
}

template<class T, class Traits>
interval<T, Traits> atan2(const interval<T, Traits>& y,
			  const interval<T, Traits>& x)
{
  interval<T, Traits> tmp = atan(y/x);
  typename Traits::rounding rnd;
  rnd.downward();
  T atl = tmp.lower();
  if(detail::sign(y.lower()) && detail::sign(x.lower()))
    atl -= math::pi;
  rnd.upward();
  T atu = tmp.upper();
  if(!detail::sign(y.upper()) && detail::sign(x.lower()))
    atu += math::pi;
  return interval<T, Traits>(atl, atu, true);
}


// hyperbolic functions

template<class T, class Traits>
interval<T, Traits> sinh(const interval<T, Traits>& x)
{
  using std::sinh;
  typename Traits::rounding rnd;
  rnd.downward();
  T shl = sinh(x.lower());
  rnd.upward();
  return interval<T, Traits>(shl, sinh(x.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> cosh(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  using std::cosh;
  typename Traits::rounding rnd;
  if(detail::sign(x.upper())) {
    rnd.downward();
    T l = cosh(x.upper());
    rnd.upward();
    return I(l, cosh(x.lower()), true);
  } else if(!detail::sign(x.lower())) {
    rnd.downward();
    T l = cosh(x.lower());
    rnd.upward();
    return I(l, cosh(x.upper()), true);
  } else {
    rnd.upward();
    if(-x.lower() > x.upper())
      return I(0, cosh(x.lower()), true);
    else
      return I(0, cosh(x.upper()), true);
  }
}

template<class T, class Traits>
interval<T, Traits> tanh(const interval<T, Traits>& x)
{
  using std::tanh;
  typename Traits::rounding rnd;
  rnd.downward();
  T tl = tanh(x.lower());
  rnd.upward();
  return interval<T, Traits>(tl, tanh(x.upper()), true);
}

#ifdef BOOST_HAVE_INV_HYPERBOLIC
template<class T, class Traits>
interval<T, Traits> asinh(const interval<T, Traits>& x)
{
  using std::asinh;
  typename Traits::rounding rnd;
  rnd.downward();
  T asl = asinh(x.lower());
  rnd.upward();
  return interval<T, Traits>(asl, asinh(x.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> acosh(const interval<T, Traits>& x)
{
  using std::acosh;
  typename Traits::rounding rnd;
  rnd.downward();
  T acl = x.lower() <= T(1) ? 0 :  acosh(x.lower());
  rnd.upward();
  return interval<T, Traits>(acl, acosh(x.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> atanh(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  if(x.upper() < T(-1) || x.lower() > T(1))
    return I::empty();
  using std::atanh;
  typename Traits::rounding rnd;
  typedef typename Traits::checking checking;
  T acl;
  if (x.lower() <= T(-1)) {
    if (x.lower() < T(-1)) checking::trigonometric_nan();
    checking_trigonometric_inf();
    acl = -std::numeric_limits<T>::infinity();
  } else {
    rnd.downward();
    acl = atanh(x.lower());
  }
  T acu;
  if (x.upper() >= T(1)) {
    if (x.upper() > T(1)) checking::trigonometric_nan();
    checking_trigonometric_inf();
    acu = std::numeric_limits<T>::infinity();
  } else {
    rnd.upward();
    acu = atanh(x.upper());
  }
  return I(acl, acu, true);
}
#endif // BOOST_HAVE_INV_HYPERBOLIC

} // namespace boost

#endif // BOOST_INTERVAL_TRANSC_HPP
