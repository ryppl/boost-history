//////////////////////////////////////////////////////////////////////////////
// distributions::unnormalized_pdf::gamma.hpp                               //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MATH_UNNORMALIZED_PDF_GAMMA_HPP_ER_2009
#define BOOST_MATH_UNNORMALIZED_PDF_GAMMA_HPP_ER_2009
#include <cmath>
#include <boost/math/distributions/gamma.hpp>
#include <boost/math/policies/policy.hpp>

namespace boost{
namespace math{

template <class T, class Policy>
inline bool is_concave(
    const gamma_distribution<T, Policy>& dist
){
    return ( dist.shape() > static_cast<T>(1) );
}

template <class T, class Policy>
inline T log_unnormalized_pdf(
    const gamma_distribution<T, Policy>& dist,
    const T& x
)
{
   BOOST_MATH_STD_USING  // for ADL of std functions

   static const char* function
    = "boost::math::log_unnormalized_pdf(const gamma_distribution<%1%>&, %1%)";

   T shape = dist.shape();
   T scale = dist.scale();

   T result;
   if(false == detail::check_gamma(function, scale, shape, &result, Policy()))
      return result;
   if(false == detail::check_gamma_x(function, x, &result, Policy()))
      return result;

   if(x == 0)
   {
      return log(0);
   }
   static T one_ = static_cast<T>(1);
   result = (shape - one_) * log(x) - x / scale;
   return result;
} // log_unnormalized_pdf

template <class T, class Policy>
inline T derivative_log_unnormalized_pdf(
    const gamma_distribution<T, Policy>& dist,
    const T& x
)
{
   BOOST_MATH_STD_USING  // for ADL of std functions

   static const char* function
 = "boost::math::log_unnormalized_pdf(const gamma_distribution<%1%>&, %1%)";

   T shape = dist.shape();
   T scale = dist.scale();

   T result;
   if(false == detail::check_gamma(function, scale, shape, &result, Policy()))
      return result;
   if(false == detail::check_gamma_x(function, x, &result, Policy()))
      return result;

   if(x == 0)
   {
      return log(0);
   }
   static T one_ = static_cast<T>(1);
   result = (shape - one_) / x - one_ / scale;
   return result;
} // derivative_log_unnormalized_pdf

}
}

#endif
