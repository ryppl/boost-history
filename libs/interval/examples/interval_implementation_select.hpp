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

struct my_checking: checking_base<double> {
  static bool is_nan(const double&) { return false; }
  static bool is_empty(const double&, const double&) { return false; }
};

typedef
  interval<double, interval_traits<double,
				   compare_certainly<double>,
				   save_state<rounded_transc_opp<double> >,
				   my_checking> >
  interval_type;

static const std::string interval_name = "boost::interval<double>";

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
