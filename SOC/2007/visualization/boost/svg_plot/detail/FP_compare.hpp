/*! \file FP_compare.hpp
    \brief Two types of floating-point comparison "Very close" and "Close enough" to a chosen tolerance.
    \details
      Derived from Boost.Test Copyright Gennadiy Rozental 2001-2007.
      See http://www.boost.org/libs/test for the Boost.Test library home page.
      Deliberately removed any treatment of percent to avoid further potential confusion!
    \date Mar 2009
    \author Paul A. Bristow
*/

//  Copyright Paul A. Bristow 2008, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FLOATING_POINT_COMPARISON_HPP
#define BOOST_FLOATING_POINT_COMPARISON_HPP

#include <boost/limits.hpp>  // using std::numeric_limits
#include <boost/math/tools/precision.hpp> // using max_value, min_value & epsilon for floating_point type.

//namespace boost
//{
//  namespace math
//  {

// Check two floating-point values are close within a chosen tolerance.
template<typename FPT> class close_to; // Default = double.

// Check floating-point value is smaller than a chosen small value.
template<typename FPT> class smallest; // Default = double.

enum floating_point_comparison_type
{ /*! \enum floating_point_comparison_type
   \brief Two types of floating-point comparison "Very close" and "Close enough".
   \details equations in Douglas E. Knuth, Seminumerical algorithms (3rd Ed) section 4.2.4, Vol II,
    pp 213-225, Addison-Wesley, 1997, ISBN: 0201896842.\n
    Strong requires closeness relative to \b both values being compared,
    Weak only requires only closeness relative to \b either \b one value.
  */
  FPC_STRONG, //!< "Very close"   - Knuth equation 1 (the default).
  FPC_WEAK    //!< "Close enough" - Knuth equation 2.
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
// CNRS Ecole normale superieure, 1 Feb 2008, http://arxiv.org/abs/cs/0701192v4
// submitted to ACM TOPLAS.

// \tparam FPT is Floating-Point Type: float, double, long double, or User-Defined like NTL quad_float or RR.
// from boost/math/tools/precision.hpp
template <class FPT> FPT max_value(FPT); //!< maximum value for floating-point type T.
template <class FPT> FPT min_value(FPT); //!< minimum value for floating-point type T.
template <class FPT> FPT epsilon(FPT); //!< epsilon for type T (about 1e-16 for double)

template<typename FPT> FPT
fpt_abs(FPT arg)
{ //! abs function (just in case abs is not defined for a user-defined FPT).
  return arg <static_cast<FPT>(0) ? -arg : arg;
}

template<typename FPT> FPT
safe_fpt_division(FPT f1, FPT f2)
{ //! Division safe from underflow and overflow. (Both f1 and f2 must be unsigned here).
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

template<typename FPT = double> //! \tparam FPT floating-point type.
class close_to
{ /*!
   \class close_to
   \brief Test if two floating-point values are close within a chosen tolerance.
   \details Close can be one of two types of floating-point comparison "Very close" and "Close enough".
    equations in Dougles E. Knuth, Seminumerical algorithms (3rd Ed) section 4.2.4, Vol II,
    pp 213-225, Addison-Wesley, 1997, ISBN: 0201896842.
    Strong requires closeness relative to BOTH values begin compared,
    Weak only requires only closeness to EITHER ONE value.

  */
public:
  //! Constructor for close_to from tolerance and strength. (By design, percent is NOT implemented).
  template<typename FPT>
  explicit close_to(FPT tolerance, //!< Fractional tolerance.
    floating_point_comparison_type fpc_type = FPC_STRONG) //!< strong requires closeness relative to both values.
  :
    fraction_tolerance_(tolerance),
      strong_or_weak_(fpc_type)
  { // Fraction.
    // Check that tolerance isn't negative - which doesn't make sense,
    // and can be assumed to be a programmer error?
    BOOST_ASSERT(tolerance >= static_cast<FPT>(0));
  }

  close_to()
  :
  fraction_tolerance_(2 * boost::math::tools::epsilon<FPT>()),
    strong_or_weak_(FPC_STRONG)
  { /*! Default is two epsilon for the FPT.\n
        Note that some user-defined floating-point types may not specialize std::numeric_limits<FPT>::epsilon()
        so it is convenient to use boost::math::tools::epsilon<FPT>(); instead.
     */

  }

  bool operator()(FPT left, FPT right) const
  { //! \brief Test if two floating-point values are close within a chosen tolerance.
    //! \details Tolerance can be interpreted as Knuth's  "Very close" (equation 1), the default, or "Close enough" (equation 2).

    FPT diff = fpt_abs(left - right);
    FPT d1   = safe_fpt_division(diff, fpt_abs(right));
    FPT d2   = safe_fpt_division(diff, fpt_abs(left));

    return strong_or_weak_
      ? ((d1 <= fraction_tolerance_) && (d2 <= fraction_tolerance_)) // Strong.
      : ((d1 <= fraction_tolerance_) || (d2 <= fraction_tolerance_)); // Weak.
  }

  FPT size()
  { //! \return the chosen tolerance, as a \b fraction (not a percentage).
    return fraction_tolerance_;
  }

  floating_point_comparison_type strength()
  { //! \return strength of comparison, Knuth's  "Very close" (equation 1), the default, or "Close enough" (equation 2).
    return strong_or_weak_;
  }

private:
    FPT fraction_tolerance_; //!< tolerance as a fraction.
    floating_point_comparison_type strong_or_weak_;  //!< Knuth's  "Very close" (equation 1), the default, or "Close enough" (equation 2).

}; // class close_to

template<typename FPT = double> //! \tparam FPT floating-point type.
class smallest
{ /*! \class smallest
      \brief Check floating-point value is smaller than a chosen small value,
         default is twice min_value() for the floating-point type FPT.
      \details
       It is somewhat common for beginners to add a comparison check to 0 before
       computing a division, in order to avoid possible division-by-zero exceptions or
       the generation of infinite results.\n
       A first objection to this practise is that, anyway,
       computing 1/x for x very close to zero will generate very large numbers
       that will most probably result in overflows later.\n
       Another objection, which few programmers know about and that we wish to draw attention
       to, is that it may actually fail to work, depending on what the compiler
       does, that is, the program may actually test that x == 0, then, further down,
       find that x = 0 without any apparent change to x!\n
       David Monniaux, http://arxiv.org/abs/cs/0701192v4.
       \tparam FPT A floating-point type, float, double, long double or user-defined like NTL quad_float or RR.
   */

public:
  template<typename FPT>
  explicit smallest(FPT s)
  :
  smallest_(s)
  { /*! Constructor with user defined value of smallest, for example 10 * min_value<FPT>.
        Note that some user-defined floating-point types may not specialize std::numeric_limits<FPT>::min_value()
        so it is convenient to use boost::math::tools::min_value<FPT>(); instead.
    */
  }

  smallest()
  :
  smallest_(2 * boost::math::tools::min_value<FPT>())
  { /*! Default Constructor with smallest_ =  2. * boost::math::tools::min_value<double>();\n
       multiplier m = 2 (must be integer or static_cast<FPT>())
       is chosen to allow for a few bits of computation error.\n
       Pessimistic multiplier is the number of arithmetic operations,
       assuming every operation causes a 1 least significant bit error,
       but a more realistic average might be half this.
     */
  }

  bool operator()(FPT fp_value, FPT s)
  { //! True if value is smaller than a smallest value s.
    if (fpt_abs(fp_value) == static_cast<FPT>(0))
    { // Test for zero first in case FPT is actually an integer type zero,
      // when the comparison < below would fail because
      // smallest_ could become zero when min_value converts to integer.
      return true;
    }
    return fpt_abs(fp_value) < fpt_abs(s);
  } // bool operator()

  bool operator()(FPT fp_value)
  { //! True if value is smaller than chosen smallest value.
    if (fpt_abs(fp_value) == static_cast<FPT>(0))
    { // Test for zero first in case FPT is actually an integer type,
      // when the comparison < below would fail because smallest could become zero.
      return true;
    }
    return fpt_abs(fp_value) < fpt_abs(smallest_);
  } // bool operator()

  FPT size()
  { //! \return chosen smallest value that will be counted as effectively zero.
    return smallest_;
  }

private:
  FPT smallest_; //!< Chosen smallest value that will be counted as effectively zero.
}; // class smallest

// Define two convenience typedefs.

typedef smallest<double> tiny;
/*!
  \typedef tiny
  \brief Allow tiny as a shorthand for twice the double min_value 4.45e-308.
  \details Since double and the default smallest value 2 * std::numeric_limits<double>::min_value() = 2 * 2.22507e-308 + 4.45015e-308
  is a very common requirement, provide an convenience alias for this.
*/

/*!
  \typedef neareq
  \brief Allow neareq as a shorthand for twice double epsilon = 4.44e-16
  \details Since double and the default close_to value 2 * epsilon =  std::numeric_limits<double>::epsilon = 2 * 2.220446e-016 = 4.440892e-016
  is a very common requirement, provide an convenience alias for this.
*/
typedef close_to<double> neareq;

 // namespace math
 // namespace boost

#endif // BOOST_FLOATING_POINT_COMPARISON_HPP
