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
#include <boost/mpl/identity.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_pseudo_scalar_distribution.hpp>

namespace boost{

    template<typename D>
    typename boost::lazy_enable_if<
        boost::statistics::detail::distribution_toolkit
            ::meta::is_pseudo_scalar_distribution<D>,
            mpl::identity<std::ostream&> 
    >::type
    operator<<(std::ostream& os,const D& dist){
        return (
            os << 
                statistics::detail::distribution_toolkit::description(dist)
        );
    }


}

#endif





