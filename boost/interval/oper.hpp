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
  bool neg_x = detail::sign(x.upper());
  bool neg_y = detail::sign(y.upper());
  bool pos_x = !detail::sign(x.lower());
  bool pos_y = !detail::sign(y.lower());
  bool zro_x = !neg_x && !pos_x;
  bool zro_y = !neg_y && !pos_y;
  if (zro_x && zro_y)
    return interval<T, Traits>(min(rnd.mul_down(x.lower(), y.upper()),
				   rnd.mul_down(x.upper(), y.lower())),
			       max(rnd.mul_up  (x.lower(), y.lower()),
				   rnd.mul_up  (x.upper(), y.upper())), true);
  else {
    const T& xl = (neg_y || zro_y &&  pos_x) ? x.upper() : x.lower();
    const T& xu = (neg_y || zro_y && !pos_x) ? x.lower() : x.upper();
    const T& yl = (neg_x || zro_x &&  pos_y) ? y.upper() : y.lower();
    const T& yu = (neg_x || zro_x && !pos_y) ? y.lower() : y.upper();
    return interval<T, Traits>(rnd.mul_down(xl, yl), rnd.mul_up(xu, yu), true);
  }
}

template<class T, class Traits> inline
interval<T, Traits> operator*(const T& x, const interval<T, Traits>& y)
{ 
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  bool neg_x = detail::sign(x);
  const T& yl = neg_x ? y.upper() : y.lower();
  const T& yu = neg_x ? y.lower() : y.upper();
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
  bool neg_x = detail::sign(x.upper());
  bool neg_y = detail::sign(y.upper());
  bool pos_x = !detail::sign(x.lower());
  bool zro_x = !pos_x && !neg_x;
  const T& xl = neg_y ? x.upper() : x.lower();
  const T& xu = neg_y ? x.lower() : x.upper();
  const T& yl = (pos_x || zro_x &&  neg_y) ? y.upper() : y.lower();
  const T& yu = (pos_x || zro_x && !neg_y) ? y.lower() : y.upper();
  return interval<T, Traits>(rnd.div_down(xl, yl), rnd.div_up(xu, yu), true);
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
  bool neg_x = detail::sign(x);
  const T& yl = !neg_x ? y.upper() : y.lower();
  const T& yu = !neg_x ? y.lower() : y.upper();
  return interval<T, Traits>(rnd.div_down(x, yl), rnd.div_up(x, yu), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator/(const interval<T, Traits>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y) || y == T(0))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  bool neg_y = detail::sign(y);
  const T& xl = neg_y ? x.upper() : x.lower();
  const T& xu = neg_y ? x.lower() : x.upper();
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
