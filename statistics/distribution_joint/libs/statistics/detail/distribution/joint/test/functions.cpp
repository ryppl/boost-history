///////////////////////////////////////////////////////////////////////////////
// distribution::joint::test::functions.cpp                                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_STATISTICS_DETAIL_DISTRIBUTION_JOINT_TEST_FUNCTIONS_HPP_ER_2009
#define LIBS_STATISTICS_DETAIL_DISTRIBUTION_JOINT_TEST_FUNCTIONS_HPP_ER_2009
#include <vector>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/iid/iid.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/iid/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/independent/independent.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/independent/log_unnormalized_pdf.hpp>

#include <boost/statistics/detail/distribution_common/concept/unary/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper_distribution.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper_variable.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper_probability.hpp>

template<typename T,typename X>
void test_functions(std::ostream& os)
{
    os << "-> test_functions" << std::endl;

    using namespace boost;
    namespace stat  = boost::statistics::detail;
    namespace dist  = stat::distribution;
    namespace tk    = dist::toolkit;
    namespace dj    = dist::joint;
    namespace dc    = dist::common;
    namespace dc_c  = dc::concept;

    typedef dc::meta::wrapper_variable<X> x_;
    typedef dc::meta::wrapper_probability<T,x_> p_x_; 

    {
        typedef math::normal_distribution<T>                   d_;
        typedef dj::iid<d_>                                    jd_;
        typedef dc::meta::wrapper_distribution<jd_,p_x_>       w_;

        BOOST_CONCEPT_ASSERT((
            dc_c::LogUnnormalizedPdf<w_>
        ));

    }
    {
        typedef math::normal_distribution<T>                   d_;
        typedef std::vector<d_>                                ds_;
        typedef dj::independent<ds_>                           jd_;
        typedef dc::meta::wrapper_distribution<jd_,p_x_>       w_;

        BOOST_CONCEPT_ASSERT((
            dc_c::LogUnnormalizedPdf<w_>
        ));

    }
    
    os << "<-" << std::endl;

}

#endif