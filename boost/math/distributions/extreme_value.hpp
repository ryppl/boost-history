//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STATS_EXTREME_VALUE_HPP
#define BOOST_STATS_EXTREME_VALUE_HPP

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/log1p.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/math/distributions/complement.hpp>
#include <boost/math/distributions/detail/common_error_handling.hpp>
#include <cmath>

//
// This is the maximum extreme value distribution, see
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda366g.htm
// and http://mathworld.wolfram.com/ExtremeValueDistribution.html
// Also known as a Fisher-Tippett distribution, a log-Weibull
// distribution or a Gumbel distribution.

#include <utility>

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable: 4702) // unreachable code (return after domain_error throw).
#endif

namespace boost{ namespace math{

namespace detail{
//
// Error check:
//
template <class RealType>
inline bool verify_scale_b(const char* function, RealType b, RealType* presult)
{
   if(b <= 0)
   {
      *presult = tools::domain_error<RealType>(
         function,
         "The scale parameter \"b\" must be > 0, but was: %1%.", b);
      return false;
   }
   return true;
}

} // namespace detail

template <class RealType = double>
class extreme_value_distribution
{
public:
   typedef RealType value_type;

   extreme_value_distribution(RealType a = 0, RealType b = 1)
      : m_a(a), m_b(b)
   {
      RealType err;
      detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &err);
   } // extreme_value_distribution

   RealType scale()const { return m_b; }
   RealType location()const { return m_a; }

private:
   RealType m_a, m_b;
};

typedef extreme_value_distribution<double> extreme_value;

template <class RealType>
const std::pair<RealType, RealType> range(const extreme_value_distribution<RealType>& dist)
{ // Range of permissible values for random variable x.
	using boost::math::tools::max_value;
	return std::pair<RealType, RealType>(-max_value<RealType>(), max_value<RealType>());
}

template <class RealType>
const std::pair<RealType, RealType> support(const extreme_value_distribution<RealType>& dist)
{ // Range of supported values for random variable x.
	// This is range where cdf rises from 0 to 1, and outside it, the pdf is zero.
	using boost::math::tools::max_value;
	return std::pair<RealType, RealType>(-max_value<RealType>(),  max_value<RealType>());
}


template <class RealType>
RealType pdf(const extreme_value_distribution<RealType>& dist, const RealType& x)
{
   using namespace std; // for ADL of std functions

   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;
   result = exp((a-x)/b) * exp(-exp((a-x)/b)) / b;
   return result;
} // pdf

template <class RealType>
RealType cdf(const extreme_value_distribution<RealType>& dist, const RealType& x)
{
   using namespace std; // for ADL of std functions

   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;

   result = exp(-exp((a-x)/b));

   return result;
} // cdf

template <class RealType>
RealType quantile(const extreme_value_distribution<RealType>& dist, const RealType& p)
{
   using namespace std; // for ADL of std functions

   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;
   if(0 == detail::check_probability(BOOST_CURRENT_FUNCTION, p, &result))
      return result;

   if(p == 0)
      return -tools::overflow_error<RealType>(BOOST_CURRENT_FUNCTION, 0);
   if(p == 1)
      return tools::overflow_error<RealType>(BOOST_CURRENT_FUNCTION, 0);

   result = a - log(-log(p)) * b;

   return result;
} // quantile

template <class RealType>
RealType cdf(const complemented2_type<extreme_value_distribution<RealType>, RealType>& c)
{
   using namespace std; // for ADL of std functions

   RealType a = c.dist.location();
   RealType b = c.dist.scale();
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;

   result = -expm1(-exp((a-c.param)/b));

   return result;
}

template <class RealType>
RealType quantile(const complemented2_type<extreme_value_distribution<RealType>, RealType>& c)
{
   using namespace std; // for ADL of std functions

   RealType a = c.dist.location();
   RealType b = c.dist.scale();
   RealType q = c.param;
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;
   if(0 == detail::check_probability(BOOST_CURRENT_FUNCTION, q, &result))
      return result;

   if(q == 0)
      return tools::overflow_error<RealType>(BOOST_CURRENT_FUNCTION, 0);
   if(q == 1)
      return -tools::overflow_error<RealType>(BOOST_CURRENT_FUNCTION, 0);

   result = a - log(-boost::math::log1p(-q)) * b;

   return result;
}

template <class RealType>
inline RealType mean(const extreme_value_distribution<RealType>& dist)
{
   RealType a = dist.location();
   RealType b = dist.scale();
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;
   return a + constants::euler<RealType>() * b;
}

template <class RealType>
inline RealType standard_deviation(const extreme_value_distribution<RealType>& dist)
{
   using namespace std; // for ADL of std functions.

   RealType b = dist.scale();
   RealType result;
   if(0 == detail::verify_scale_b(BOOST_CURRENT_FUNCTION, b, &result))
      return result;
   return constants::pi<RealType>() * b / sqrt(static_cast<RealType>(6));
}

template <class RealType>
inline RealType mode(const extreme_value_distribution<RealType>& dist)
{
   return dist.location();
}

template <class RealType>
inline RealType median(const extreme_value_distribution<RealType>& dist)
{
  using constants::ln_ln_two;
   return dist.location() - dist.scale() * ln_ln_two<RealType>();
}

template <class RealType>
inline RealType skewness(const extreme_value_distribution<RealType>& dist)
{
   //
   // This is 12 * sqrt(6) * zeta(3) / pi^3:
   // See http://mathworld.wolfram.com/ExtremeValueDistribution.html
   //
   return static_cast<RealType>(1.1395470994046486574927930193898461120875997958366L);
}

template <class RealType>
inline RealType kurtosis(const extreme_value_distribution<RealType>& dist)
{
   // See http://mathworld.wolfram.com/ExtremeValueDistribution.html
   return RealType(27) / 5;
}

template <class RealType>
inline RealType kurtosis_excess(const extreme_value_distribution<RealType>& dist)
{
   // See http://mathworld.wolfram.com/ExtremeValueDistribution.html
   return RealType(12) / 5;
}


} // namespace math
} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif

// This include must be at the end, *after* the accessors
// for this distribution have been defined, in order to
// keep compilers that support two-phase lookup happy.
#include <boost/math/distributions/detail/derived_accessors.hpp>

#endif // BOOST_STATS_EXTREME_VALUE_HPP
