/* interval implementation selection
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

#ifndef INTERVAL_IMPLEMENTATION_SELECT_HPP
#define INTERVAL_IMPLEMENTATION_SELECT_HPP

/*
 * This file provides:
 *  - typedef interval_type
 *  - static const std::string interval_name, a short string describing
 *    the implementation
 *  - interval math functions visible in the global namespace
 *  - optional loop unrolling instructions (#define UNROLL)
 */

#include <string>

#define HAVE_TRANSCENDENTAL
#define HAVE_FMOD
#define HAVE_SINCOS
#define HAVE_TAN
#define HAVE_ASINCOS
#define HAVE_ATAN2
#define HAVE_HYPERBOLIC
#define HAVE_CLOSED_SEMANTICS

#if defined(USE_BOOST)

#include <boost/interval.hpp>
using namespace boost;
using namespace interval_lib;

typedef
  save_state
    <rounded_transc_opposite_trick
       <double, rounded_arithmetic_opposite_trick
	          <double, rounding_control<double> > > >
  my_rounded_arith;

typedef
  boost::interval<double, interval_traits<double,
					  compare_certainly<double>,
					  my_rounded_arith,
					  checking_lax<double> > >
  interval_type;
//typedef boost::interval<double, boost::interval_traits<double> > interval_type;
static const std::string interval_name = "boost::interval<double>";
using namespace boost;     // not generally recommended
#define UNROLL(x) UNROLL4(x)

#elif defined(USE_BOOST_INDET)

// In order to show the performance benefits of avoiding rounding mode
// switches, this ugly hack avoids restoring the mode after each operation.
// It violates the interface specification for boost::interval<> !!!

namespace boost {
namespace detail {
  template<class T> struct global_rounding_control { };
} // namespace detail
} // namespace boost
// platform-specific
#define BOOST_INTERVAL_HPP
#define rounding_control global_rounding_control
//#include <boost/detail/isoc99_rounding_control.hpp>
#include <boost/detail/x86gcc_rounding_control.hpp>
#undef rounding_control
#undef BOOST_INTERVAL_HPP
#include <boost/interval.hpp>

namespace boost {
namespace detail {
  static global_rounding_control<double> singleton_global_rounding_control;
  template<>
  struct rounding_control<double>
  {
    void upward() { singleton_global_rounding_control.upward(); }
    void downward() { singleton_global_rounding_control.downward(); }
    void tonearest() { singleton_global_rounding_control.tonearest(); }
    void towardzero() { singleton_global_rounding_control.towardzero(); }
  };
} // namespace detail
} // namespace boost

typedef boost::interval<double, boost::interval_traits<double> > interval_type;
static const std::string interval_name = "boost::interval<double,indet>";
using namespace boost;        // not generally recommended
#define UNROLL(x) UNROLL4(x)

#elif defined(USE_RVINTERVAL)

#include "RVInterv/interval.h"
typedef interval interval_type;
static const std::string interval_name = "RVInterval";
inline interval_type abs(const interval_type & a) { return fabs(a); }
#undef HAVE_FMOD
#undef HAVE_TAN
#undef HAVE_ATAN2
#undef HAVE_HYPERBOLIC
#undef HAVE_CLOSED_SEMANTICS

#elif defined(USE_JEREMY)

#include "interval-1.0/interval.h"
typedef interval<double> interval_type;
static const std::string interval_name = "Jeremy_Siek";
#undef HAVE_FMOD
#undef HAVE_SINCOS     // restricted to [0..2pi]
#undef HAVE_ASINCOS    // prints error outside of [-1,1]
#undef HAVE_ATAN2
#undef HAVE_HYPERBOLIC

#elif defined(USE_PROFIL_BIAS)

#include "Profil/Interval.h"
#undef min
#undef max
typedef INTERVAL interval_type;
static const std::string interval_name = "Profil/BIAS";
inline interval_type abs(const interval_type & a)
{ return Abs(const_cast<INTERVAL &>(a)); }
#undef HAVE_FMOD
#undef HAVE_TRANSCENDENTAL

#elif defined(USE_POINT)

typedef double interval_type;
static const std::string interval_name = "double";
#define UNROLL(x) UNROLL4(x)

#else
#error Please adjust for your interval package and #define USE_XXXX.
#endif

#ifndef UNROLL
#define UNROLL(x) x
#endif

#endif /* INTERVAL_IMPLEMENTATION_SELECT_HPP */
