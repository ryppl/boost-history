#ifndef BOOST_DETAIL_INTERVAL_CMP_HPP
#define BOOST_DETAIL_INTERVAL_CMP_HPP

namespace boost {

#if 0

/*
 * Equality comparison
 */

template<class T, class Traits>
inline bool operator==(const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  return x.lower() == y.lower() && x.upper() == y.upper();
}

#ifndef BOOST_INTERVAL_USE_FRIEND_OPERATORS
template<class T, class Traits>
inline bool operator!=(const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  return !(x == y);
}
#endif // BOOST_INTERVAL_USE_FRIEND_OPERATORS

#endif

/*
 * Certainly... operations
 */

template<class T, class Traits>
inline bool cerlt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() < y.lower();
}

template<class T, class Traits>
inline bool cerlt(const interval<T, Traits>& x, const T& y)
{
  return x.upper() < y;
}

template<class T, class Traits>
inline bool cerlt(const T& x, const interval<T, Traits>& y)
{
  return x < y.lower();
}

template<class T, class Traits>
inline bool cerle(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() <= y.lower();
}

template<class T, class Traits>
inline bool cerle(const interval<T, Traits>& x, const T& y)
{
  return x.upper() <= y;
}

template<class T, class Traits>
inline bool cerle(const T& x, const interval<T, Traits>& y)
{
  return x <= y.lower();
}

template<class T, class Traits>
inline bool cergt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return y.upper() < x.lower();
}

template<class T, class Traits>
inline bool cergt(const interval<T, Traits>& x, const T& y)
{
  return y < x.lower();
}

template<class T, class Traits>
inline bool cergt(const T& x, const interval<T, Traits>& y)
{
  return y.upper() < x;
}

template<class T, class Traits>
inline bool cerge(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return y.upper() <= x.lower();
}

template<class T, class Traits>
inline bool cerge(const interval<T, Traits>& x, const T& y)
{
  return y <= x.lower();
}

template<class T, class Traits>
inline bool cerge(const T& x, const interval<T, Traits>& y)
{
  return y.upper() <= x;
}

template<class T, class Traits>
inline bool cereq(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() == y.lower() && singleton(x) && singleton(y);
}

template<class T, class Traits>
inline bool cereq(const interval<T, Traits>& x, const T& y)
{
  return x.lower() == y && singleton(x);
}

template<class T, class Traits>
inline bool cereq(const T& x, const interval<T, Traits>& y)
{
  return x == y.lower() && singleton(y);
}

template<class T, class Traits>
inline bool cerne(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return !overlap(x, y);
}

template<class T, class Traits>
inline bool cerne(const interval<T, Traits>& x, const T& y)
{
  return !in(y, x);
}

template<class T, class Traits>
inline bool cerne(const T& x, const interval<T, Traits>& y)
{
  return !in(x, y);
}

/*
 * Possibly... comparisons
 */

template<class T, class Traits>
inline bool poslt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() < y.upper();
}

template<class T, class Traits>
inline bool poslt(const interval<T, Traits>& x, const T& y)
{
  return x.lower() < y;
}

template<class T, class Traits>
inline bool poslt(const T& x, const interval<T, Traits>& y)
{
  return x < y.upper();
}

template<class T, class Traits>
inline bool posle(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.lower() <= y.upper();
}

template<class T, class Traits>
inline bool posle(const interval<T, Traits>& x, const T& y)
{
  return x.lower() <= y;
}

template<class T, class Traits>
inline bool posle(const T& x, const interval<T, Traits>& y)
{
  return x <= y.upper();
}

template<class T, class Traits>
inline bool posgt(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() > y.lower();
}

template<class T, class Traits>
inline bool posgt(const interval<T, Traits>& x, const T& y)
{
  return x.lower() > y;
}

template<class T, class Traits>
inline bool posgt(const T& x, const interval<T, Traits> & y)
{
  return x > y.upper();
}

template<class T, class Traits>
inline bool posge(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return x.upper() >= y.lower();
}

template<class T, class Traits>
inline bool posge(const interval<T, Traits>& x, const T& y)
{
  return x.upper() >= y;
}

template<class T, class Traits>
inline bool posge(const T& x, const interval<T, Traits>& y)
{
  return x >= y.lower();
}

template<class T, class Traits>
inline bool poseq(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return overlap(x, y);
}

template<class T, class Traits>
inline bool poseq(const interval<T, Traits>& x, const T& y)
{
  return in(y, x);
}

template<class T, class Traits>
inline bool poseq(const T& x, const interval<T, Traits>& y)
{
  return in(x, y);
}

template<class T, class Traits>
inline bool posne(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  return !cereq(x, y);
}

template<class T, class Traits>
inline bool posne(const interval<T, Traits>& x, const T& y)
{
  return !cereq(x, y);
}

template<class T, class Traits>
inline bool posne(const T& x, const interval<T, Traits>& y)
{
  return !cereq(x, y);
}

/* Comparison operators */

template<class T, class Traits>
inline bool operator< (const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits>
inline bool operator< (const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(x, x, y.lower(), y.upper());
}

template<class T, class Traits>
inline bool operator< (const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(x.lower(), x.upper(), y, y);
}

template<class T, class Traits>
inline bool operator>=(const interval<T, Traits>& x, 
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits>
inline bool operator>=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(x, x, y.lower(), y.upper());
}

template<class T, class Traits>
inline bool operator>=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(x.lower(), x.upper(), y, y);
}

#define _m_ibc_ interval<T, Traits>

#define _m2_ibc_(a, b) \
  _m1_ibc_(a, b, _m_ibc_, _m_ibc_) \
  _m1_ibc_(a, b, _m_ibc_, T) \
  _m1_ibc_(a, b, T, _m_ibc_)

#define _m1_ibc_(A, B, C, D) \
  template<class T, class Traits> \
  inline bool \
  operator A(const C& x, const D& y) \
  { \
    return y B x; \
  }

_m2_ibc_(> , <)
_m2_ibc_(<=, >=)

#undef _m1_ibc_
#undef _m2_ibc_
#undef _m_ibc_

  namespace interval_lib {

template<class T>
struct compare_certainly
{
  typedef const T& ref;
  static bool lessthan(ref a1, ref a2, ref b1, ref b2)
  { return a2 < b1; }
};

template<class T>
struct compare_possibly
{
  typedef const T& ref;
  static bool lessthan(ref a1, ref a2, ref b1, ref b2)
  { return a1 < b2; }
};

template<class T, class F>
struct compare_full
{
  typedef const T& ref;
  static bool lessthan(ref a1, ref a2, ref b1, ref b2)
  { return a2 < b1 || (!(b2 < a1) && F()()); }
};

  } // namespace interval
} // namespace boost

#endif // BOOST_DETAIL_INTERVAL_CMP_HPP
