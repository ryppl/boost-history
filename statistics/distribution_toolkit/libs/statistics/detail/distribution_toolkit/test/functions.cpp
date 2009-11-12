//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::test::functions.h                                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_FUNCTIONS_H_ER_2009
#define LIBS_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_FUNCTIONS_H_ER_2009
#include <ostream>

#include <boost/concept/assert.hpp>

#include <boost/statistics/detail/distribution_toolkit/distributions/chi_squared/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/exponential/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/gamma/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/students_t/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/uniform/include.hpp>

#include <boost/statistics/detail/distribution_toolkit/fwd_math/cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/mean.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/quantile.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/variance.hpp>

#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/location_scale.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/quantile.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/mean.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/derivative_log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/variance.hpp>

#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/include.hpp>

#include <boost/statistics/detail/distribution_toolkit/map_pdf/inverse_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/product_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/ratio_pdf/include.hpp>

// Concepts must come after distributions
#include <boost/statistics/detail/distribution_common/concept/unary/mean.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/variance.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/quantile.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/cdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/pdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/derivative_log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/concept/aggregate/function_ptrs.hpp>

#include <boost/statistics/detail/distribution_common/meta/wrapper_distribution.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper_variable.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper_probability.hpp>

template <typename T>
void test_functions(std::ostream& os)
{
    os << "test_functions - >" << std::endl;
    using namespace boost;
    using namespace math;
   
    namespace st        = boost::statistics::detail;
    namespace st_dc     = st::distribution::common;
    namespace st_dc_c   = st_dc::concept;
    namespace st_tk     = st::distribution::toolkit;
    namespace st_tk_c   = st_tk::concept;

    typedef st_dc::meta::wrapper_variable<T> x_;
    typedef st_dc::meta::wrapper_probability<T,x_> p_x_; 

    {
        typedef math::chi_squared_distribution<T>                   d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
            st_tk_c::FunctionPtrs<d_>
        ));

        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                    st_dc_c::Variance<w_>
                                >
                            >
                        > 
                    > 
                > 
            >
        ));
    }
    {
        typedef math::exponential_distribution<T>                   d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;
        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                    st_dc_c::Variance<w_>
                                >
                            >
                        > 
                    > 
                > 
            >
        ));
    }
    {
        typedef math::gamma_distribution<T>                         d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;
        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                    st_dc_c::Variance<w_>
                                >
                            >
                        > 
                    > 
                > 
            >
        ));

    }
    {
        typedef math::normal_distribution<T>                        d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;
        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                    st_dc_c::Variance<w_>
                                >
                            >
                        > 
                    > 
                > 
            >
        ));
    }
    {
        typedef math::students_t_distribution<T>                    d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                   st_dc_c::Variance<w_>
                               >
                           >
                       > 
                   > 
               > 
           >
        ));
    }
    {
        typedef math::normal_distribution<T>                        d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                   st_dc_c::Variance<w_>
                               >
                           >
                       > 
                   > 
               > 
           >
        ));
    }
    {
        typedef math::normal_distribution<T>                        z_;
        typedef st_tk::location_scale_distribution< z_ >            d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                    st_dc_c::Variance<w_>
                                >
                            >
                        > 
                    > 
                > 
            >
        ));
    }
    {
        typedef math::normal_distribution<T>                        d0_;
        typedef st_tk::wrapper< const d0_& >                        d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;
    
        BOOST_CONCEPT_ASSERT((
            st_dc_c::DerivativeLogUnnormalizedPdf<
                st_dc_c::LogUnnormalizedPdf<
                    st_dc_c::Pdf<
                        st_dc_c::Quantile<
                            st_dc_c::Cdf<
                                st_dc_c::Mean<
                                    st_dc_c::Variance<w_>
                                >
                            >
                        > 
                    > 
                > 
            >
        ));
    }
    {
        typedef math::normal_distribution<T>                        d0_;
        typedef st_tk::inverse_pdf< d0_ >                           d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;
    
        BOOST_CONCEPT_ASSERT((
            st_dc_c::LogUnnormalizedPdf<w_>
        ));
    }
    {
        typedef math::normal_distribution<T>                        d0_;
        typedef math::chi_squared_distribution<T>                   d1_;
        typedef st_tk::product_pdf< d0_, d1_ >                      d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>          w_;
    
        BOOST_CONCEPT_ASSERT((
            st_dc_c::LogUnnormalizedPdf<w_>
        ));
    }
    {
        typedef math::normal_distribution<T>                        d0_;
        typedef math::chi_squared_distribution<T>                   d1_;
        typedef typename st_tk::meta_ratio_pdf< d0_, d1_ >::type    d_;
        typedef st_dc::meta::wrapper_distribution<d_,p_x_>  w_;
    
        BOOST_CONCEPT_ASSERT((
            st_dc_c::LogUnnormalizedPdf<w_>
        ));
    }
    os << " <- " << std::endl;
}

#endif