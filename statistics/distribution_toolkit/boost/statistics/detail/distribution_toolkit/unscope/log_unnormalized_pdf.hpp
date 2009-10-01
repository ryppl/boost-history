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
#include <boost/statistics/detail/distribution_toolkit/meta/is_pseudo_scalar_distribution.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>

namespace boost{

    template<typename D,typename T>
    typename boost::lazy_enable_if<
        boost::statistics::detail::distribution_toolkit::meta
            ::is_pseudo_scalar_distribution<D>,
        boost::statistics::detail::distribution_toolkit::meta::value<D>
    >::type
    log_unnormalized_pdf(const D& d,const T& x){
        return boost::statistics::detail::distribution_toolkit
            ::log_unnormalized_pdf(d,x);
    }

}// boost

#endif
