// numeric_limits_handling.hpp

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
// -----------------------------------------------------------------

#ifndef BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP
#define BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP

#include <boost/math/special_functions/fpclassify.hpp>

// TODO use the boost version instead to be more portable?
// Since only double is used, template versions are not needed,
// and TR1 should provide max, min, denorm_min, infinity and isnan,
// but older comilers and libraries may not provide all these.

// using boost::math::fpclassify
// boost::math::
// template <class T>bool isfinite (T);
// template <class T>bool isinf (T);
// template <class T> bool isnan (T);

#include <limits>
  // using std::numeric_limits;
#include <cmath> // Why?


// Allows NaNs to be displayed differently from just too big or too small values.

namespace boost
{
namespace svg
{
namespace detail
{
  // Provide checks on data values to be plotted.
  // Test if at max or +infinity, or min or - infinity, or NaN.

  // Not clear why min or denorm min are not just ignored as almost zero (which is an OK value).

inline bool limit_max(double a)
{ // At max value or _infinity.
    return (a ==(std::numeric_limits<int>::max)() // Avoid macro max trap!
         || a == std::numeric_limits<double>::infinity());
}

inline bool limit_min(double a)
{// At min value, denorm_min or -infinity.  

  return (
    (a == -(std::numeric_limits<int>::max)()) // Avoid macro min trap!
    || (a == -std::numeric_limits<double>::infinity())
    );
    //return (a == (std::numeric_limits<int>::min)() // Avoid macro min trap!
    //    || a == -std::numeric_limits<double>::infinity()
    //    || a == std::numeric_limits<double>::denorm_min()); // Too small to be useful.
}

inline bool limit_NaN(double a)
{ // Separate test for NaNs.
#if defined (BOOST_MSVC)
    return _isnan(a) ? true : false;
  // Ternary operator used to remove warning of casting int to bool.
#else
    return (std::fpclassify(a) == FP_NAN);
#endif
}

inline bool is_limit(double a)
{ // Is at some limit - 
    return limit_max(a) || limit_min(a) || limit_NaN(a);
}

inline bool pair_is_limit(std::pair<double, double> a)
{ // Check on both x and y data points. Return false if either or both are at limit.
  return limit_max(a.first) || limit_min(a.first) || limit_NaN(a.first)
    || limit_max(a.second) || limit_min(a.second) || limit_NaN(a.second);
}

} // namespace detail
} // namespace svg
} // namespace boost


// Defines :
bool boost::svg::detail::limit_max(double); // true if max or +infinity.
bool boost::svg::detail::limit_min(double); // true if min, denorm_min or -infinity.
bool boost::svg::detail::limit_NaN(double); // true if NaN.
bool boost::svg::detail::is_limit(double); // max, min, infinity or NaN - not a 'proper' data value.
bool boost::svg::detail::pair_is_limit(std::pair<double, double>); // x and/or y  not a proper data value.

#endif // BOOST_SVG_NUMERIC_LIMITS_HANDLING_DETAIL_HPP
