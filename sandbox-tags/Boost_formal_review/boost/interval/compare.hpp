/* Boost interval/compare.hpp template implementation file
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

#ifndef BOOST_DETAIL_INTERVAL_CMP_HPP
#define BOOST_DETAIL_INTERVAL_CMP_HPP

namespace boost {
  namespace interval_lib {

/*
 * Certainly... operations
 */

template<class T, class Traits> inline
bool cerlt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() < y.lower();
}

template<class T, class Traits> inline
bool cerlt(const interval<T, Traits>& x, const T& y)
{
  return x.upper() < y;
}

template<class T, class Traits> inline
bool cerlt(const T& x, const interval<T, Traits>& y)
{
  return x < y.lower();
}

template<class T, class Traits> inline
bool cerle(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() <= y.lower();
}

template<class T, class Traits> inline
bool cerle(const interval<T, Traits>& x, const T& y)
{
  return x.upper() <= y;
}

template<class T, class Traits> inline
bool cerle(const T& x, const interval<T, Traits>& y)
{
  return x <= y.lower();
}

template<class T, class Traits> inline
bool cergt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() > y.upper();
}

template<class T, class Traits> inline
bool cergt(const interval<T, Traits>& x, const T& y)
{
  return x.lower() > y;
}

template<class T, class Traits> inline
bool cergt(const T& x, const interval<T, Traits>& y)
{
  return x > y.upper();
}

template<class T, class Traits> inline
bool cerge(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() >= y.upper();
}

template<class T, class Traits> inline
bool cerge(const interval<T, Traits>& x, const T& y)
{
  return x.lower() >= y;
}

template<class T, class Traits> inline
bool cerge(const T& x, const interval<T, Traits>& y)
{
  return x >= y.upper();
}

template<class T, class Traits> inline
bool cereq(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() == y.upper() && y.lower() == x.upper();
}

template<class T, class Traits> inline
bool cereq(const interval<T, Traits>& x, const T& y)
{
  return x.lower() == y && x.upper() == y;
}

template<class T, class Traits> inline
bool cereq(const T& x, const interval<T, Traits>& y)
{
  return x == y.lower() && x == y.upper();
}

template<class T, class Traits> inline
bool cerne(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() < y.lower() || y.upper() < x.lower();
}

template<class T, class Traits> inline
bool cerne(const interval<T, Traits>& x, const T& y)
{
  return x.upper() < y || y < x.lower();
}

template<class T, class Traits> inline
bool cerne(const T& x, const interval<T, Traits>& y)
{
  return x < y.lower() || y.upper() < x;
}

/*
 * Possibly... comparisons
 */

template<class T, class Traits> inline
bool poslt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() < y.upper();
}

template<class T, class Traits> inline
bool poslt(const interval<T, Traits>& x, const T& y)
{
  return x.lower() < y;
}

template<class T, class Traits> inline
bool poslt(const T& x, const interval<T, Traits>& y)
{
  return x < y.upper();
}

template<class T, class Traits> inline
bool posle(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() <= y.upper();
}

template<class T, class Traits> inline
bool posle(const interval<T, Traits>& x, const T& y)
{
  return x.lower() <= y;
}

template<class T, class Traits> inline
bool posle(const T& x, const interval<T, Traits>& y)
{
  return x <= y.upper();
}

template<class T, class Traits> inline
bool posgt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() > y.lower();
}

template<class T, class Traits> inline
bool posgt(const interval<T, Traits>& x, const T& y)
{
  return x.upper() > y;
}

template<class T, class Traits> inline
bool posgt(const T& x, const interval<T, Traits> & y)
{
  return x > y.lower();
}

template<class T, class Traits> inline
bool posge(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() >= y.lower();
}

template<class T, class Traits> inline
bool posge(const interval<T, Traits>& x, const T& y)
{
  return x.upper() >= y;
}

template<class T, class Traits> inline
bool posge(const T& x, const interval<T, Traits>& y)
{
  return x >= y.lower();
}

template<class T, class Traits> inline
bool poseq(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() >= y.lower() && y.upper() >= x.lower();
}

template<class T, class Traits> inline
bool poseq(const interval<T, Traits>& x, const T& y)
{
  return x.upper() >= y && y >= x.lower();
}

template<class T, class Traits> inline
bool poseq(const T& x, const interval<T, Traits>& y)
{
  return x >= y.lower() && y.upper() >= x;
}

template<class T, class Traits> inline
bool posne(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() != y.lower() || y.upper() != x.lower();
}

template<class T, class Traits> inline
bool posne(const interval<T, Traits>& x, const T& y)
{
  return x.upper() != y || y != x.lower();
}

template<class T, class Traits> inline
bool posne(const T& x, const interval<T, Traits>& y)
{
  return x != y.lower() || y.upper() != x;
}

  } // namespace interval_lib

/* Comparison operators */

template<class T, class Traits> inline
bool operator<(const interval<T, Traits>& x,
		      const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::less_than(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator<(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::less_than(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator<(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::less_than(x.lower(), x.upper(), y, y);
}

template<class T, class Traits> inline
bool operator>(const interval<T, Traits>& x,
		      const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::less_than(y.lower(), y.upper(), x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator>(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::less_than(y.lower(), y.upper(), x, x);
}

template<class T, class Traits> inline
bool operator>(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::less_than(y, y, x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator<=(const interval<T, Traits>& x, 
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::less_than(y.lower(), y.upper(), x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator<=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::less_than(y.lower(), y.upper(), x, x);
}

template<class T, class Traits> inline
bool operator<=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::less_than(y, y, x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator>=(const interval<T, Traits>& x, 
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::less_than(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator>=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::less_than(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator>=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::less_than(x.lower(), x.upper(), y, y);
}

template<class T, class Traits> inline
bool operator==(const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::equal_to(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator==(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::equal_to(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator==(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::equal_to(x.lower(), x.upper(), y, y);
}

template<class T, class Traits> inline
bool operator!=(const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::equal_to(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator!=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::equal_to(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator!=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::equal_to(x.lower(), x.upper(), y, y);
}


  namespace interval_lib {

template<class T>
struct compare_certainly
{
  static bool less_than(const T&, const T& xu, const T& yl, const T&)
  { return xu < yl; }
  static bool equal_to(const T& xl, const T& xu, const T& yl, const T& yu)
  { return xu == yl && yu == xl; }
};

template<class T>
struct compare_possibly
{
  static bool less_than(const T& xl, const T&, const T&, const T& yu)
  { return xl < yu; }
  static bool equal_to(const T& xl, const T& xu, const T& yl, const T& yu)
  { return !(xu < yl || yu < xl); }
};

template<class T, class F>
struct compare_full
{
  static bool less_than(const T& xl, const T& xu, const T& yl, const T& yu)
  { return xu < yl || (xl < yu && F()()); }
  static bool equal_to(const T& xl, const T& xu, const T& yl, const T& yu)
  { return (xu == yl && yu == xl) || (!(xu < yl || yu < xl) && F()()); }
};

template<class T>
struct compare_data
{
  static bool less_than(const T& xl, const T&, const T&, const T& yu)
  { return xl < yl || (xl == yl && xu < yu); }
  static bool equal_to(const T& xl, const T& xu, const T& yl, const T& yu)
  { return xl == yl && xu == yu; }
};

  } // namespace interval_lib
} // namespace boost

#endif // BOOST_DETAIL_INTERVAL_CMP_HPP
