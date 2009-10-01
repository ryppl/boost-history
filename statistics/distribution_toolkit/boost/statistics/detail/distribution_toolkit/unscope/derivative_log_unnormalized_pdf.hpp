//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::unscope::derivative_log_unnormalized_pdf.hpp       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_UNSCOPE_DERIVATIVE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_UNSCOPE_DERIVATIVE_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_pseudo_scalar_distribution.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>

namespace boost{

    template<typename D,typename T>
    typename boost::lazy_enable_if<
        statistics::detail::distribution_toolkit::meta
            ::is_pseudo_scalar_distribution<D>,
        statistics::detail::distribution_toolkit::meta::value<D>
    >::type
    derivative_log_unnormalized_pdf(const D& d,const T& x){
        return statistics::detail::distribution_toolkit
            ::derivative_log_unnormalized_pdf(d,x);
    }

}// boost

#endif