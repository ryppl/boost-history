/* Boost interval/arith.hpp template implementation file
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

#ifndef BOOST_INTERVAL_ARITH_HPP
#define BOOST_INTERVAL_ARITH_HPP

#include <boost/interval/detail/interval_prototype.hpp>
#include <boost/interval/detail/bugs.hpp>
#include <boost/interval/detail/test_input.hpp>
#include <algorithm>

namespace boost {

/*
 * Basic arithmetic operators
 */

template<class T, class Policies> inline
const interval<T, Policies>& operator+(const interval<T, Policies>& x)
{
  return x;
}

template<class T, class Policies> inline
interval<T, Policies> operator-(const interval<T, Policies>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Policies>::empty();
  return interval<T, Policies>(-x.upper(), -x.lower(), true);
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator+=(const interval<T, Policies>& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Policies::rounding rnd;
    set(rnd.add_down(low, r.low), rnd.add_up(up, r.up));
  }
  return *this;
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator+=(const T& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Policies::rounding rnd;
    set(rnd.add_down(low, r), rnd.add_up(up, r));
  }
  return *this;
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator-=(const interval<T, Policies>& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Policies::rounding rnd;
    set(rnd.sub_down(low, r.up), rnd.sub_up(up, r.low));
  }
  return *this;
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator-=(const T& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Policies::rounding rnd;
    set(rnd.sub_down(low, r), rnd.sub_up(up, r));
  }
  return *this;
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator*=(const interval<T, Policies>& r)
{
  return *this = *this * r;
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator*=(const T& r)
{
  return *this = r * *this;
}

template<class T, class Policies> inline
interval<T, Policies> multiplicative_inverse(const interval<T, Policies>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Policies>::empty();
  if (in_zero(x)) {
    typedef typename Policies::checking checking;
    checking::divide_by_zero(x.lower(), x.upper());
    return interval<T, Policies>::whole();
  }
  typename Policies::rounding rnd;
  return interval<T, Policies>(rnd.div_down(1, x.upper()),
			     rnd.div_up  (1, x.lower()), true);
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator/=(const interval<T, Policies>& r)
{
  return *this = *this / r;
}

template<class T, class Policies> inline
interval<T, Policies>& interval<T, Policies>::operator/=(const T& r)
{
  return *this = *this / r;
}

template<class T, class Policies> inline
interval<T, Policies> operator+(const interval<T, Policies>& x,
				const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  return interval<T,Policies>(rnd.add_down(x.lower(), y.lower()),
			    rnd.add_up  (x.upper(), y.upper()), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator+(const T& x, const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  return interval<T,Policies>(rnd.add_down(x, y.lower()),
			    rnd.add_up  (x, y.upper()), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator+(const interval<T, Policies>& x, const T& y)
{ return y + x; }

template<class T, class Policies> inline
interval<T, Policies> operator-(const interval<T, Policies>& x,
				const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  return interval<T,Policies>(rnd.sub_down(x.lower(), y.upper()),
			    rnd.sub_up  (x.upper(), y.lower()), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator-(const T& x, const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  return interval<T,Policies>(rnd.sub_down(x, y.upper()),
			    rnd.sub_up  (x, y.lower()), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator-(const interval<T, Policies>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  return interval<T,Policies>(rnd.sub_down(x.lower(), y),
			    rnd.sub_up  (x.upper(), y), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator*(const interval<T, Policies>& x,
				const interval<T, Policies>& y)
{
  // using std::min;
  // using std::max;
  BOOST_INTERVAL_using_max(min);
  BOOST_INTERVAL_using_max(max);
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  typedef interval<T, Policies> I;
  const T& xl = x.lower();
  const T& xu = x.upper();
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::is_neg(xu))
    if (detail::is_neg(yu))
      return I(rnd.mul_down(xu, yu), rnd.mul_up(xl, yl), true);
    else if (detail::is_neg(yl))
      return I(rnd.mul_down(xl, yu), rnd.mul_up(xl, yl), true);
    else
      return I(rnd.mul_down(xl, yu), rnd.mul_up(xu, yl), true);
  else if (detail::is_neg(xl))
    if (detail::is_neg(yu))
      return I(rnd.mul_down(xu, yl), rnd.mul_up(xl, yl), true);
    else if (detail::is_neg(yl))
      return I(min(rnd.mul_down(xl, yu), rnd.mul_down(xu, yl)),
	       max(rnd.mul_up  (xl, yl), rnd.mul_up  (xu, yu)), true);
    else
      return I(rnd.mul_down(xl, yu), rnd.mul_up(xu, yu), true);
  else
    if (detail::is_neg(yu))
      return I(rnd.mul_down(xu, yl), rnd.mul_up(xl, yu), true);
    else if (detail::is_neg(yl))
      return I(rnd.mul_down(xu, yl), rnd.mul_up(xu, yu), true);
    else
      return I(rnd.mul_down(xl, yl), rnd.mul_up(xu, yu), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator*(const T& x, const interval<T, Policies>& y)
{ 
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::is_neg(x))
    return interval<T, Policies>(rnd.mul_down(x, yu), rnd.mul_up(x, yl), true);
  else
    return interval<T, Policies>(rnd.mul_down(x, yl), rnd.mul_up(x, yu), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator*(const interval<T, Policies>& x, const T& y)
{ return y * x; }

template<class T, class Policies> inline
interval<T, Policies> operator/(const interval<T, Policies>& x,
				const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  if (in_zero(y)) {
    if (singleton(y)) return interval<T, Policies>::empty();
    else              return interval<T, Policies>::whole();
  }
  typename Policies::rounding rnd;
  typedef interval<T, Policies> I;
  const T& xl = x.lower();
  const T& xu = x.upper();
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::is_neg(xu))
    if (detail::is_neg(yu))
      return I(rnd.div_down(xu, yl), rnd.div_up(xl, yu), true);
    else
      return I(rnd.div_down(xl, yl), rnd.div_up(xu, yu), true);
  else if (detail::is_neg(xl))
    if (detail::is_neg(yu))
      return I(rnd.div_down(xu, yu), rnd.div_up(xl, yu), true);
    else
      return I(rnd.div_down(xl, yl), rnd.div_up(xu, yl), true);
  else
    if (detail::is_neg(yu))
      return I(rnd.div_down(xu, yu), rnd.div_up(xl, yl), true);
    else
      return I(rnd.div_down(xl, yu), rnd.div_up(xu, yl), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator/(const T& x, const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  if (in_zero(y)) {
    if (singleton(y)) return interval<T, Policies>::empty();
    else              return interval<T, Policies>::whole();
  }
  typename Policies::rounding rnd;
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::is_neg(x))
    return interval<T, Policies>(rnd.div_down(x, yl), rnd.div_up(x, yu), true);
  else
    return interval<T, Policies>(rnd.div_down(x, yu), rnd.div_up(x, yl), true);
}

template<class T, class Policies> inline
interval<T, Policies> operator/(const interval<T, Policies>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y) || y == T(0))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  const T& xl = x.lower();
  const T& xu = x.upper();
  if (detail::is_neg(y))
    return interval<T, Policies>(rnd.div_down(xu, y), rnd.div_up(xl, y), true);
  else
    return interval<T, Policies>(rnd.div_down(xl, y), rnd.div_up(xu, y), true);
}

} // namespace boost

#endif // BOOST_INTERVAL_ARITH_HPP
