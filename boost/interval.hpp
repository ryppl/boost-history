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
 */

#ifndef BOOST_INTERVAL_HPP
#define BOOST_INTERVAL_HPP

#include <boost/config.hpp>
#include <boost/limits.hpp>

namespace boost {
  namespace interval_lib {

template<class T> struct compare_certainly;
template<class T> struct rounded_math;
template<class T> struct checking_strict;

  } // namespace interval_lib

// default traits class
template<class T,
	 class Compare = interval_lib::compare_certainly<T>,
	 class Rounding = interval_lib::rounded_math<T>,
	 class Checking = interval_lib::checking_strict<T> >
struct interval_traits
{
  typedef T base_type;
  typedef Compare compare;
  typedef Rounding rounding;
  typedef Checking checking;
};

template<class T, class Traits = interval_traits<T> >
class interval
{
public:
  typedef T base_type;
  typedef std::numeric_limits<base_type> base_limits;
  typedef Traits traits_type;

  interval(const T& v = 0): low(v), up(v) {}
  interval(const T& l, const T& u);

  template<class Traits2>
  interval(const interval<T, Traits2>& r): low(r.lower()), up(r.upper()) {}

  // compiler-generated copy constructor and assignment operator are fine

  interval& operator=(const T& x);
  void assign(const T& l, const T& u);

  static interval empty();
  static interval entire();
  static interval hull(const T& x, const T& y);

  const T& lower() const { return low; }
  const T& upper() const { return up;  }

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
interval<T, Traits> operator+(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator+(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator+(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> operator-(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator-(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator-(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> operator*(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> operator*(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> operator*(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> operator/(const interval<T, Traits>& x, const interval<T, Traits>& y);
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

template<class T, class Traits> const T& lower(const interval<T, Traits>& x);
template<class T, class Traits> const T& upper(const interval<T, Traits>& x);
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
bool in_zero(const interval<T, Traits>& b);
template<class T, class Traits>
bool subset(const interval<T, Traits>& a, const interval<T, Traits>& b);
template<class T, class Traits>
bool proper_subset(const interval<T, Traits>& a, const interval<T, Traits>& b);
template<class T, class Traits>
bool overlap(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> intersect(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> hull(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> hull(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> hull(const T& x, const interval<T, Traits>& y);
template<class T>
interval<T> hull(const T& x, const T& y);
template<class T, class Traits>
bool singleton(const interval<T, Traits>& x);
template<class T, class Traits>
bool equal(const interval<T, Traits>& x, const interval<T, Traits>& y);

template<class T, class Traits>
std::pair<interval<T, Traits>, interval<T, Traits> > bisect(const interval<T, Traits>& x);

#if 0
template<class T, class Traits>
T dist(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
T dist(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
T dist(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> scale(const interval<T, Traits>& x, const T& mirror, const T& factor);
template<class T, class Traits>
interval<T, Traits> symmetric_scale(const interval<T, Traits>& x, const T& factor);
#endif

template<class T, class Traits>
interval<T, Traits> square(const interval<T, Traits>& x);

template<class T, class Traits>
interval<T, Traits> fmod(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> fmod(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> fmod(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> abs(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> sqrt(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> exp(const interval<T, Traits>& x);
template<class T, class Traits>
interval<T, Traits> log(const interval<T, Traits>& x);

#if 0
template<class T, class Traits>
interval<T, Traits> log10(const interval<T, Traits>& x);
template<class T> T pow(const T& x, long y);
template<class T, class Traits>
interval<T, Traits> pow(const interval<T, Traits>& x, const interval<T, Traits>& y);
#endif

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

#if 0
template<class T, class Traits>
interval<T, Traits> atan2(const interval<T, Traits>& y, const interval<T, Traits>& x);
#endif

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

template<class T, class Traits>
interval<T, Traits> max(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> max(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> max(const T& x, const interval<T, Traits>& y);

template<class T, class Traits>
interval<T, Traits> min(const interval<T, Traits>& x, const interval<T, Traits>& y);
template<class T, class Traits>
interval<T, Traits> min(const interval<T, Traits>& x, const T& y);
template<class T, class Traits>
interval<T, Traits> min(const T& x, const interval<T, Traits>& y);

  namespace interval_lib {

template<class I> I pi();
template<class I> I pi_1_2();
template<class I> I pi_2_1();

  } // namespce interval_lib
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

  BOOST_STATIC_CONSTANT(float_round_style, round_style = round_indeterminate);
  BOOST_STATIC_CONSTANT(bool, is_iec559 = false);

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

#include <boost/interval/rounding.hpp>
#include <boost/interval/rounded_arith.hpp>
#include <boost/interval/rounded_transc.hpp>

#include <boost/interval/checking.hpp>
#include <boost/interval/compare.hpp>
#include <boost/interval/utility.hpp>

#include <boost/interval/oper.hpp>
#include <boost/interval/misc.hpp>
#include <boost/interval/transc.hpp>

namespace boost {
  namespace interval_lib {

template<> struct rounded_math<float>: detail::rm_aux<float> {};
template<> struct rounded_math<double>: detail::rm_aux<double> {};

  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_HPP
