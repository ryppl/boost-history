/* boost interval.hpp header file
 *
 * Copyright Jens Maurer 2000
 * Copyright Hervé Brönnimann, Guillaume Melquiond, Sylvain Pion, 2002
 * Permission to use, copy, modify, sell, and distribute this software
 * is hereby granted without fee provided that the above copyright notice
 * appears in all copies and that both that copyright notice and this
 * permission notice appear in supporting documentation,
 *
 * None of the above authors make any representation about the
 * suitability of this software for any purpose. It is provided "as is"
 * without express or implied warranty.
 *
 * $Id$
 */

#ifndef BOOST_INTERVAL_HPP
#define BOOST_INTERVAL_HPP

#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <cmath>
#include <algorithm>
#include <boost/interval/detail/bugs.hpp>


namespace boost {

  namespace interval_lib {

    template<class T> struct compare_certainly;
    template<class T> struct rounded_math;
    template<class T> struct checking_strict;

  } // namespace interval_lib

  /*
   * default traits class
   */

  template<class T,
           class Compare  = interval_lib::compare_certainly<T>,
           class Rounding = interval_lib::rounded_math<T>,
           class Checking = interval_lib::checking_strict<T> >
  struct interval_traits
  {
    typedef T base_type;
    typedef Compare compare;
    typedef Rounding rounding;
    typedef Checking checking;
  };

  /*
   * interval class
   */

  template<class T, class Traits = interval_traits<T> >
  class interval
  {
  public:
    typedef T base_type;
    typedef Traits traits_type;

    interval(const T& v = T());
    interval(const T& l, const T& u);

    template<class Traits2>
      interval(const interval<T, Traits2>& r);

    // compiler-generated copy constructor and assignment operator are fine

    interval& operator=(const T& x);
    void assign(const T& l, const T& u);

    static interval empty();
    static interval whole();
    static interval hull(const T& x, const T& y);

    const T& lower() const;
    const T& upper() const;

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
    void set_whole();
    void set(const T& l, const T& u);

  private:
    typedef typename Traits::compare  compare;
    typedef typename Traits::checking checking;
    typedef typename Traits::rounding rounding;
    T low;
    T up;
  };


  /*
   * Non-Member Function Declarations
   */

  template<class T, class Traits>
  const interval<T, Traits>& operator+(const interval<T, Traits>& x);

  template<class T, class Traits>
  interval<T, Traits> operator-(const interval<T, Traits>& x);

  // Declares all three binary operators:
  //   interval<T,Tr> op (interval<T,Tr> const&, interval<T,Tr> const&);
  //   interval<T,Tr> op (interval<T,Tr> const&, T const&);
  //   interval<T,Tr> op (T const&, interval<T,Tr> const&);
  #define BOOST_INTERVAL_DEFINE_OPERATOR_2( name ) \
    template<class T, class Traits> \
      interval<T,Traits> \
      name (const interval<T,Traits>& x, const interval<T,Traits>& y); \
    template<class T, class Traits> \
      interval<T,Traits> \
      name (const interval<T,Traits>& x, const T& y); \
    template<class T, class Traits> \
      interval<T,Traits> \
      name (const T& x, const interval<T,Traits>& y);

  BOOST_INTERVAL_DEFINE_OPERATOR_2( operator+ )
  BOOST_INTERVAL_DEFINE_OPERATOR_2( operator- )
  BOOST_INTERVAL_DEFINE_OPERATOR_2( operator* )
  BOOST_INTERVAL_DEFINE_OPERATOR_2( operator/ )

  #undef BOOST_INTERVAL_DEFINE_OPERATOR_2

  // Declares all three comparison functions:
  //   bool comp  (interval<T,Tr> const&, interval<T,Tr> const&);
  //   bool comp  (interval<T,Tr> const&, T const&);
  //   bool comp  (T const&, interval<T,Tr> const&);
  #define BOOST_INTERVAL_DEFINE_COMPARISON( name ) \
    template<class T, class Traits> \
      bool name (const interval<T,Traits>& x, const interval<T,Traits>& y); \
    template<class T, class Traits> \
      bool name (const interval<T,Traits>& x, const T& y); \
    template<class T, class Traits> \
      bool name (const T& x, const interval<T,Traits>& y);

  BOOST_INTERVAL_DEFINE_COMPARISON( cerlt )
  BOOST_INTERVAL_DEFINE_COMPARISON( cerle )
  BOOST_INTERVAL_DEFINE_COMPARISON( cergt )
  BOOST_INTERVAL_DEFINE_COMPARISON( cerge )
  BOOST_INTERVAL_DEFINE_COMPARISON( cereq )
  BOOST_INTERVAL_DEFINE_COMPARISON( cerne )

  BOOST_INTERVAL_DEFINE_COMPARISON( poslt )
  BOOST_INTERVAL_DEFINE_COMPARISON( posle )
  BOOST_INTERVAL_DEFINE_COMPARISON( posgt )
  BOOST_INTERVAL_DEFINE_COMPARISON( posge )
  BOOST_INTERVAL_DEFINE_COMPARISON( poseq )
  BOOST_INTERVAL_DEFINE_COMPARISON( posne )

  BOOST_INTERVAL_DEFINE_COMPARISON( operator<  )
  BOOST_INTERVAL_DEFINE_COMPARISON( operator<= )
  BOOST_INTERVAL_DEFINE_COMPARISON( operator>  )
  BOOST_INTERVAL_DEFINE_COMPARISON( operator>= )
  BOOST_INTERVAL_DEFINE_COMPARISON( operator== )
  BOOST_INTERVAL_DEFINE_COMPARISON( operator!= )

  #undef BOOST_INTERVAL_DEFINE_COMPARISON

  /*
   * Interval bounds-related functions
   */

  template<class T, class Traits>
    const T& lower(const interval<T,Traits>& x);

  template<class T, class Traits>
    const T& upper(const interval<T,Traits>& x);

  template<class T, class Traits>
    T width(const interval<T,Traits>& x);

  template<class T, class Traits>
    T median(const interval<T,Traits>& x);

#if 0
  template<class T, class Traits>
    interval<T,Traits> pred(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> succ(const interval<T,Traits>& x);
#endif


  /*
   * Interval Boolean predicates
   */

  template<class T, class Traits>
    bool empty(const interval<T,Traits>& x);

  template<class T, class Traits>
    bool singleton(const interval<T,Traits>& x);

  template<class T, class Traits>
    bool equal(const interval<T,Traits>& x, const interval<T,Traits>& y);

  template<class T, class Traits>
    bool in(const interval<T,Traits>& x);

  template<class T, class Traits>
    bool in_zero(const interval<T,Traits>& x);

  template<class T, class Traits>
    bool subset(const interval<T,Traits>& a, const interval<T,Traits>& b);

  template<class T, class Traits>
    bool
    proper_subset(const interval<T,Traits>& a, const interval<T,Traits>& b);

  template<class T, class Traits>
    bool overlap(const interval<T,Traits>& x, const interval<T,Traits>& y);


  /*
   * Interval set manipulations
   */

  template<class T, class Traits>
    interval<T,Traits>
    intersection(const interval<T,Traits>& x, const interval<T,Traits>& y);

  template<class T, class Traits>
    interval<T,Traits>
    hull(const interval<T,Traits>& x, const interval<T,Traits>& y);

  template<class T, class Traits>
    interval<T,Traits>
    hull(const interval<T,Traits>& x, const T& y);

  template<class T, class Traits>
    interval<T,Traits>
    hull(const T& x, const interval<T,Traits>& y);

  template<class T>
    interval<T>
    hull(const T& x, const T& y);

  template<class T, class Traits>
    std::pair<interval<T,Traits>, interval<T,Traits> >
    bisect(const interval<T,Traits>& x);


  /*
   * Interval set manipulations
   */

#if 0
  template<class T, class Traits>
    interval<T,Traits>
    scale(const interval<T,Traits>& x, const T& mirror, const T& factor);
  template<class T, class Traits>
    interval<T,Traits>
    symmetric_scale(const interval<T,Traits>& x, const T& factor);
#endif

  /*
   * Min, max, abs, square
   */

  template<class T, class Traits>
    interval<T,Traits>
    min(const interval<T,Traits>& x, const interval<T,Traits>& y);
  template<class T, class Traits>
    interval<T,Traits>
    min(const interval<T,Traits>& x, const T& y);
  template<class T, class Traits>
    interval<T,Traits>
    min(const T& x, const interval<T,Traits>& y);

  template<class T, class Traits>
    interval<T,Traits>
    max(const interval<T,Traits>& x, const interval<T,Traits>& y);
  template<class T, class Traits>
    interval<T,Traits>
    max(const interval<T,Traits>& x, const T& y);
  template<class T, class Traits>
    interval<T,Traits>
    max(const T& x, const interval<T,Traits>& y);

  template<class T, class Traits>
    interval<T,Traits>
    abs(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits>
    square(const interval<T,Traits>& x);

  /*
   * Fmod
   */

  template<class T, class Traits>
    interval<T,Traits>
    fmod(const interval<T,Traits>& x, const interval<T,Traits>& y);
  template<class T, class Traits>
    interval<T,Traits>
    fmod(const interval<T,Traits>& x, const T& y);
  template<class T, class Traits>
    interval<T,Traits>
    fmod(const T& x, const interval<T,Traits>& y);

  template<class T, class Traits>
    interval<T,Traits> sqrt(const interval<T,Traits>& x);

  /*
   * Transcendental functions: exp, log
   */

  template<class T, class Traits>
    interval<T,Traits> exp(const interval<T,Traits>& x);
  template<class T, class Traits>
    interval<T,Traits> log(const interval<T,Traits>& x);
#if 0
  template<class T, class Traits>
  interval<T,Traits> log10(const interval<T,Traits>& x);
#endif
  template<class T, class Traits>
  interval<T,Traits> pow(const interval<T,Traits>& x, int y);

  /*
   * Trigonometric functions: sin, cos, tan, asin, acos, atan, atan2
   */

  template<class T, class Traits>
    interval<T,Traits> sin(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> cos(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> tan(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> asin(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> acos(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> atan(const interval<T,Traits>& x);

#if 0
  template<class T, class Traits>
  interval<T,Traits>
  atan2(const interval<T,Traits>& y, const interval<T,Traits>& x);
#endif

  /*
   * Hyperbolic trigonometric functions: sinh, cosh, tanh, asinh, acosh, atanh
   */

  template<class T, class Traits>
    interval<T,Traits> sinh(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> cosh(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> tanh(const interval<T,Traits>& x);

#ifdef BOOST_HAVE_INV_HYPERBOLIC
  template<class T, class Traits>
    interval<T,Traits> asinh(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> acosh(const interval<T,Traits>& x);

  template<class T, class Traits>
    interval<T,Traits> atanh(const interval<T,Traits>& x);
#endif

  /*
   * Interval trigonometric constants: pi, pi/2, 2*pi
   */

  namespace interval_lib {

    template<class I> I pi();
    template<class I> I pi_1_2();
    template<class I> I pi_2_1();

  } // namespace interval_lib

} // namespace boost


#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace std {

  template<class T, class Traits>
  class numeric_limits<boost::interval<T,Traits> >
    : public std::numeric_limits<T>
  {
  private:
    typedef boost::interval<T,Traits> vt;
    typedef numeric_limits<T> bl;
  public:
    static vt min() throw() { return vt(bl::min(), bl::min()); }
    static vt max() throw() { return vt(bl::max(), bl::max()); }
    static vt epsilon() throw() { return vt(bl::epsilon(), bl::epsilon()); }

    BOOST_STATIC_CONSTANT(float_round_style, round_style = round_indeterminate);
    BOOST_STATIC_CONSTANT(bool, is_iec559 = false);

    static vt infinity() throw() { return vt::whole(); }
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
  struct less<boost::interval<T,Traits> >
  {
    bool operator()(const boost::interval<T,Traits> & x,
                    const boost::interval<T,Traits> & y)
    {
      return lower(x) < lower(y)
          || (lower(x) == lower(y) && upper(x) < upper(y));
    }
  };
#endif

} // namespace std

#endif

#include <boost/interval/rounding.hpp>
#include <boost/interval/rounded_arith.hpp>
#include <boost/interval/rounded_transc.hpp>
#include <boost/interval/constants.hpp>

#include <boost/interval/checking.hpp>
#include <boost/interval/compare.hpp>
#include <boost/interval/utility.hpp>

#include <boost/interval/oper.hpp>
#include <boost/interval/transc.hpp>

namespace boost {

  namespace interval_lib {

  /*
   * Three specializations of rounded_math<T>
   */

  template<>
  struct rounded_math<float>
    : save_state<rounded_transc_dummy<float, rounded_arith_opp<float> > >
  {};

  template<>
  struct rounded_math<double>
    : save_state<rounded_transc_dummy<double, rounded_arith_opp<double> > >
  {};

  template<>
  struct rounded_math<long double>
    : save_state<rounded_transc_dummy<long double, rounded_arith_opp<long double> > >
  {};

  } // namespace interval_lib

} // namespace boost

#endif // BOOST_INTERVAL_HPP
