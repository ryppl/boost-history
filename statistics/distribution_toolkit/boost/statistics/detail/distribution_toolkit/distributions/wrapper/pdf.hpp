/////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::wrapper::pdf.hpp                       //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_WRAPPER_PDF_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/value.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/unary/pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/wrapper.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    template<typename D>
    typename meta::value< wrapper<D> >::type
    pdf(
        const wrapper<D>& w,
        typename meta::value< wrapper<D> >::type x
    ){
        typedef wrapper<D> w_;
        typedef typename w_::distribution_type d_;
        BOOST_CONCEPT_ASSERT((
            concept::Pdf<d_>
        ));

        return pdf(w.const_reference(),x);
    }

}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif