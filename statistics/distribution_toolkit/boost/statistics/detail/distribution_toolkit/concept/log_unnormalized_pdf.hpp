//////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::concept::log_unnormalized_pdf.hpp       //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_CONCEPT_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_CONCEPT_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace concept{

    // Why would this fail?
    // If D is in boost::math, include (or if neccessary create)
    // distribution_toolkit/distribution/D/is_math_distribution
    // distribution_toolkit/distribution/D/log_unnormalized_pdf
    template<typename D>
    class LogUnnormalizedPdf{
        public:
    
        BOOST_CONCEPT_USAGE(LogUnnormalizedPdf){
            val =  log_unnormalized_pdf(dist,x);
        }

        private:
        typedef typename meta::value<D>::type value_;
        D dist;
        value_ val;
        value_ x;
        
    };

}// concept
}// distribution_toolkit
}// statistics
}// detail
}// boost

#endif