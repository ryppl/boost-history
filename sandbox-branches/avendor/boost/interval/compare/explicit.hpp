#ifndef BOOST_INTERVAL_COMPARE_EXPLICIT_HPP
#define BOOST_INTERVAL_COMPARE_EXPLICIT_HPP

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
} //namespace boost

#endif // BOOST_INTERVAL_COMPARE_EXPLICIT_HPP
