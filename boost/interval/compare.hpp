#ifndef BOOST_DETAIL_INTERVAL_CMP_HPP
#define BOOST_DETAIL_INTERVAL_CMP_HPP

namespace boost {

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

/* Comparison operators */

template<class T, class Traits> inline
bool operator<(const interval<T, Traits>& x,
		      const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator<(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator<(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(x.lower(), x.upper(), y, y);
}

template<class T, class Traits> inline
bool operator>(const interval<T, Traits>& x,
		      const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(y.lower(), y.upper(), x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator>(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(y.lower(), y.upper(), x, x);
}

template<class T, class Traits> inline
bool operator>(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::lessthan(y, y, x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator<=(const interval<T, Traits>& x, 
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(y.lower(), y.upper(), x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator<=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(y.lower(), y.upper(), x, x);
}

template<class T, class Traits> inline
bool operator<=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(y, y, x.lower(), x.upper());
}

template<class T, class Traits> inline
bool operator>=(const interval<T, Traits>& x, 
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator>=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator>=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::lessthan(x.lower(), x.upper(), y, y);
}

template<class T, class Traits> inline
bool operator==(const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::equalto(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator==(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return compare::equalto(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator==(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return compare::equalto(x.lower(), x.upper(), y, y);
}

template<class T, class Traits> inline
bool operator!=(const interval<T, Traits>& x,
		       const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::equalto(x.lower(), x.upper(), y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator!=(const T& x, const interval<T, Traits>& y)
{
  typedef typename Traits::compare compare;
  return !compare::equalto(x, x, y.lower(), y.upper());
}

template<class T, class Traits> inline
bool operator!=(const interval<T, Traits>& x, const T& y)
{
  typedef typename Traits::compare compare;
  return !compare::equalto(x.lower(), x.upper(), y, y);
}


  namespace interval_lib {

template<class T>
struct compare_certainly
{
  static bool lessthan(const T&, const T& xu, const T& yl, const T&)
  { return xu < yl; }
  static bool equalto(const T& xl, const T& xu, const T& yl, const T& yu)
  { return xu == yl && yu == xl; }
};

template<class T>
struct compare_possibly
{
  static bool lessthan(const T& xl, const T&, const T&, const T& yu)
  { return xl < yu; }
  static bool equalto(const T& xl, const T& xu, const T& yl, const T& yu)
  { return !(xu < yl || yu < xl); }
};

template<class T, class F>
struct compare_full
{
  static bool lessthan(const T& xl, const T& xu, const T& yl, const T& yu)
  { return xu < yl || (xl < yu && F()()); }
  static bool equalto(const T& xl, const T& xu, const T& yl, const T& yu)
  { return (xu == yl && yu == xl) || (!(xu < yl || yu < xl) && F()()); }
};

template<class T>
struct compare_data
{
  static bool lessthan(const T& xl, const T&, const T&, const T& yu)
  { return xl < yl || (xl == yl && xu < yu); }
  static bool equalto(const T& xl, const T& xu, const T& yl, const T& yu)
  { return xl == yl && xu == yu; }
};

  } // namespace interval
} // namespace boost

#endif // BOOST_DETAIL_INTERVAL_CMP_HPP
