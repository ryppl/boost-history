#ifndef BOOST_INTERVAL_UTILITY_HPP
#define BOOST_INTERVAL_UTILITY_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#include <algorithm>

/*
 * Implementation of simple functions
 */

namespace boost {

template<class T, class Traits> inline
void interval<T, Traits>::set_empty()
{
  typedef typename Traits::checking checking;
  checking::created_empty();
  low = checking::empty_lower();
  up  = checking::empty_upper();
}

template<class T, class Traits> inline
void interval<T, Traits>::set_whole()
{
  T inf = base_limits::infinity();
  low = -inf;
  up  =  inf;
}

template<class T, class Traits> inline
interval<T, Traits>::interval(const T& l, const T& u): low(l), up(u)
{
  if (low > up) {
    typedef typename Traits::checking checking;
    checking::inverted_bound(l, u);
    set_empty();
  }
}

template<class T, class Traits> inline
interval<T, Traits>& interval<T, Traits>::operator=(const T& x)
{
  low = up = x;
  return *this;
}

template<class T, class Traits> inline
void interval<T, Traits>::assign(const T& l, const T& u)
{
  if (l > u) {
    typedef typename Traits::checking checking;
    checking::inverted_bound(l, u);
    set_empty();
  } else set(l, u);
}

template<class T, class Traits> inline
void interval<T, Traits>::set(const T& l, const T& u)
{
  low = l;
  up = u;
}

template<class T, class Traits> inline
interval<T, Traits> interval<T, Traits>::hull(const T& x, const T& y)
{
  if (x < y)
    return interval<T, Traits>(x, y, true);
  else
    return interval<T, Traits>(y, x, true);
}

template<class T, class Traits> inline
interval<T, Traits> interval<T, Traits>::empty()
{
  typedef typename Traits::checking checking;
  checking::created_empty();
  return interval<T, Traits>(checking::empty_lower(),
			     checking::empty_upper(), true);
}

template<class T, class Traits> inline
interval<T, Traits> interval<T, Traits>::whole()
{
  T inf = base_limits::infinity();
  return interval<T, Traits>(-inf, inf, true);
}


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
T width(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return rnd.sub_up(x.upper(), x.lower());
}

template<class T, class Traits> inline
T median(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return rnd.median(x.lower(), x.upper());
}

template<class T, class Traits> inline
interval<T, Traits> widen(const interval<T, Traits>& x, const T& v)
{
  typename Traits::rounding rnd;
  return interval<T, Traits>(rnd.sub_down(x.lower(), v),
			     rnd.add_up  (x.upper(), v), true);
}

/*
 * Set-like operations
 */

template<class T, class Traits> inline
bool empty(const interval<T, Traits>& b)
{
  typedef typename Traits::checking checking;
  return checking::is_empty(b.lower(), b.upper());
}

template<class T, class Traits> inline
bool in_zero(const interval<T, Traits>& b)
{
  return b.lower() <= T(0) && T(0) <= b.upper();
}

template<class T, class Traits> inline
bool in(const T& r, const interval<T, Traits>& b)
{
  return b.lower() <= r && r <= b.upper();
}

template<class T, class Traits> inline
bool subset(const interval<T, Traits>& a,
	    const interval<T, Traits>& b)
{
  if (empty(a))
    return true;
  else
    return b.lower() <= a.lower() && a.upper() <= b.upper();
}

template<class T, class Traits> inline
bool proper_subset(const interval<T, Traits>& a,
		   const interval<T, Traits>& b)
{
  if (empty(a) && !empty(b))
    return true;
  else
    return b.lower() < a.lower() && a.upper() <= b.upper() ||
      b.lower() <= a.lower() && a.upper() <= b.upper();
}

template<class T, class Traits> inline
bool overlap(const interval<T, Traits>& x,
	     const interval<T, Traits>& y)
{
  return x.lower() <= y.lower() && y.lower() <= x.upper() ||
    y.lower() <= x.lower() && x.lower() <= y.upper();
}

template<class T, class Traits> inline
bool singleton(const interval<T, Traits>& x)
{
  return x.lower() == x.upper();
}

template<class T, class Traits> inline
bool equal(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() == y.lower() && x.upper() == y.upper();
}

template<class T, class Traits> inline
interval<T, Traits> intersect(const interval<T, Traits>& x,
			      const interval<T, Traits>& y)
{
  T l = std::max(x.lower(), y.lower());
  T u = std::min(x.upper(), y.upper());
  if (l <= u)
    return interval<T, Traits>(l, u, true);
  else
    return interval<T, Traits>::empty();
}

template<class T, class Traits> inline
interval<T, Traits> hull(const interval<T, Traits>& x,
			 const interval<T, Traits>& y)
{
  if (empty(x))
    return y;
  else if (empty(y))
    return x;
  else
    return interval<T, Traits>(std::min(x.lower(), y.lower()),
			       std::max(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> hull(const interval<T, Traits>& x, const T& y)
{
  if (empty(x))
    return interval<T, Traits>(y);
  else
    return interval<T, Traits>(std::min(x.lower(), y),
			       std::max(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> hull(const T& x, const interval<T, Traits>& y)
{
  if (empty(y))
    return interval<T, Traits>(x);
  else
    return interval<T, Traits>(std::min(x, y.lower()),
			       std::max(x, y.upper()), true);
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
  const T m = median(x);
  return std::pair<I,I>(I(x.lower(), m, true), I(m, x.upper(), true));
}

/*
 * Elementary functions
 */

template<class T, class Traits> inline
interval<T, Traits> abs(const interval<T, Traits>& x)
{
  if (!detail::sign(x.lower()))
    return x;
  else if (detail::sign(x.upper()))
    return -x;
  else {
    return interval<T, Traits>(0, std::max(-x.lower(), x.upper()), true);
  }
}

template<class T, class Traits> inline
interval<T, Traits> max(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::max(x.lower(), y.lower()),
			     std::max(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> max(const interval<T, Traits>& x, const T& y)
{
  return interval<T, Traits>(std::max(x.lower(), y),
			     std::max(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> max(const T& x, const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::max(x, y.lower()),
			     std::max(x, y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> min(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::min(x.lower(), y.lower()),
			     std::min(x.upper(), y.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> min(const interval<T, Traits>& x, const T& y)
{
  return interval<T, Traits>(std::min(x.lower(), y),
			     std::min(x.upper(), y), true);
}

template<class T, class Traits> inline
interval<T, Traits> min(const T& x, const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::min(x, y.lower()),
			     std::min(x, y.upper()), true);
}

  namespace interval_lib {

template<class I> inline
I pi()
{
  typedef typename I::traits_type::rounding rounding;
  return I(rounding::pi_down(), rounding::pi_up(), true);
}

template<class I> inline
I pi_1_2()
{
  typedef typename I::traits_type::rounding rounding;
  return I(rounding::pi_1_2_down(), rounding::pi_1_2_up(), true);
}

template<class I> inline
I pi_2_1()
{
  typedef typename I::traits_type::rounding rounding;
  return I(rounding::pi_2_1_down(), rounding::pi_2_1_up(), true);
}

  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_UTILITY_HPP
