/* Boost interval/rounded_transc.hpp template implementation file
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

#ifndef BOOST_INTERVAL_ROUNDED_TRANSC_HPP
#define BOOST_INTERVAL_ROUNDED_TRANSC_HPP

#include <boost/interval/rounding.hpp>
#include <boost/interval/detail/bugs.hpp>
#include <cmath>

namespace boost {
  namespace interval_lib {

template<class T, class Rounding>
struct rounded_transc_exact: Rounding
{
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) { BOOST_INTERVAL_using_math(f); return f(x); } \
    T f##_up  (const T& x) { BOOST_INTERVAL_using_math(f); return f(x); }
  BOOST_INTERVAL_new_func(exp)
  BOOST_INTERVAL_new_func(log)
  BOOST_INTERVAL_new_func(sin)
  BOOST_INTERVAL_new_func(cos)
  BOOST_INTERVAL_new_func(tan)
  BOOST_INTERVAL_new_func(asin)
  BOOST_INTERVAL_new_func(acos)
  BOOST_INTERVAL_new_func(atan)
  BOOST_INTERVAL_new_func(sinh)
  BOOST_INTERVAL_new_func(cosh)
  BOOST_INTERVAL_new_func(tanh)
# undef BOOST_INTERVAL_new_func
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) { BOOST_INTERVAL_using_ahyp(f); return f(x); } \
    T f##_up  (const T& x) { BOOST_INTERVAL_using_ahyp(f); return f(x); }
  BOOST_INTERVAL_new_func(asinh)
  BOOST_INTERVAL_new_func(acosh)
  BOOST_INTERVAL_new_func(atanh)
# undef BOOST_INTERVAL_new_func
};
  
template<class T, class Rounding>
struct rounded_transc_std: Rounding
{
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) \
    { BOOST_INTERVAL_using_math(f); downward(); return force_rounding(f(x)); } \
    T f##_up  (const T& x) \
    { BOOST_INTERVAL_using_math(f); upward();   return force_rounding(f(x)); }
  BOOST_INTERVAL_new_func(exp)
  BOOST_INTERVAL_new_func(log)
  BOOST_INTERVAL_new_func(sin)
  BOOST_INTERVAL_new_func(cos)
  BOOST_INTERVAL_new_func(tan)
  BOOST_INTERVAL_new_func(asin)
  BOOST_INTERVAL_new_func(acos)
  BOOST_INTERVAL_new_func(atan)
  BOOST_INTERVAL_new_func(sinh)
  BOOST_INTERVAL_new_func(cosh)
  BOOST_INTERVAL_new_func(tanh)
# undef BOOST_INTERVAL_new_func
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) \
    { BOOST_INTERVAL_using_ahyp(f); downward(); return force_rounding(f(x)); } \
    T f##_up  (const T& x) \
    { BOOST_INTERVAL_using_ahyp(f); upward();   return force_rounding(f(x)); }
  BOOST_INTERVAL_new_func(asinh)
  BOOST_INTERVAL_new_func(acosh)
  BOOST_INTERVAL_new_func(atanh)
# undef BOOST_INTERVAL_new_func
};

template<class T, class Rounding>
struct rounded_transc_opp: Rounding
{
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) \
    { BOOST_INTERVAL_using_math(f); \
      downward(); T y = force_rounding(f(x)); upward(); return y; } \
    T f##_up  (const T& x) \
    { BOOST_INTERVAL_using_math(f); return force_rounding(f(x)); }
  BOOST_INTERVAL_new_func(exp)
  BOOST_INTERVAL_new_func(log)
  BOOST_INTERVAL_new_func(cos)
  BOOST_INTERVAL_new_func(acos)
  BOOST_INTERVAL_new_func(cosh)
# undef BOOST_INTERVAL_new_func
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) \
    { BOOST_INTERVAL_using_math(f); return -force_rounding(-f(x)); } \
    T f##_up  (const T& x) \
    { BOOST_INTERVAL_using_math(f); return force_rounding(f(x)); }
  BOOST_INTERVAL_new_func(sin)
  BOOST_INTERVAL_new_func(tan)
  BOOST_INTERVAL_new_func(asin)
  BOOST_INTERVAL_new_func(atan)
  BOOST_INTERVAL_new_func(sinh)
  BOOST_INTERVAL_new_func(tanh)
# undef BOOST_INTERVAL_new_func
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) \
    { BOOST_INTERVAL_using_ahyp(f); \
      downward(); T y = force_rounding(f(x)); upward(); return y; } \
    T f##_up  (const T& x) \
    { BOOST_INTERVAL_using_ahyp(f); return force_rounding(f(x)); }
  BOOST_INTERVAL_new_func(asinh)
  BOOST_INTERVAL_new_func(atanh)
# undef BOOST_INTERVAL_new_func
# define BOOST_INTERVAL_new_func(f) \
    T f##_down(const T& x) \
    { BOOST_INTERVAL_using_ahyp(f); return -force_rounding(-f(x)); } \
    T f##_up  (const T& x) \
    { BOOST_INTERVAL_using_ahyp(f); return force_rounding(f(x)); }
  BOOST_INTERVAL_new_func(acosh)
# undef BOOST_INTERVAL_new_func
};
  
  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_ROUNDED_TRANSC_HPP
