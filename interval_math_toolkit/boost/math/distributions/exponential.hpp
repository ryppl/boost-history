//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STATS_EXPONENTIAL_HPP
#define BOOST_STATS_EXPONENTIAL_HPP

#include <boost/math/distributions/fwd.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/log1p.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/math/distributions/complement.hpp>
#include <boost/math/distributions/detail/common_error_handling.hpp>
#include <cmath>

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable: 4702) // unreachable code (return after domain_error throw).
#endif

#include <utility>

namespace boost{ namespace math{

namespace detail{
//
// Error check:
//
template <class RealType, class Policy>
inline bool verify_lambda(const char* function, RealType l, RealType* presult, const Policy& pol)
{
   BOOST_MATH_STD_USING
   if(l <= 0)
   {
      *presult = policies::raise_domain_error<RealType>(
         function,
         "The scale parameter \"lambda\" must be > 0, but was: %1%.", l, pol);
      return false;
   }
   return true;
}

template <class RealType, class Policy>
inline bool verify_exp_x(const char* function, RealType x, RealType* presult, const Policy& pol)
{
   BOOST_MATH_STD_USING
   if(x < 0)
   {
      *presult = policies::raise_domain_error<RealType>(
         function,
         "The random variable must be >= 0, but was: %1%.", x, pol);
      return false;
   }
   return true;
}

} // namespace detail

template <class RealType = double, class Policy = policies::policy<> >
class exponential_distribution
{
public:
   typedef RealType value_type;
   typedef Policy policy_type;

   exponential_distribution(RealType lambda = 1)
      : m_lambda(lambda)
   {
      RealType err;
      detail::verify_lambda("boost::math::exponential_distribution<%1%>::exponential_distribution", lambda, &err, Policy());
   } // exponential_distribution

   RealType lambda()const { return m_lambda; }

private:
   RealType m_lambda;
};

typedef exponential_distribution<double> exponential;

template <class RealType, class Policy>
inline const std::pair<RealType, RealType> range(const exponential_distribution<RealType, Policy>& /*dist*/)
{ // Range of permissible values for random variable x.
   using boost::math::tools::max_value;
   return std::pair<RealType, RealType>(static_cast<RealType>(0), max_value<RealType>());
}

template <class RealType, class Policy>
inline const std::pair<RealType, RealType> support(const exponential_distribution<RealType, Policy>& /*dist*/)
{ // Range of supported values for random variable x.
   // This is range where cdf rises from 0 to 1, and outside it, the pdf is zero.
   using boost::math::tools::max_value;
   using boost::math::tools::min_value;
   return std::pair<RealType, RealType>(min_value<RealType>(),  max_value<RealType>());
   // min_value<RealType>() to avoid a discontinuity at x = 0.
}

template <class RealType, class Policy>
inline RealType pdf(const exponential_distribution<RealType, Policy>& dist, const RealType& x)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::pdf(const exponential_distribution<%1%>&, %1%)";

   RealType lambda = dist.lambda();
   RealType result;
   if(0 == detail::verify_lambda(function, lambda, &result, Policy()))
      return result;
   if(0 == detail::verify_exp_x(function, x, &result, Policy()))
      return result;
   result = lambda * exp(-lambda * x);
   return result;
} // pdf

template <class RealType, class Policy>
inline RealType cdf(const exponential_distribution<RealType, Policy>& dist, const RealType& x)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::cdf(const exponential_distribution<%1%>&, %1%)";

   RealType result;
   RealType lambda = dist.lambda();
   if(0 == detail::verify_lambda(function, lambda, &result, Policy()))
      return result;
   if(0 == detail::verify_exp_x(function, x, &result, Policy()))
      return result;
   result = -boost::math::expm1(static_cast<RealType>(-x * lambda), Policy());

   return result;
} // cdf

template <class RealType, class Policy>
inline RealType quantile(const exponential_distribution<RealType, Policy>& dist, const RealType& p)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::quantile(const exponential_distribution<%1%>&, %1%)";

   RealType result;
   RealType lambda = dist.lambda();
   if(0 == detail::verify_lambda(function, lambda, &result, Policy()))
      return result;
   if(0 == detail::check_probability(function, p, &result, Policy()))
      return result;

   if(p == 0)
      return 0;
   if(p == 1)
      return policies::raise_overflow_error<RealType>(function, 0, Policy());

   result = -boost::math::log1p(static_cast<RealType>(-p), Policy()) / lambda;
   return result;
} // quantile

template <class RealType, class Policy>
inline RealType cdf(const complemented2_type<exponential_distribution<RealType, Policy>, RealType>& c)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::cdf(const exponential_distribution<%1%>&, %1%)";

   RealType result;
   RealType lambda = c.dist.lambda();
   if(0 == detail::verify_lambda(function, lambda, &result, Policy()))
      return result;
   if(0 == detail::verify_exp_x(function, c.param, &result, Policy()))
      return result;
   result = exp(-c.param * lambda);

   return result;
}

template <class RealType, class Policy>
inline RealType quantile(const complemented2_type<exponential_distribution<RealType, Policy>, RealType>& c)
{
   BOOST_MATH_STD_USING // for ADL of std functions

   static const char* function = "boost::math::quantile(const exponential_distribution<%1%>&, %1%)";

   RealType result;
   RealType lambda = c.dist.lambda();
   if(0 == detail::verify_lambda(function, lambda, &result, Policy()))
      return result;

   RealType q = c.param;
   if(0 == detail::check_probability(function, q, &result, Policy()))
      return result;

   if(q == 1)
      return 0;
   if(q == 0)
      return policies::raise_overflow_error<RealType>(function, 0, Policy());

   result = -log(q) / lambda;
   return result;
}

template <class RealType, class Policy>
inline RealType mean(const exponential_distribution<RealType, Policy>& dist)
{
   RealType result;
   RealType lambda = dist.lambda();
   if(0 == detail::verify_lambda("boost::math::mean(const exponential_distribution<%1%>&)", lambda, &result, Policy()))
      return result;
   return 1 / lambda;
}

template <class RealType, class Policy>
inline RealType standard_deviation(const exponential_distribution<RealType, Policy>& dist)
{
   RealType result;
   RealType lambda = dist.lambda();
   if(0 == detail::verify_lambda("boost::math::standard_deviation(const exponential_distribution<%1%>&)", lambda, &result, Policy()))
      return result;
   return 1 / lambda;
}

template <class RealType, class Policy>
inline RealType mode(const exponential_distribution<RealType, Policy>& /*dist*/)
{
   return 0;
}

template <class RealType, class Policy>
inline RealType median(const exponential_distribution<RealType, Policy>& dist)
{
   using boost::math::constants::ln_two;
   return ln_two<RealType>() / dist.lambda(); // ln(2) / lambda
}

template <class RealType, class Policy>
inline RealType skewness(const exponential_distribution<RealType, Policy>& /*dist*/)
{
   return 2;
}

template <class RealType, class Policy>
inline RealType kurtosis(const exponential_distribution<RealType, Policy>& /*dist*/)
{
   return 9;
}

template <class RealType, class Policy>
inline RealType kurtosis_excess(const exponential_distribution<RealType, Policy>& /*dist*/)
{
   return 6;
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

#endif // BOOST_STATS_EXPONENTIAL_HPP
