/* boost interval.hpp header file
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
 *  2000-03-07  added overlap, intersect, combine, scale, symmetric_scale;
 *              Maarten Keijzer contributed msvc_rounding_control.hpp,
 *              adapted interval_traits for MSVC
 *  2000-03-08  added operator++, operator--, hull, bisect, dist, pred, succ,
 *              specialization of std::numeric_limits<>
 *  2000-03-09  switched to non-member functions, interval_traits_exceptions
 *  2000-03-14  added Chiriaev and Walster extensions to pow
 *  2000-03-30  further speed improvements
 *  2000-04-10  rounding mode control is not part of the interface; cleanup
 *  2000-09-24  separated implementation from interface
 */

/*
 * Local configuration settings
 *
 * BOOST_INTERVAL_USE_FRIEND_OPERATORS:  Use <boost/operators.hpp>
 * (increases the struct interval size from 16 to 24 bytes and reduces
 * performance for + - * / by about 50%)
 *
 * BOOST_INTERVAL_NO_INLINE:  Don't include + - * / and I/O template
 * function definitions to avoid inlining and thus save space.
 *
 * BOOST_INTERVAL_HAVE_LIBRARY:  Assume that transcendental function
 * definitions are provided by an object code library.
 */

#ifndef BOOST_INTERVAL_HPP
#define BOOST_INTERVAL_HPP

// FIXME: nécessaire pour utiliser g++ 3.*
#if __GNUC__==3
# define BOOST_NO_LIMITS
# define _CPP_NUMERIC_LIMITS
#endif

#include <boost/limits.hpp>
#include <cmath>         // for abs, exp, log, sin, cos, tan, etc.
#include <iosfwd>
#include <boost/config.hpp>
//#include <boost/mathconst.hpp>

#ifdef BOOST_INTERVAL_USE_FRIEND_OPERATORS
#include <boost/operators.hpp>
#endif

#include <boost/interval/rounded_transc.hpp>
#include <boost/interval/rounded_arith.hpp>
#include <boost/interval/checking.hpp>

namespace boost {

  namespace interval_lib {
    template<class T>
    struct compare_certainly;
  }

// default traits class
template<class T,
	 class Compare = interval_lib::compare_certainly<T>,
	 class Rounding = interval_lib::rounded_arithmetic<T>,
	 class Checking = interval_lib::checking_nothing<T> >
struct interval_traits
{
  typedef T base_type;
  typedef Compare compare;
  typedef Rounding rounding;
  typedef Checking checking;
};

  namespace interval_lib {
    namespace detail {

template<class T, class Compare, class Rounding, class Checking>
struct unprotect<interval_traits<T, Compare, Rounding, Checking> > {
  typedef
    interval_traits<T, Compare, unprotect<Rounding>::type, Checking> type;
};

    }
  }

template<class T, class Traits = interval_traits<T> >
class interval
#ifdef BOOST_INTERVAL_USE_FRIEND_OPERATORS
// gcc enlarges this class from 16 to 24 bytes with these base classes :-(
// other compilers may be worse, so avoid for now
  :
  addable1<interval<T, Traits>,
  addable2<interval<T, Traits>, T,
  subtractable1<interval<T, Traits>,
  subtractable2<interval<T, Traits>, T,
  multipliable1<interval<T, Traits>,
  multipliable2<interval<T, Traits>, T,
  dividable1<interval<T, Traits>,
  dividable2<interval<T, Traits>, T,
  equality_comparable1<interval<T, Traits> > > > > > > > > >
#endif  // BOOST_INTERVAL_USE_FRIEND_OPERATORS
{
public:
  typedef T base_type;
  typedef std::numeric_limits<base_type> base_limits;
  typedef Traits traits_type;

  interval(const T& value = 0);
  interval(const T& l, const T& u);

  template<class Traits2>
  interval(const interval<T, Traits2>& r): low(r.lower()), up(r.upper()) {}

  // compiler-generated copy constructor and assignment operator are fine
  interval& operator=(const T& x);
  void assign(const T& l, const T& u);

  // constructor-like functions
  static interval empty();
  static interval entire();
  static interval hull(const T& x, const T& y);

  T lower() const { return low; }
  T upper() const { return up; }

  interval& operator+= (const T& r);
  interval& operator+= (const interval& r);
  interval& operator-= (const T& r);
  interval& operator-= (const interval& r);
  interval& operator*= (const T& r);
  interval& operator*= (const interval& r);
  interval& operator/= (const T& r);
  interval& operator/= (const interval& r);

  // the following is for internal use only, it is not a published interface
  // nevertheless, it's public because friends don't always work correctly.
  interval(const T& l, const T& u, bool): low(l), up(u) {}
  void set_empty();
  void set_entire();
  void set(const T& l, const T& u);

private:
  T low;
  T up;
};


/*
 * Non-Member Function Declarations
 */

template<class T, class Traits>
interval<T, Traits> operator+(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> operator-(const interval<T, Traits>& x);

template<class T, class Traits>
interval<T, Traits> operator+(const interval<T, Traits>& x,
			      const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator+(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator+(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> operator-(const interval<T, Traits>& x,
			      const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator-(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator-(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> operator*(const interval<T, Traits>& x,
			      const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator*(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator*(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> operator/(const interval<T, Traits>& x,
			      const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator/(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator/(const T& r, const interval<T, Traits>& x);

template<class T, class Traits>
bool cerlt(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool cerlt(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool cerlt(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool cerle(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool cerle(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool cerle(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool cergt(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool cergt(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool cergt(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool cerge(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool cerge(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool cerge(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool cereq(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool cereq(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool cereq(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool cerne(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool cerne(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool cerne(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool poslt(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool poslt(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool poslt(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool posle(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool posle(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool posle(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool posgt(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool posgt(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool posgt(const T& x, const interval<T, Traits> & y);

template<class T, class Traits>
bool posge(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool posge(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool posge(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool poseq(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool poseq(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool poseq(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool posne(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool posne(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool posne(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool operator<(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool operator<(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool operator<(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool operator<=(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool operator<=(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool operator<=(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool operator>(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool operator>(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool operator>(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool operator>=(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool operator>=(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool operator>=(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool operator==(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool operator==(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool operator==(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
bool operator!=(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
bool operator!=(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
bool operator!=(const T& x, const interval<T, Traits>& y);

#if 0
template<class Ch, class ChTr, class T, class Traits>
std::basic_ostream<Ch, ChTr>&
operator<<(std::basic_ostream<Ch, ChTr>& os, const interval<T, Traits>& r);
template<class Ch, class ChTr, class T, class Traits>
std::basic_istream<Ch, ChTr>&
operator>>(std::basic_istream<Ch, ChTr>& is, const interval<T, Traits>& r);
#endif

template<class T, class Traits> T lower(const interval<T, Traits>& x);
template<class T, class Traits> T upper(const interval<T, Traits>& x);
template<class T, class Traits> T width(const interval<T, Traits>& x);
template<class T, class Traits> T median(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> pred(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> succ(const interval<T, Traits>& x);

template<class T, class Traits>
bool empty(const interval<T, Traits>& b);
template<class T, class Traits>
bool in(const T& r, const interval<T, Traits>& b);
template<class T, class Traits>
bool subset(const interval<T, Traits>& a, const interval<T, Traits>& b);
template<class T, class Traits>
bool proper_subset(const interval<T, Traits>& a,
		   const interval<T, Traits>& b);
template<class T, class Traits>
bool overlap(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> intersect(const interval<T, Traits>& x,
                              const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> hull(const interval<T, Traits>& x,
			 const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> hull(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> hull(const T& x, const interval<T, Traits>& y);
template<class T>
interval<T> hull(const T& x, const T& y);

template<class T, class Traits>
std::pair<interval<T, Traits>, interval<T, Traits> >
bisect(const interval<T, Traits>& x);

template<class T, class Traits>
T dist(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
T dist(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
T dist(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> scale(const interval<T, Traits>& x,
			  const T& mirror, const T& factor);
template<class T, class Traits>
interval<T, Traits> symmetric_scale(const interval<T, Traits>& x,
				    const T& factor);

template<class T, class Traits>
interval<T, Traits> square(const interval<T, Traits>& x);

template<class T, class Traits>
interval<T, Traits> fmod(const interval<T, Traits>& x,
			 const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> fmod(const interval<T, Traits>& x, const T& y);

template<class T, class Traits>
interval<T, Traits> abs(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> sqrt(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> exp(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> log(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> log10(const interval<T, Traits>& x);
template<class T> T pow(const T& x, long y);
template<class T, class Traits>
interval<T, Traits> pow(const interval<T, Traits>& x,
			const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> sin(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> cos(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> tan(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> asin(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> acos(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> atan(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> atan2(const interval<T, Traits>& y,
			  const interval<T, Traits>& x);

template<class T, class Traits>
interval<T, Traits> sinh(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> cosh(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> tanh(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> asinh(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> acosh(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> atanh(const interval<T, Traits>& x);

} // namespace boost


#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace std {
template<class T, class Traits>
class numeric_limits<boost::interval<T, Traits> >
  : public std::numeric_limits<T>
{
private:
  typedef boost::interval<T, Traits> vt;
  typedef numeric_limits<T> bl;
public:
  static vt min() throw() { return vt(bl::min(), bl::min()); }
  static vt max() throw() { return vt(bl::max(), bl::max()); }
  static vt epsilon() throw() { return vt(bl::epsilon(), bl::epsilon()); }

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
  static const float_round_style round_style = round_indeterminate;
  static const bool is_iec559 = false;
#else
  enum {
    round_style = round_indeterminate,
    is_iec559 = false
  };
#endif

  static vt infinity() throw() { return vt::entire(); }
  static vt quiet_NaN() throw() { return vt::empty(); }
  static vt signaling_NaN() throw()
  { return vt(bl::signaling_NaN(), bl::signaling_Nan()); }
  static vt denorm_min() throw()
  { return vt(bl::denorm_min(), bl::denorm_min()); }
private:
  static vt round_error();    // hide this on purpose, not yet implemented
};

#if 0
// the order for std::less should not be totally different from operator<
// total order, for std::map keys
template<class T, class Traits>
struct less<boost::interval<T, Traits> >
{
  bool operator()(const boost::interval<T, Traits> & x,
		  const boost::interval<T, Traits> & y)
  {
    return lower(x) < lower(y) ||
      (lower(x) == lower(y) && upper(x) < upper(y));
  }
};
#endif

} // namespace std
#endif


/*
 * Implementation of simple functions
 */

namespace boost {

template<class T, class Traits>
inline void interval<T, Traits>::set_empty()
{
  low = up = std::numeric_limits<T>::quiet_NaN();
}

template<class T, class Traits>
inline void interval<T, Traits>::set_entire()
{
  low = -std::numeric_limits<T>::infinity();
  up = std::numeric_limits<T>::infinity();
}

template<class T, class Traits>
inline interval<T, Traits>::interval(const T& x)
{
  *this = x;
}

template<class T, class Traits>
inline interval<T, Traits>::interval(const T& l, const T& u): low(l), up(u)
{
  if (low > up) {
    typedef typename Traits::checking checking;
    checking::inverted_bound(l, u);
    set_empty();
  }
}

template<class T, class Traits>
inline interval<T, Traits>& interval<T, Traits>::operator=(const T& x)
{
  low = up = x;
  return *this;
}

template<class T, class Traits>
inline void interval<T, Traits>::assign(const T& l, const T& u)
{
  if (l > u) {
    typedef typename Traits::checking checking;
    checking::inverted_bound(l, u);
    set_empty();
  } else set(l, u);
}

template<class T, class Traits>
inline void interval<T, Traits>::set(const T& l, const T& u)
{
  low = l;
  up = u;
}

template<class T, class Traits>
inline interval<T, Traits> interval<T, Traits>::hull(const T& x, const T& y)
{
  if (x < y)
    return interval<T, Traits>(x, y, true);
  else
    return interval<T, Traits>(y, x, true);
}

template<class T, class Traits>
inline interval<T, Traits> interval<T, Traits>::empty()
{
  return interval<T, Traits>(std::numeric_limits<T>::quiet_NaN(),
			     std::numeric_limits<T>::quiet_NaN(), true);
}

template<class T, class Traits>
inline interval<T, Traits> interval<T, Traits>::entire()
{
  return interval<T, Traits>(-std::numeric_limits<T>::infinity(),
			     std::numeric_limits<T>::infinity(), true);
}


/*
 * Utility Functions
 */

template<class T, class Traits>
inline T lower(const interval<T, Traits>& x)
{
  return x.lower();
}

template<class T, class Traits>
inline T upper(const interval<T, Traits>& x)
{
  return x.upper();
}

template<class T, class Traits>
inline T width(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  return rnd.sub_up(x.upper(), x.lower());
}

template<class T, class Traits>
inline T median(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  rnd.tonearest();
  return (x.lower() + x.upper())/2;
}

template<class T, class Traits>
inline interval<T, Traits> pred(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  T l = rnd.add_up(x.lower(), std::numeric_limits<T>::min());
  T u = rnd.sub_down(x.upper(), std::numeric_limits<T>::min());
  if (u < l)
    // We could not shrink the interval any further
    return x;
  else
    return interval<T, Traits>(l, u, true);
}

template<class T, class Traits>
inline interval<T, Traits> succ(const interval<T, Traits>& x)
{
  typename Traits::rounding rnd;
  T l = rnd.sub_down(x.lower(), std::numeric_limits<T>::min());
  T u = rnd.add_up(x.upper(), std::numeric_limits<T>::min());
  return interval<T, Traits>(l, u);
}

/*
 * Set-like operations
 */

template<class T, class Traits>
bool empty(const interval<T, Traits>& b)
{
  return detail::is_nan(b.lower()) || detail::is_nan(b.upper());
}

template<class T, class Traits>
bool in_zero(const interval<T, Traits>& b)
{
  return b.lower() <= T(0) && T(0) <= b.upper();
}

template<class T, class Traits>
bool in(const T& r, const interval<T, Traits> & b)
{
  return b.lower() <= r && r <= b.upper();
}

template<class T, class Traits>
bool subset(const interval<T, Traits>& a,
	    const interval<T, Traits>& b)
{
  if (empty(a))
    return true;
  else
    return b.lower() <= a.lower() && a.upper() <= b.upper();
}

template<class T, class Traits>
bool proper_subset(const interval<T, Traits> & a,
		   const interval<T, Traits> & b)
{
  if (empty(a) && !empty(b))
    return true;
  else
    return b.lower() < a.lower() && a.upper() <= b.upper() ||
      b.lower() <= a.lower() && a.upper() <= b.upper();
}

template<class T, class Traits>
inline bool overlap(const interval<T, Traits>& x,
		    const interval<T, Traits>& y)
{
  return x.lower() <= y.lower() && y.lower() <= x.upper() ||
    y.lower() <= x.lower() && x.lower() <= y.upper();
}

template<class T, class Traits>
inline bool singleton(const interval<T, Traits>& x)
{
  return x.lower() == x.upper();
}

template<class T, class Traits>
inline bool equal(const interval<T, Traits>& x,
		  const interval<T, Traits>& y)
{
  return x.lower() == y.lower() && x.upper() == y.upper();
}

template<class T, class Traits>
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

template<class T, class Traits>
inline interval<T, Traits> hull(const interval<T, Traits>& x,
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

template<class T, class Traits>
inline interval<T, Traits> hull(const interval<T, Traits>& x, const T& y)
{
  if (empty(x))
    return interval<T, Traits>(y);
  else
    return interval<T, Traits>(std::min(x.lower(), y),
			       std::max(x.upper(), y), true);
}

template<class T, class Traits>
inline interval<T, Traits> hull(const T& x, const interval<T, Traits>& y)
{
  if (empty(y))
    return interval<T, Traits>(x);
  else
    return interval<T, Traits>(std::min(x, y.lower()),
			       std::max(x, y.upper()), true);
}

template<class T>
inline interval<T> hull(const T& x, const T& y)
{
  return interval<T>::hull(x, y);
}

template<class T, class Traits>
inline std::pair<interval<T, Traits>, interval<T, Traits> >
bisect(const interval<T, Traits>& x)
{
  typedef interval<T, Traits> I;
  T m = median(x);
  return std::pair<I,I>(I(x.lower(), m), I(m, x.upper()));
}

template<class T, class Traits>
T dist(const interval<T, Traits>& x, const interval<T, Traits>& y)
{
  using std::abs;
  typename Traits::rounding rnd;
  rnd.tonearest();
  return std::min(abs(x.lower() - y.lower()), abs(x.upper() - y.upper()));
}

template<class T, class Traits>
T dist(const interval<T, Traits>& x, const T& y)
{
  typename Traits::rounding rnd;
  rnd.tonearest();
  if (y < x.lower())
    return y - x.lower();
  else if (y > x.upper())
    return x.upper() - y;
  else
    return 0;
}

template<class T, class Traits>
T dist(const T& x, const interval<T, Traits>& y)
{
  return -dist(y,x);
}

template<class T, class Traits>
inline interval<T, Traits> scale(const interval<T, Traits>& x,
				 const T& mirror, const T& factor)
{
  return factor * (x - mirror);
}

template<class T, class Traits>
interval<T, Traits> symmetric_scale(const interval<T, Traits>& x,
				    const T& factor)
{
  return scale(x, median(x), factor);
}


/*
 * Elementary functions
 */

template<class T, class Traits>
inline interval<T, Traits> abs(const interval<T, Traits>& x)
{
  if (!detail::sign(x.lower()))
    return x;
  else if (detail::sign(x.upper()))
    return -x;
  else {
    return interval<T, Traits>(T(0), std::max(-x.lower(), x.upper()), true);
  }
}

template<class T, class Traits>
interval<T, Traits> max(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::max(x.lower(), y.lower()),
			     std::max(x.upper(), y.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> max(const interval<T, Traits>& x, const T& y)
{
  return interval<T, Traits>(std::max(x.lower(), y),
			     std::max(x.upper(), y), true);
}

template<class T, class Traits>
interval<T, Traits> max(const T& x, const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::max(x, y.lower()),
			     std::max(x, y.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> min(const interval<T, Traits>& x,
			const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::min(x.lower(), y.lower()),
			     std::min(x.upper(), y.upper()), true);
}

template<class T, class Traits>
interval<T, Traits> min(const interval<T, Traits>& x, const T& y)
{
  return interval<T, Traits>(std::min(x.lower(), y),
			     std::min(x.upper(), y), true);
}

template<class T, class Traits>
interval<T, Traits> min(const T& x, const interval<T, Traits>& y)
{
  return interval<T, Traits>(std::min(x, y.lower()),
			     std::min(x, y.upper()), true);
}

} // namespace boost

#include <boost/interval/compare.hpp>

#ifndef BOOST_INTERVAL_NO_INLINE
#include <boost/interval/oper.hpp>
#endif // BOOST_INTERVAL_NO_INLINE
#ifndef BOOST_INTERVAL_HAVE_LIBRARY
#include <boost/interval/misc.hpp>
#include <boost/interval/transc.hpp>
#endif // BOOST_INTERVAL_HAVE_LIBRARY

#endif // BOOST_INTERVAL_HPP
