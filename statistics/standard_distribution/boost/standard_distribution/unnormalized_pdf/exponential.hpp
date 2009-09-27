//////////////////////////////////////////////////////////////////////////////
// standard_distribution::unnormalized_pdf::exponential.hpp                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_EXPONENTIAL_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_EXPONENTIAL_HPP_ER_2009
#include <string>
#include <boost/math/distributions/exponential.hpp>
#include <boost/math/policies/policy.hpp> 

namespace boost{
namespace math{

    template<typename T,typename P>
    T
    log_unnormalized_pdf(
        const boost::math::exponential_distribution<T,P>& d,
        const T& x
    ){

        typedef std::string str_;
        BOOST_MATH_STD_USING // for ADL of std functions
        static const str_ function = 
            str_("boost::math::log_unnormalized_pdf(")+ 
            "const exponential_distribution<%1%>&,%1%)"
        ;

        T lambda = d.lambda();
        T result;
        if(0 == detail::verify_lambda(function, lambda, &result, P()))
            return result;
        if(0 == detail::verify_exp_x(function, x, &result, P()))
            return result;
        result = (-lambda * x);
        return result;
    }

    template<typename T,typename P>
    T
    derivative_log_unnormalized_pdf(
        const boost::math::exponential_distribution<T,P>& d,
        const T& x
    ){

        BOOST_MATH_STD_USING // for ADL of std functions
        // to keep width = 80 caracters
        static const char* function = strcpy(
            "boost::math::derivative_log_unnormalized_pdf(",
            "const exponential_distribution<%1%>&,%1%)"
        );

        T lambda = d.lambda();
        T result;
        if(0 == detail::verify_lambda(function, lambda, &result, P()))
            return result;
        if(0 == detail::verify_exp_x(function, x, &result, P()))
            return result;
        result = (-lambda);
        return result;
    }

}//math
}//boost

#endif
