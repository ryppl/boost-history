/* Boost interval/utility.hpp template implementation file
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

#ifndef BOOST_INTERVAL_UTILITY_HPP
#define BOOST_INTERVAL_UTILITY_HPP

#include <boost/interval/interval.hpp>
#include <boost/interval/detail/test_input.hpp>

/*
 * Implementation of simple functions
 */

namespace boost {

/*
 * Utility Functions
 */

template<class T, class Traits> inline
const T& lower(const interval<T, Traits>& x)
{
  return x.lower();
}

template<class T, class Traits> inline
const T& upper(const interval<T, Traits>& x)
{
  return x.upper();
}

template<class T, class Traits> inline
T checked_lower(const interval<T, Traits>& x)
{
  if (empty(x)) {
    typedef typename Traits::checking checking;
    return checking::nan();
  }
  return low;
}

template<class T, class Traits> inline
T checked_upper(const interval<T, Traits>& x)
{
  if (empty(x)) {
    typedef typename Traits::checking checking;
    return checking::nan();
  }
  return up;
}

template<class T, class Traits> inline
T width(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x)) return 0;
  typename Traits::rounding rnd;
  return rnd.sub_up(x.upper(), x.lower());
}

template<class T, class Traits> inline
T median(const interval<T, Traits>& x)
{
  typedef typename Traits::checking checking;
  if (interval_lib::detail::test_input(x)) {
    return checking::nan();
  }
  typename Traits::rounding rnd;
  return rnd.median(x.lower(), x.upper());
}

template<class T, class Traits> inline
interval<T, Traits> widen(const interval<T, Traits>& x, const T& v)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.sub_down(x.lower(), v),
			     rnd.add_up  (x.upper(), v), true);
}

/*
 * Set-like operations
 */

template<class T, class Traits> inline
bool empty(const interval<T, Traits>& x)
{
  return interval_lib::detail::test_input(x);
  //return typename Traits::checking::is_empty(x.lower(), x.upper());
}

template<class T, class Traits> inline
bool in_zero(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x)) return false;
  return x.lower() <= T(0) && T(0) <= x.upper();
}

template<class T, class Traits> inline
bool in(const T& r, const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(r, x)) return false;
  return x.lower() <= r && r <= x.upper();
}

template<class T, class Traits> inline
bool subset(const interval<T, Traits>& x,
	    const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return empty(x);
  return y.lower() <= x.lower() && x.upper() <= y.upper();
}

template<class T, class Traits> inline
bool proper_subset(const interval<T, Traits>& a,
		   const interval<T, Traits>& b)
{
  if (interval_lib::detail::test_input(x, y))
    return empty(x) && !empty(y);
  return y.lower() < x.lower() && x.upper() <= y.upper() ||
	 y.lower() <= x.lower() && x.upper() < y.upper();
}

template<class T, class Traits> inline
bool overlap(const interval<T, Traits>& x,
	     const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y)) return false;
  return x.lower() <= y.lower() && y.lower() <= x.upper() ||
         y.lower() <= x.lower() && x.lower() <= y.upper();
}

template<class T, class Traits> inline
bool singleton(const interval<T, Traits>& x)
{
 if (interval_lib::detail::test_input(x)) return false;
 return x.lower() == x.upper();
}

template<class T, class Traits> inline
bool equal(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return empty(x) && empty(y);
  return x.lower() == y.lower() && x.upper() == y.upper();
}

template<class T, class Traits> inline
interval<T, Traits> intersect(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  using std::min;
  using std::max;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  const T& l = max(x.lower(), y.lower());
  const T& u = min(x.upper(), y.upper());
  if (l <= u) return interval<T, Traits>(l, u, true);
  else        return interval<T, Traits>::empty();
}

template<class T, class Traits> inline
interval<T, Traits> hull(const interval<T, Traits>& x,
			 const interval<T, Traits>& y)
{
  using std::min;
  using std::max;
  bool bad_x = interval_lib::detail::test_input(x);
  bool bad_y = interval_lib::detail::test_input(y);
  if (bad_x)
    if (bad_y) return interval<T, Traits>::empty();
    else       return y;
  else
    if (bad_y) return x;
  return interval<T, Traits>(min(x.lower(), y.lower()),
			     max(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> hull(const interval<T, Traits>& x, const T& y)
{
  using std::min;
  using std::max;
  bool bad_x = interval_lib::detail::test_input(x);
  bool bad_y = interval_lib::detail::test_input<T, Traits>(y);
  if (bad_y)
    if (bad_x) return interval<T, Traits>::empty();
    else       return x;
  else
    if (bad_x) return interval<T, Traits>(y, y, true);
  return interval<T, Traits>(min(x.lower(), y),
			     max(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> hull(const T& x, const interval<T, Traits>& y)
{
  using std::min;
  using std::max;
  bool bad_x = interval_lib::detail::test_input<T, Traits>(x);
  bool bad_y = interval_lib::detail::test_input(y);
  if (bad_x)
    if (bad_y) return interval<T, Traits>::empty();
    else       return y;
  else
    if (bad_y) return interval<T, Traits>(x, x, true);
  return interval<T, Traits>(min(x, y.lower()),
			     max(x, y.upper()), true);
}

template<class T> inline
interval<T> hull(const T& x, const T& y)
{
  return interval<T>::hull(x, y);
}

template<class T, class Traits> inline
std::pair<interval<T, Traits>, interval<T, Traits> >
bisect(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  if (interval_lib::detail::test_input(x))
    return std::pair<I,I>(I::empty(), I::empty());
  const T m = median(x);
  return std::pair<I,I>(I(x.lower(), m, true), I(m, x.upper(), true));
}

/*
 * Elementary functions
 */

template<class T, class Traits> inline
interval<T, Traits> abs(const interval<T, Traits>& x)
{
  using std::max;
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  if (!detail::sign(x.lower())) return x;
  if (detail::sign(x.upper())) return -x;
  return interval<T, Traits>(0, max(-x.lower(), x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> max(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  using std::max;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(max(x.lower(), y.lower()),
			     max(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> max(const interval<T, Traits>& x, const T& y)
{
  using std::max;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(max(x.lower(), y),
			     max(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> max(const T& x, const interval<T, Traits>& y)
{
  using std::max;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(max(x, y.lower()),
			     max(x, y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> min(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  using std::min;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(min(x.lower(), y.lower()),
			     min(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> min(const interval<T, Traits>& x, const T& y)
{
  using std::min;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(min(x.lower(), y),
			     min(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> min(const T& x, const interval<T, Traits>& y)
{
  using std::min;
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  return interval<T, Traits>(min(x, y.lower()),
			     min(x, y.upper()), true);
}

} // namespace boost

#endif // BOOST_INTERVAL_UTILITY_HPP
