//////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::iid::log_unnormalized_pdf.hpp        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <stdexcept>
#include <numeric>
#include <functional>
#include <ostream>
#include <boost/range.hpp>
#include <boost/format.hpp>

#include <boost/statistics/detail/distribution_common/meta/value.hpp>

#include <boost/statistics/detail/distribution_toolkit/fun_wrap/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/iterator/distribution_function.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/iid/iid.hpp>

namespace boost{
namespace statistics{
namespace detail{

    template<typename D,typename R>
    typename distribution::common::meta::value< 
        distribution::joint::iid<D> 
    >::type 
    log_unnormalized_pdf(const distribution::joint::iid<D>& dist,const R& x){
        static const char* msg = 
            "log_unnormalized_pdf(%1%,x) : size(x) = %2% != dim";
        if( boost::size(x) != dist.dimension() ){
            throw std::runtime_error(
                (format(msg)%dist%size(x)).str()
            );
        }
        typedef distribution::joint::iid<D> iid_;
        typedef typename iid_::value_type val_;
        val_ init = static_cast<val_>(0);
        val_ log_pdf 
            = std::accumulate(
                    distribution::toolkit::iterator::make_distribution_function<
                    distribution::toolkit::fun_wrap::log_unnormalized_pdf_
                >(dist.element(),boost::begin(x)),
                    distribution::toolkit::iterator::make_distribution_function<
                    distribution::toolkit::fun_wrap::log_unnormalized_pdf_
                >(dist.element(),boost::end(x)),
                init
            );

        return log_pdf;
    };

}// detail
}// statistics
}// boost

#endif
