//////////////////////////////////////////////////////////////////////////////
// distributions::unnormalized_pdf::chi_squared.hpp                          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_CHI_SQUARED_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_CHI_SQUARED_HPP_ER_2009
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/special_functions/log1p.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/math/policies/policy.hpp>

namespace boost{
namespace math{

    template<typename T,typename Policy>
    T
    log_unnormalized_pdf(
        const boost::math::chi_squared_distribution<T,Policy>& dist,
        const T& x
    ){
        BOOST_MATH_STD_USING  // for ADL of std functions
        T degrees_of_freedom = dist.degrees_of_freedom();
        // Error check:
        T error_result;

        static const char* function = 
"boost::math::log_unnormalized_pdf(const chi_squared_distribution<%1%>&, %1%)";

        if(false == detail::check_df(
            function, degrees_of_freedom, &error_result, Policy()))
                return error_result;

        if((x <= 0) || !(boost::math::isfinite)(x))
        {
            return policies::raise_domain_error<T>(
            function, "Chi Square parameter was %1%, but must be > 0 !", 
            x, Policy());
        }

        // Lumped case x == 0 in x<=0 above

        static T one = static_cast<T>(1);
        static T two = static_cast<T>(2);
        return (degrees_of_freedom/two-one) * log(x) - x/two;
    }

    template<typename T,typename Policy>
    T
    derivative_log_unnormalized_pdf(
        const boost::math::chi_squared_distribution<T,Policy>& dist,
        const T& x
    ){
        BOOST_MATH_STD_USING  // for ADL of std functions
        T degrees_of_freedom = dist.degrees_of_freedom();
        // Error check:
        T error_result;

        static const char* function = 
"boost::math::log_unnormalized_pdf(const chi_squared_distribution<%1%>&, %1%)";

        if(false == detail::check_df(
            function, degrees_of_freedom, &error_result, Policy()))
                return error_result;

        if((x < 0) || !(boost::math::isfinite)(x))
        {
            return policies::raise_domain_error<T>(
            function, "Chi Square parameter was %1%, but must be > 0 !", 
            x, Policy());
        }

        // Lumped case x == 0 in x<=0 above

        static T one = static_cast<T>(1);
        static T two = static_cast<T>(2);
        return (degrees_of_freedom/two-one) / x - one/two;
    }

}
}

#endif