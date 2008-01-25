/* Boost interval/rounded_arith.hpp template implementation file
 *
 * Copyright 2002-2003 Hervé Brönnimann, Guillaume Melquiond, Sylvain Pion
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_NUMERIC_INTERVAL_ROUNDED_ARITH_HPP
#define BOOST_NUMERIC_INTERVAL_ROUNDED_ARITH_HPP

#include <boost/numeric/interval/rounding.hpp>
#include <boost/numeric/interval/detail/bugs.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/identity.hpp>
#include <cmath>

namespace boost {
namespace numeric {
namespace interval_lib {

/*
 * Three classes of rounding: exact, std, opp
 * See documentation for details.
 */

template<class T, class Rounding>
struct rounded_arith_exact: Rounding {
  void init() { }
  template<class U> T conv_down(U const &v) { return static_cast<T>(v); }
  template<class U> T conv_up  (U const &v) { return static_cast<T>(v); }
  T add_down (const T& x, const T& y) { return x + y; }
  T add_up   (const T& x, const T& y) { return x + y; }
  T sub_down (const T& x, const T& y) { return x - y; }
  T sub_up   (const T& x, const T& y) { return x - y; }
  T mul_down (const T& x, const T& y) { return x * y; }
  T mul_up   (const T& x, const T& y) { return x * y; }
  T div_down (const T& x, const T& y) { return x / y; }
  T div_up   (const T& x, const T& y) { return x / y; }
  T median   (const T& x, const T& y) { return (x + y) / 2; }
  T sqrt_down(const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(sqrt); return sqrt(x); }
  T sqrt_up  (const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(sqrt); return sqrt(x); }
  T int_down (const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(floor); return floor(x); }
  T int_up   (const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(ceil); return ceil(x); }
};

template<class T, class Rounding>
struct rounded_arith_std: Rounding {
# define BOOST_DN(EXPR) this->downward();   return this->force_rounding(EXPR)
# define BOOST_NR(EXPR) this->to_nearest(); return this->force_rounding(EXPR)
# define BOOST_UP(EXPR) this->upward();     return this->force_rounding(EXPR)
  void init() { }
  template<class U> T conv_down(U const &v) { BOOST_DN(static_cast<T>(v)); }
  template<class U> T conv_up  (U const &v) { BOOST_UP(static_cast<T>(v)); }
  T add_down(const T& x, const T& y) { BOOST_DN(x + y); }
  T sub_down(const T& x, const T& y) { BOOST_DN(x - y); }
  T mul_down(const T& x, const T& y) { BOOST_DN(x * y); }
  T div_down(const T& x, const T& y) { BOOST_DN(x / y); }
  T add_up  (const T& x, const T& y) { BOOST_UP(x + y); }
  T sub_up  (const T& x, const T& y) { BOOST_UP(x - y); }
  T mul_up  (const T& x, const T& y) { BOOST_UP(x * y); }
  T div_up  (const T& x, const T& y) { BOOST_UP(x / y); }
  T median(const T& x, const T& y) { BOOST_NR((x + y) / 2); }
  T sqrt_down(const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(sqrt); BOOST_DN(sqrt(x)); }
  T sqrt_up  (const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(sqrt); BOOST_UP(sqrt(x)); }
  T int_down(const T& x) { this->downward(); return to_int(x); }
  T int_up  (const T& x) { this->upward();   return to_int(x); }
# undef BOOST_DN
# undef BOOST_NR
# undef BOOST_UP
};
  
template<class T, class Rounding>
struct rounded_arith_opp: Rounding {
  void init() { this->upward(); }
# define BOOST_DN(EXPR) \
    this->downward(); \
    T r = this->force_rounding(EXPR); \
    this->upward(); \
    return r
# define BOOST_NR(EXPR) \
    this->to_nearest(); \
    T r = this->force_rounding(EXPR); \
    this->upward(); \
    return r
# define BOOST_UP(EXPR) return this->force_rounding(EXPR)
# define BOOST_UP_NEG(EXPR) return -this->force_rounding(EXPR)
private:
   // Downward conversion has three cases:
   template<class U> T conv_down(U const &v, const mpl::true_&, const mpl::true_&)
   {
      // U is an integer as wide as boost::intmax_t:
      if((v > static_cast<U>(std::numeric_limits<boost::intmax_t>::max()))
         || (v < static_cast<U>(std::numeric_limits<boost::intmax_t>::min())))
      {
         BOOST_DN(static_cast<T>(v));
      }
      BOOST_UP_NEG(static_cast<T>(-static_cast<boost::intmax_t>(v))); 
   }
   template<class U> T conv_down(U const &v, const mpl::true_&, const mpl::false_&)
   {
      // U is an integer smaller than boost::intmax_t:
      BOOST_UP_NEG(static_cast<T>(-static_cast<boost::intmax_t>(v))); 
   }
   template<class U> T conv_down(U const &v, const mpl::false_&, const mpl::false_&)
   {
      // U is not an integer, so it had better be a signed type(!):
      BOOST_UP_NEG(-v);
   }
public:
  template<class U> T conv_down(U const &v) 
  {
     typedef typename boost::is_integral<U>::type t1;
     typedef typename mpl::and_<t1, boost::is_unsigned<U>, mpl::bool_<sizeof(U) == sizeof(boost::intmax_t)> >::type t2;
     return conv_down(v, t1(), t2());
  }
  template<class U> T conv_up  (U const &v) { BOOST_UP(static_cast<T>(v)); }
  T add_down(const T& x, const T& y) { BOOST_UP_NEG((-x) - y); }
  T sub_down(const T& x, const T& y) { BOOST_UP_NEG(y - x); }
  T mul_down(const T& x, const T& y) { BOOST_UP_NEG(x * (-y)); }
  T div_down(const T& x, const T& y) { BOOST_UP_NEG(x / (-y)); }
  T add_up  (const T& x, const T& y) { BOOST_UP(x + y); }
  T sub_up  (const T& x, const T& y) { BOOST_UP(x - y); }
  T mul_up  (const T& x, const T& y) { BOOST_UP(x * y); }
  T div_up  (const T& x, const T& y) { BOOST_UP(x / y); }
  T median  (const T& x, const T& y) { BOOST_NR((x + y) / 2); }
  T sqrt_down(const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(sqrt); BOOST_DN(sqrt(x)); }
  T sqrt_up  (const T& x)
  { BOOST_NUMERIC_INTERVAL_using_math(sqrt); BOOST_UP(sqrt(x)); }
  T int_down(const T& x) { return -to_int(-x); }
  T int_up  (const T& x) { return to_int(x); }
# undef BOOST_DN
# undef BOOST_NR
# undef BOOST_UP
# undef BOOST_UP_NEG
};

} // namespace interval_lib
} // namespace numeric
} // namespace boost

#endif // BOOST_NUMERIC_INTERVAL_ROUNDED_ARITH_HPP
