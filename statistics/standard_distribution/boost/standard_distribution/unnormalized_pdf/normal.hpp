//////////////////////////////////////////////////////////////////////////////
// distributions::unnormalized_pdf::normal.hpp                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_NORMAL_HPP_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_UNNORMALIZED_PDF_NORMAL_HPP_ER_2009
#include <boost/math/distributions/normal.hpp>
#include <boost/numeric/conversion/converter.hpp>
//#include <boost/math/policies/policy.hpp>//TODO

namespace boost{
namespace math{

    // If only the unnormalized_pdf is required, this is supposed to
    // be faster than log(pdf(D,x))

    template<typename T,typename Policy>
    T
    log_unnormalized_pdf(
        const boost::math::normal_distribution<T,Policy>& d,
        const T& x
    ){
        typedef boost::numeric::converter<T,int> int2real_t;
        T mu = d.location();
        T sigma = d.scale();

        T z = (x-mu)/sigma;
        return - z * z / int2real_t::convert(2);
    }

    template<typename T,typename Policy>
    T
    derivative_log_unnormalized_pdf(
        const boost::math::normal_distribution<T,Policy>& d,
        const T& x
    ){
        typedef boost::numeric::converter<T,int> int2real_t;
        T mu = d.location();
        T sigma = d.scale();

        T z = (x-mu)/sigma;
        T dz = int2real_t::convert(1)/sigma;
        return (- z) * dz;
    }
}
}

#endif
