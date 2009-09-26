//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::map_pdf::inverse_pdf::log_unnomarlized_pdf.hp      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_MAP_PDF_INVERSE_PDF_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_MAP_PDF_INVERSE_PDF_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/inverse_pdf/inverse_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/log_unnormalized_pdf.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename D>
    typename meta::value< inverse_pdf<D> >::type
    log_unnormalized_pdf(
        const inverse_pdf<D>& dist,
        const typename meta::value< inverse_pdf<D> >::type& x
    ){
        BOOST_CONCEPT_ASSERT((
            concept::LogUnnormalizedPdf<D>
        ));
        return
            ( -log_unnormalized_pdf(dist.distribution(),x) );
    }

}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif 