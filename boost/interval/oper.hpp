/* boost interval/oper.hpp template implementation file
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

#ifndef BOOST_INTERVAL_OPER_HPP
#define BOOST_INTERVAL_OPER_HPP


namespace boost {

/*
 * Basic arithmetic operators
 */

template<class T, class Traits> inline
const interval<T, Traits>& operator+(const interval<T, Traits>& x)
{
  return x;
}

template<class T, class Traits> inline
interval<T, Traits> operator-(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(-x.upper(), -x.lower(), true);
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator+=(const interval<T, Traits>& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Traits::rounding rnd;
    set(rnd.add_down(low, r.low), rnd.add_up(up, r.up));
  }
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator+=(const T& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Traits::rounding rnd;
    set(rnd.add_down(low, r), rnd.add_up(up, r));
  }
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator-=(const interval<T, Traits>& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Traits::rounding rnd;
    set(rnd.sub_down(low, r.up), rnd.sub_up(up, r.low));
  }
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator-=(const T& r)
{
  if (interval_lib::detail::test_input(*this, r))
    set_empty();
  else {
    typename Traits::rounding rnd;
    set(rnd.sub_down(low, r), rnd.sub_up(up, r));
  }
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator*=(const interval<T, Traits>& r)
{
  return *this = *this * r;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator*=(const T& r)
{
  return *this = r * *this;
}

template<class T, class Traits> inline
interval<T, Traits> multiplicative_inverse(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  if (in_zero(x)) {
    typedef typename Traits::checking checking;
    checking::divide_by_zero(x.lower(), x.upper());
    return interval<T, Traits>::whole();
  }
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.div_down(1, x.upper()),
			     rnd.div_up  (1, x.lower()), true);
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator/=(const interval<T, Traits>& r)
{
  return *this = *this / r;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator/=(const T& r)
{
  return *this = *this / r;
}

template<class T, class Traits> inline
interval<T, Traits> operator+(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.add_down(x.lower(), y.lower()),
			    rnd.add_up  (x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator+(const T& x, const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.add_down(x, y.lower()),
			    rnd.add_up  (x, y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator+(const interval<T, Traits>& x, const T& y)
{ return y + x; }

template<class T, class Traits> inline
interval<T, Traits> operator-(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.sub_down(x.lower(), y.upper()),
			    rnd.sub_up  (x.upper(), y.lower()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator-(const T& x, const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.sub_down(x, y.upper()),
			    rnd.sub_up  (x, y.lower()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator-(const interval<T, Traits>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.sub_down(x.lower(), y),
			    rnd.sub_up  (x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator*(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  using std::min;
  using std::max;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef interval<T, Traits> I;
  const T& xl = x.lower();
  const T& xu = x.upper();
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::sign(xu))
    if (detail::sign(yu))
      return I(rnd.mul_down(xu, yu), rnd.mul_up(xl, yl), true);
    else if (detail::sign(yl))
      return I(rnd.mul_down(xl, yu), rnd.mul_up(xl, yl), true);
    else
      return I(rnd.mul_down(xl, yu), rnd.mul_up(xu, yl), true);
  else if (detail::sign(xl))
    if (detail::sign(yu))
      return I(rnd.mul_down(xu, yl), rnd.mul_up(xl, yl), true);
    else if (detail::sign(yl))
      return I(min(rnd.mul_down(xl, yu), rnd.mul_down(xu, yl)),
	       max(rnd.mul_up  (xl, yl), rnd.mul_up  (xu, yu)), true);
    else
      return I(rnd.mul_down(xl, yu), rnd.mul_up(xu, yu), true);
  else
    if (detail::sign(yu))
      return I(rnd.mul_down(xu, yl), rnd.mul_up(xl, yu), true);
    else if (detail::sign(yl))
      return I(rnd.mul_down(xu, yl), rnd.mul_up(xu, yu), true);
    else
      return I(rnd.mul_down(xl, yl), rnd.mul_up(xu, yu), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator*(const T& x, const interval<T, Traits>& y)
{ 
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::sign(x))
    return interval<T, Traits>(rnd.mul_down(x, yu), rnd.mul_up(x, yl), true);
  else
    return interval<T, Traits>(rnd.mul_down(x, yl), rnd.mul_up(x, yu), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator*(const interval<T, Traits>& x, const T& y)
{ return y * x; }

template<class T, class Traits> inline
interval<T, Traits> operator/(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  if (in_zero(y)) {
    if (singleton(y)) return interval<T, Traits>::empty();
    else              return interval<T, Traits>::whole();
  }
  typename Traits::rounding rnd;
  typedef interval<T, Traits> I;
  const T& xl = x.lower();
  const T& xu = x.upper();
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::sign(xu))
    if (detail::sign(yu))
      return I(rnd.div_down(xu, yl), rnd.div_up(xl, yu), true);
    else
      return I(rnd.div_down(xl, yl), rnd.div_up(xu, yu), true);
  else if (detail::sign(xl))
    if (detail::sign(yu))
      return I(rnd.div_down(xu, yu), rnd.div_up(xl, yu), true);
    else
      return I(rnd.div_down(xl, yl), rnd.div_up(xu, yl), true);
  else
    if (detail::sign(yu))
      return I(rnd.div_down(xu, yu), rnd.div_up(xl, yl), true);
    else
      return I(rnd.div_down(xl, yu), rnd.div_up(xu, yl), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator/(const T& x, const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  if (in_zero(y)) {
    if (singleton(y)) return interval<T, Traits>::empty();
    else              return interval<T, Traits>::whole();
  }
  typename Traits::rounding rnd;
  const T& yl = y.lower();
  const T& yu = y.upper();
  if (detail::sign(x))
    return interval<T, Traits>(rnd.div_down(x, yl), rnd.div_up(x, yu), true);
  else
    return interval<T, Traits>(rnd.div_down(x, yu), rnd.div_up(x, yl), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator/(const interval<T, Traits>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y) || y == T(0))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  const T& xl = x.lower();
  const T& xu = x.upper();
  if (detail::sign(y))
    return interval<T, Traits>(rnd.div_down(xu, y), rnd.div_up(xl, y), true);
  else
    return interval<T, Traits>(rnd.div_down(xl, y), rnd.div_up(xu, y), true);
}

template<class T, class Traits> inline
interval<T, Traits> sqrt(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x) || detail::sign(x.upper()))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  T l = (x.lower() <= T(0)) ? 0 : rnd.sqrt_down(x.lower());
  return interval<T, Traits>(l, rnd.sqrt_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> square(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  if (detail::sign(x.upper())) {
    return interval<T, Traits>(rnd.mul_down(x.upper(), x.upper()),
			       rnd.mul_up(x.lower(), x.lower()), true);
  } else if (!detail::sign(x.lower())) {
    return interval<T, Traits>(rnd.mul_down(x.lower(), x.lower()),
			       rnd.mul_up(x.upper(), x.upper()), true);
  } else {
    return interval<T, Traits>(0, (-x.lower() > x.upper() ? 
				   rnd.mul_up(x.lower(), x.lower()) :
				   rnd.mul_up(x.upper(), x.upper())), true);
  }
}

} // namespace boost

#endif // BOOST_INTERVAL_OPER_HPP
