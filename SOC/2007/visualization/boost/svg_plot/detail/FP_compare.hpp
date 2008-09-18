//  Copyright Paul A. Bristow 2008

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  Derived from Boost.Test Copyright Gennadiy Rozental 2001-2007.
//  See http://www.boost.org/libs/test for the library home page.
//  Deliberately removed any treatment of percent!

#ifndef BOOST_FLOATING_POINT_COMPARISON_HPP
#define BOOST_FLOATING_POINT_COMPARISON_HPP

#include <boost/limits.hpp>  // for std::numeric_limits
#include <boost/math/tools/precision.hpp> // for max_value, min_value & epsilon for floating_point type;

// Check two floating-point values are close within a chosen tolerance.
template<typename FPT> class close_to;

// Check floating-point value is smaller than a chosen small value.
template<typename FPT> class smallest;

enum floating_point_comparison_type
{ // Two types of comparison.
  FPC_STRONG, // "Very close"   - Knuth equation 1'  the default.
  FPC_WEAK    // "Close enough" - equation 2'.
  // equations in Dougles E. Knuth, Seminumerical algorithms (3rd Ed) section 4.2.4, Vol II,
  // pp 213-225, Addison-Wesley, 1997, ISBN: 0201896842.
  // Strong requires closeness relative to BOTH values begin compared,
  // Weak only requires only closeness to EITHER ONE value.
};

// GNU int gsl_fcmp (double x, double y, double epsilon) provides similar function.
// fcmp also provides a C implementation at https://sourceforge.net/projects/fcmp/
// For IEEE floating-point types, some speedups are possible, for example see:
// Taming the Floating-point Beast, Chris Lomont
// www.lomont.org/Math/Papers/2005/CompareFloat.pdf
// Alberto Squassabia, Comparing Floats:
// How to determine if Floating-point quantities are close enough
// once a tolerance has been reached:  C++ report March 2000.
// Gennadiy Rozental, Floating_point comparison algorithms,
// www.boost.org/libs/test/doc/components/test_tools/floating_point_comparison.html
// Comparison of Floating Point Numbers, Matthias Ruppwww.mrupp.info/Data/2007floatingcomp.pdf, July 2007.
// The pitfalls of verifying floating-point computations, David Monniaux
// CNRS Ecole normale sup´erieure, 1 Feb 2008, http://arxiv.org/abs/cs/0701192v4
// submitted to ACM TOPLAS.

// FPT is Floating-Point Type: float, double, long double, or User-Defined like NTL quad_float or RR.
// from boost/math/tools/precision.hpp
template <class T> T max_value(T);
template <class T> T min_value(T);
template <class T> T epsilon(T);

template<typename FPT> FPT
fpt_abs(FPT arg)
{ // abs function (just in case abs is not defined for FPT).
  return arg <static_cast<FPT>(0) ? -arg : arg;
}

template<typename FPT> FPT
safe_fpt_division(FPT f1, FPT f2)
{ // Safe from under and overflow.
  // Both f1 and f2 must be unsigned here.

  if( (f2 < static_cast<FPT>(1))  && (f1 > f2 * boost::math::tools::max_value<FPT>()) )
  { // Avoid overflow.
    return boost::math::tools::max_value<FPT>();
  }

  if( (f1 == static_cast<FPT>(0))
    || ((f2 > static_cast<FPT>(1)) && (f1 < f2 * boost::math::tools::min_value<FPT>()) )
    )
  {  // Avoid underflow.
    return static_cast<FPT>(0);
  }
  return f1 / f2;
} // safe_fpt_division(FPT f1, FPT f2)

// Check two floating-point values are close within a chosen tolerance.

template<typename FPT = double>
class close_to
{
public:

  // One constructor for fraction tolerance only.
  template<typename FPT>
  explicit close_to(FPT tolerance,
    floating_point_comparison_type fpc_type = FPC_STRONG)
  :
    fraction_tolerance_(tolerance),
      strong_or_weak_(fpc_type)
  { // Fraction.
    // Check that tolerance isn't negative - which doesn't make sense,
    // and can be assumed to be a programmer error?
    BOOST_ASSERT(tolerance >= static_cast<FPT>(0));
  }

  //template<typename FPT>
  close_to()
  :
  fraction_tolerance_(2 * boost::math::tools::epsilon<FPT>()),
    strong_or_weak_(FPC_STRONG)
  { // Default is two epsilon.
  }

  bool operator()(FPT left, FPT right) const
  {
    FPT diff = fpt_abs(left - right);
    FPT d1   = safe_fpt_division(diff, fpt_abs(right));
    FPT d2   = safe_fpt_division(diff, fpt_abs(left));

    return strong_or_weak_
      ? ((d1 <= fraction_tolerance_) && (d2 <= fraction_tolerance_)) // Strong.
      : ((d1 <= fraction_tolerance_) || (d2 <= fraction_tolerance_)); // Weak.
  }

  FPT size()
  { // Get function.
    return fraction_tolerance_;
  }

  floating_point_comparison_type strength()
  { // Get function.
    return strong_or_weak_;
  }

private:
    FPT fraction_tolerance_;
    floating_point_comparison_type strong_or_weak_;

}; // class close_to

// Check floating-point value is smaller than a chosen small value.

// David Monniaux, http://arxiv.org/abs/cs/0701192v4,
// It is somewhat common for beginners to add a comparison check to 0 before
// computing a division, in order to avoid possible division-by-zero exceptions or
// the generation of infinite results. A first objection to this practise is that, anyway,
// computing 1/x for x very close to zero will generate very large numbers
// that will most probably result in overflows later.
// Another objection, which few programmers know about and that we wish to draw attention
// to, is that it may actually fail to work, depending on what the compiler
// does — that is, the program may actually test that x 6= 0, then, further down,
// find that x = 0 without any apparent change to x!

template<typename FPT = double>
class smallest
{
public:
  template<typename FPT>
  explicit smallest(FPT s)
  :
  smallest_(s)
  { // Constructor.
  }

  smallest()
  :
  smallest_(2 * boost::math::tools::min_value<FPT>())
  { // Default Constructor.
    // smallest_ =  2. * boost::math::tools::min_value<double>();
    // multiplier m = 2 (must be integer or static_cast<FPT>())
    // is chosen to allow for a few bits of computation error.
    // Pessimistic multiplier is the number of arithmetic operations,
    // assuming every operation causes a 1 least significant bit error,
    // but a more realistic average might be half this.
  }

  template<typename FPT>
  bool operator()(FPT fp_value, FPT s)
  {
    if (fpt_abs(fp_value) == static_cast<FPT>(0))
    { // Test for zero first in case FPT is actually an integer type zero,
      // when the comparison < below would fail because
      // smallest_ could become zero when min_value converts to integer.
      return true;
    }
    return fpt_abs(fp_value) < fpt_abs(s);
  } // bool operator()

  template<typename FPT>
  bool operator()(FPT fp_value)
  {

    if (fpt_abs(fp_value) == static_cast<FPT>(0))
    { // Test for zero first in case FPT is actually an integer type,
      // when the comparison < below would fail because
      // smallest could become zero.
      return true;
    }
    return fpt_abs(fp_value) < fpt_abs(smallest_);
  } // bool operator()

  FPT size()
  { // Get function.
    return smallest_;
  }

private:
  // Smallest value that will be counted as effectively zero.
  FPT smallest_;

}; // class smallest

// Define two convenient typedefs.

// Since double and the default smallest value 2 * min_value = 4.45015e-308
// is a very common requirement, provide an convenience alias for this:
typedef smallest<double> tiny; // Allow tiny as a shorthand for 1e-308

// Since double and the default smallest value 2 * min_value = 4.45015e-308
// is a very common requirement, provide an convenience alias for this:
typedef close_to<double> neareq; // Allow tiny as a shorthand for epsilon

#endif // BOOST_FLOATING_POINT_COMPARISON_HPP
