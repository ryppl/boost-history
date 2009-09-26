//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::map_pdf::product_pdf::log_unnomralized_pdf.hpp     //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_MAP_PDF_PRODUCT_PDF_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_MAP_PDF_PRODUCT_PDF_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/product_pdf/product_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/log_unnormalized_pdf.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename A,typename B>
    typename meta::value < product_pdf<A,B> >::type
    log_unnormalized_pdf(
        const product_pdf<A,B>& dist,
        const typename meta::value< product_pdf<A,B> >::type& x
    ){

        BOOST_CONCEPT_ASSERT((
            concept::LogUnnormalizedPdf<A>
        ));
        BOOST_CONCEPT_ASSERT((
            concept::LogUnnormalizedPdf<A>
        ));

        return
            log_unnormalized_pdf(dist.first(),x)
                + log_unnormalized_pdf(dist.second(),x);
    }


}// distribution_toolkit
}// detail
}// statistics
}// boost


#endif 