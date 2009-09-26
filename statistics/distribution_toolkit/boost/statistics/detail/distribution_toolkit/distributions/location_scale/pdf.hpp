//////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::location_scale::pdf.hpp         //
//                                                                      //
//  (C) Copyright 2009 Erwann Rogard                                    //
//  Use, modification and distribution are subject to the               //
//  Boost Software License, Version 1.0. (See accompanying file         //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    //
//////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_LOCATION_SCALE_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_LOCATION_SCALE_PDF_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/location_scale.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/pdf.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename Z>
    typename location_scale_distribution<Z>::value_type
    pdf(
        const location_scale_distribution<Z>& d,
        const typename 
            location_scale_distribution<Z>::value_type& x
    ){
        typedef typename 
            location_scale_distribution<Z>::value_type value_t;
        value_t z = (x-d.mu())/d.sigma();
        
        BOOST_CONCEPT_ASSERT((
            concept::Pdf<Z>
        ));
        
        value_t result = pdf(d.z(),z);
        result /= d.sigma();
        return result;
    }

}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif