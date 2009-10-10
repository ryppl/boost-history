//////////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::independent::log_unnormalized_pdf.hpp    //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_INDEPENDENT_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_INDEPENDENT_LOG_UNNORMALIZED_PDF_HPP_ER_2009
#include <stdexcept>
#include <numeric>
#include <functional>
#include <ostream>
#include <boost/range.hpp>
#include <boost/format.hpp>

#include <boost/statistics/detail/distribution_common/meta/value.hpp>

#include <boost/statistics/detail/distribution_toolkit/fun_wrap/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/iterator/distribution_function.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/independent/independent.hpp>

namespace boost{
namespace statistics{
namespace detail{

    template<typename R,typename R1>
    typename distribution::common::meta::value< 
        distribution::joint::independent<R>
    >::type
    log_unnormalized_pdf(
        const distribution::joint::independent<R>& dist,
        const R1& x
    ){
        static const char* msg = 
            "log_unnormalized_pdf(%1%,x) : size(x) = %2% != dim";
        if( boost::size(x) != dist.dimension() ){
            throw std::runtime_error(
                (format(msg)%dist%size(x)).str()
            );
        }
        typedef distribution::joint::independent<R>                  indep_;
        typedef typename distribution::common::meta::value< 
            indep_ 
        >::type val_;
        typedef typename indep_::element_type                       dist_;
        val_ init = static_cast<val_>(0);

        typedef distribution::toolkit::fun_wrap::log_unnormalized_pdf_<
            dist_
        > f_;
        boost::begin(dist.elements());
        return std::inner_product(
            boost::begin(dist.elements()),
            boost::end(dist.elements()),
            boost::begin(x),
            init,
            boost::lambda::_1 + boost::lambda::_2,
            boost::lambda::bind(
                f_::instance,
               boost::lambda::_1,
                boost::lambda::_2
            )
        );
    }

}// detail
}// statistics
}// boost

#endif