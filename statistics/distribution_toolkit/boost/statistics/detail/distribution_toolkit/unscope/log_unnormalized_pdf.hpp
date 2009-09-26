//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::unscope::log_unnormalized_pdf.hpp                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_UNSCOPE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_UNSCOPE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_scalar_distribution.hpp>

namespace boost{

namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename D,typename T> 
    T log_unnormalized_pdf(const D& d,const T& x);
    
}// detail
}// distribution_toolkit
}// detail
}// statistics


    // The reason for unscoping is to have a common interface for 
    // distributions defined in distribution_toolkit and other
    // libraries, not necessarily restricted to scalars

    template<typename D,typename T>
    typename boost::enable_if<
        statistics::detail::distribution_toolkit::meta
            ::is_pseudo_scalar_distribution_distribution<D>,
        T // TODO or perhaps meta::value<D>::type, using lazy_enable_if
    >
    log_unnormalized_pdf(const D& d,const T& x){
        return statistics::detail::distribution_toolkit
            ::log_unnormalized_pdf<D>(d,x);
    }

}// boost

#endif
