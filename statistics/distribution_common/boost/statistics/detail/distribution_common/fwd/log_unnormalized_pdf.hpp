///////////////////////////////////////////////////////////////////////////////
// distribution::common::fwd::log_unnormalized_pdf.hpp                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_COMMON_FWD_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_COMMON_FWD_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/statistics/detail/distribution_common/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
 
	template<typename D,typename X>
    typename 
    	boost::statistics::detail::distribution::common::meta::value<D>::type
    log_unnormalized_pdf(const D& dist,const X& x);

}// detail
}// statistics
}// boost

#endif