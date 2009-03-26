/////////////////////////////////////////////////////////////////////////////
// normal.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FUNCTION_NORMAL_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FUNCTION_NORMAL_HPP_ER_2009
#include <cmath>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/math/constants/constants.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace function{
// T lgamma_imp(T z, const Policy& pol, const lanczos::undefined_lanczos& l, int*sign)
//static const char* function = "boost::math::lgamma<%1%>(%1%)";
//      if(floor(z) == z)
//         return policies::raise_pole_error<T>(function, "Evaluation of tgamma at a negative integer %1%.", z, pol);

    template<typename D,typename Args>
    typename function_argument<D>::type
    zscore(
        const D& d, // d.mu(args), d.sigma(args)
        const Args& args,
        typename function_argument<D>::type x//,
        //const Policy&, pol //TODO
    ){
        typedef typename function_argument<D>::type value_type;
        value_type m = d.mu(args);
        value_type s = d.sigma(args);
        value_type result = (x-m)/s;

        return result;
    };

    template<typename D,typename Args>
    typename function_value<D>::type
    unnormalized_log_pdf(
        const crtp::normal<D>& d,
        const Args& args,
        typename function_argument<D>::type x
    ){
        typedef typename function_argument<D>::type value_type;
        const D&   d_           = static_cast<const D&>(d);
        value_type z            = zscore(d_,args,x);
        value_type sigma        = d_.sigma(args);
        value_type log_sigma    = log(sigma);
        value_type result       = -(z*z)/((value_type)(2)) - log_sigma;
        return result;
	};

    template<typename D,typename Args>
    typename function_value<D>::type
    derivative_unnormalized_log_pdf(
        const crtp::normal<D>& d,
        const Args& args,
        typename function_argument<D>::type x
    ){
        typedef typename function_argument<D>::type value_type;
        const D& d_             = static_cast<const D&>(d);
        value_type z            = zscore(d_,args,x);
        value_type sigma        = d_.sigma(args);
        value_type result       = - (z / sigma);
        return result;
	};



}
}
}

#endif // BOOST_DISTRIBUTION_FUNCTION_NORMAL_HPP_ER_2009
