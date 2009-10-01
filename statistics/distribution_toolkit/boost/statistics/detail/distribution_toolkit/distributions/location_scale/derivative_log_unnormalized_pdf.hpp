///////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::location_scale::log_unnormalized_pdf.hpp //
//                                                                               //
//  (C) Copyright 2009 Erwann Rogard                                             //
//  Use, modification and distribution are subject to the                        //
//  Boost Software License, Version 1.0. (See accompanying file                  //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_LOCATION_SCALE_DERIVATIVE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_LOCATION_SCALE_DERIVATIVE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/location_scale.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/unary/derivative_log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename Z>
    typename meta::value< location_scale_distribution<Z> >::type
    derivative_log_unnormalized_pdf(
        const location_scale_distribution<Z>& d,
        const typename meta::value< location_scale_distribution<Z> >::type& x
    ){
        typedef typename 
            meta::value< location_scale_distribution<Z> >::type value_t;
        value_t z = (x-d.mu())/d.sigma();
        BOOST_CONCEPT_ASSERT((
            concept::DerivativeLogUnnormalizedPdf<Z>
        ));
        value_t result = derivative_log_unnormalized_pdf(d.z(),z); 
        result /= d.sigma(); // = dz/dx
        return result;
    }


}// distribution_toolkit
}// detail
}// statistics 
}// boost

#endif