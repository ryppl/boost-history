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
interval<T, Traits> operator+(const interval<T, Traits>& x)
{
  return x;
}

template<class T, class Traits> inline
interval<T, Traits> operator-(const interval<T, Traits>& x)
{
  return interval<T, Traits>(-x.upper(), -x.lower(), true);
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator+=(const interval<T, Traits>& r)
{
  typename Traits::rounding rnd;
  set(rnd.add_down(low, r.low), rnd.add_up(up, r.up));
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator+=(const T& r)
{
  typename Traits::rounding rnd;
  set(rnd.add_down(low, r), rnd.add_up(up, r));
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator-=(const interval<T, Traits>& r)
{
  typename Traits::rounding rnd;
  set(rnd.sub_down(low, r.up), rnd.sub_up(up, r.low));
  return *this;
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator-=(const T& r)
{
  typename Traits::rounding rnd;
  set(rnd.sub_down(low, r), rnd.sub_up(up, r));
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
  return *this = *this * r;
}

template<class T, class Traits> inline
interval<T, Traits> multiplicative_inverse(const interval<T, Traits>& x)
{
  if (in_zero(x)) {
    typedef typename Traits::checking checking;
    checking::divide_by_zero(x.lower(), x.upper());
    return interval<T, Traits>::entire();
  }
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.div_down(T(1), x.upper()),
			     rnd.div_up(T(1), x.lower()), true);
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

#ifndef BOOST_INTERVAL_USE_FRIEND_OPERATORS

template<class T, class Traits> inline
interval<T, Traits> operator+(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.add_down(x.lower(), y.lower()),
			    rnd.add_up(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator+(const T& x, const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.add_down(x, y.lower()),
			    rnd.add_up(x, y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator+(const interval<T, Traits>& x, const T& y)
{ return y + x; }

template<class T, class Traits> inline
interval<T, Traits> operator-(const interval<T, Traits>& x,
				     const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.sub_down(x.lower(), y.upper()),
			    rnd.sub_up(x.upper(), y.lower()), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator-(const interval<T, Traits>& x, const T& y)
{
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.sub_down(x.lower(), y),
			    rnd.sub_up(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> operator-(const T& x, const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  return interval<T,Traits>(rnd.sub_down(x, y.upper()),
			    rnd.sub_up(x, y.lower()), true);
}

#define _m_ibo_(A, B, C, D) \
  return interval<T, Traits>(rnd.mul_down(A, B), rnd.mul_up(C, D), true);

template<class T, class Traits> inline
interval<T, Traits> operator*(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  if (detail::sign(x.upper())) {
    if (detail::sign(y.upper())) {
      _m_ibo_(x.upper(), y.upper(), x.lower(), y.lower());
    } else if(!detail::sign(y.lower())) {
      _m_ibo_(x.lower(), y.upper(), x.upper(), y.lower());
    } else {
      _m_ibo_(x.lower(), y.upper(), x.lower(), y.lower());
    }
  } else if(!detail::sign(x.lower())) {
    if (!detail::sign(y.lower())) {
      _m_ibo_(x.lower(), y.lower(), x.upper(), y.upper());
    } else if (detail::sign(y.upper())) {
      _m_ibo_(x.upper(), y.lower(), x.lower(), y.upper());
    } else  {
      _m_ibo_(x.upper(), y.lower(), x.upper(), y.upper());
    }
  } else {
    if(!detail::sign(y.lower())) {
      _m_ibo_(x.lower(), y.upper(), x.upper(), y.upper());
    } else if(detail::sign(y.upper())) {
      _m_ibo_(x.upper(), y.lower(), x.lower(), y.lower());
    } else {
      return interval<T,Traits>(std::min(rnd.mul_down(x.lower(), y.upper()),
					 rnd.mul_down(x.upper(), y.lower())),
				std::max(rnd.mul_up(x.lower(), y.lower()),
					 rnd.mul_up(x.upper(), y.upper())),
				true);
    }
  }
}

template<class T, class Traits> inline
interval<T, Traits> operator*(const T& x, const interval<T, Traits>& y)
{ 
  typename Traits::rounding rnd;
  if (!detail::sign(x)) {
    _m_ibo_(x, y.lower(), x, y.upper());
  } else {
    _m_ibo_(x, y.upper(), x, y.lower());
  }
}

#undef _m_ibo_

template<class T, class Traits> inline
interval<T, Traits> operator*(const interval<T, Traits>& x, const T& y)
{ return y * x; }

#define _m_ibo_(A, B, C, D) \
  return interval<T, Traits>(rnd.div_down(A, B), rnd.div_up(C, D), true);

template<class T, class Traits> inline
interval<T, Traits> operator/(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  if (in_zero(y)) {
    typedef typename Traits::checking checking;
    checking::divide_by_zero(y.lower(), y.upper());
    return interval<T, Traits>::entire();
  }

  typename Traits::rounding rnd;
  if (detail::sign(x.upper())) {
    if (detail::sign(y.upper())) {
      _m_ibo_(x.upper(), y.lower(), x.lower(), y.upper());
    } else {
      _m_ibo_(x.lower(), y.lower(), x.upper(), y.upper());
    }
  } else if (!detail::sign(x.lower())) {
    if (!detail::sign(y.lower())) {
      _m_ibo_(x.lower(), y.upper(), x.upper(), y.lower());
    } else {
      _m_ibo_(x.upper(), y.upper(), x.lower(), y.lower());
    }
  } else {
    if (!detail::sign(y.lower())) {
      _m_ibo_(x.lower(), y.lower(), x.upper(), y.lower());
    } else {
      _m_ibo_(x.upper(), y.upper(), x.lower(), y.upper());
    }
  }
}

template<class T, class Traits> inline
interval<T, Traits> operator/(const interval<T, Traits>& x, const T& y)
{
  if (y == 0) {
    typedef typename Traits::checking checking;
    checking::divide_by_zero(y, y);
    return interval<T, Traits>::empty();
  }
  typename Traits::rounding rnd;
  if (!detail::sign(y)) {
    _m_ibo_(x.lower(), y, x.upper(), y);
  } else {
    _m_ibo_(x.upper(), y, x.lower(), y);
  }
}

template<class T, class Traits> inline
interval<T, Traits> operator/(const T& x, const interval<T, Traits>& y)
{
  typename Traits::rounding rnd;
  if (in_zero(y)) {
    typedef typename Traits::checking checking;
    checking::divide_by_zero(y.lower(), y.upper());
    return interval<T, Traits>::entire();
  }
  if (detail::sign(x)) {
    _m_ibo_(x, y.lower(), x, y.upper());
  } else {
    _m_ibo_(x, y.upper(), x, y.lower());
  }
}

#undef _m_ibo_

template<class T, class Traits> inline
interval<T, Traits> sqrt(const interval<T, Traits>& x)
{
  if (detail::sign(x.upper())) {
    typedef typename Traits::checking checking;
    checking::sqrt_nan();
    return interval<T, Traits>::empty();
  }

  typename Traits::rounding rnd;
  T l = (x.lower() <= T(0)) ? 0 : rnd.sqrt_down(x.lower());
  return interval<T, Traits>(l, rnd.sqrt_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> square(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  typename Traits::rounding rnd;
  if (detail::sign(x.lower()) && !detail::sign(x.upper())) {
    return I(T(0), (-x.lower() > x.upper() ? 
		    rnd.mul_up(x.lower(), x.lower()) :
		    rnd.mul_up(x.upper(), x.upper())), true);
  } else if (detail::sign(x.upper())) {
    return I(rnd.mul_down(x.upper(), x.upper()),
	     rnd.mul_up(x.lower(), x.lower()), true);
  } else {
    return I(rnd.mul_down(x.lower(), x.lower()),
	     rnd.mul_up(x.upper(), x.upper()), true);
  }
}

#endif // BOOST_INTERVAL_USE_FRIEND_OPERATORS

} // namespace boost

#endif // BOOST_INTERVAL_OPER_HPP
