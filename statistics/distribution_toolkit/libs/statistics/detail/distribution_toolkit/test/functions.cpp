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
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/include.hpp>

#include <boost/statistics/detail/distribution_toolkit/map_pdf/inverse_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/product_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/ratio_pdf/include.hpp>

#include <boost/statistics/detail/distribution_common/concept/unary/description.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/mean.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/variance.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/quantile.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/cdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/pdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/derivative_log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/os.hpp>

#include <boost/statistics/detail/distribution_common/distributions/reference/include.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper/distribution.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper/variable.hpp>
#include <boost/statistics/detail/distribution_common/meta/wrapper/probability.hpp>

template <typename T>
void test_functions(std::ostream& os)
{
    os << "test_functions - >";
    using namespace boost;
    using namespace math;
   
    namespace dist = boost::statistics::detail::distribution;

    typedef dist::meta::wrapper_variable<T> x_;
    typedef dist::meta::wrapper_probability<T,x_> p_x_; 

    {
        typedef math::chi_squared_distribution<T>                   d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>          w_;
        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
                        		> 
                    		>	 
                		> 
            		>
            	>
            >
        ));
    }
    {
        typedef boost::math::exponential_distribution<T>     d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>    w_;
        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
                        		> 
                    		>	 
                		> 
            		>
            	>
            >
        ));
    }
    {
        typedef math::gamma_distribution<T>                d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>  w_;
        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
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
        typedef dist::meta::wrapper_distribution<d_,p_x_>          w_;
        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
                        		> 
                    		>	 
                		> 
            		>
            	>
            >
        ));
    }
    {
        typedef math::students_t_distribution<T>                   d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
                        		> 
                    		>	 
                		> 
            		>
            	>
            >
        ));
    }
    {
        typedef boost::math::uniform_distribution<T>                d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>          	w_;

        BOOST_CONCEPT_ASSERT((
        	dist::concept::Description<
            	dist::concept::DerivativeLogUnnormalizedPdf<
                	dist::concept::LogUnnormalizedPdf<
                    	dist::concept::Pdf<
                        	dist::concept::Quantile<
                            	dist::concept::Cdf<
                                	dist::concept::Mean<
                                    	dist::concept::Variance<w_>
                                	>
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
        typedef dist::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
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
        typedef dist::toolkit::location_scale_distribution< z_ >    d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>          w_;

        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
                        		> 
                    		>	 
                		> 
            		>
            	>
            >
        ));
    }
    {
        typedef math::normal_distribution<T>                d0_;
        typedef dist::reference_wrapper< const d0_& >       d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>   w_;
    
        BOOST_CONCEPT_ASSERT((
        	dist::concept::Os<
        		dist::concept::Description<
            		dist::concept::DerivativeLogUnnormalizedPdf<
                		dist::concept::LogUnnormalizedPdf<
                    		dist::concept::Pdf<
                        		dist::concept::Quantile<
                            		dist::concept::Cdf<
                                		dist::concept::Mean<
                                    		dist::concept::Variance<w_>
                                		>
                            		>
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
        typedef dist::toolkit::inverse_pdf< d0_ >                  	d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>          	w_;
    
        BOOST_CONCEPT_ASSERT(( 
        	dist::concept::Os<
        		dist::concept::Description<
        			dist::concept::LogUnnormalizedPdf<w_> 
                >
            >
        ));
    }
    {
        typedef math::normal_distribution<T>                        d0_;
        typedef math::chi_squared_distribution<T>                   d1_;
        typedef dist::toolkit::product_pdf< d0_, d1_ >              d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>  			w_;
    
        BOOST_CONCEPT_ASSERT((
        	dist::concept::Description<
            	dist::concept::LogUnnormalizedPdf<w_>
            >
        ));
    }
    {
        typedef math::normal_distribution<T>                        		d0_;
        typedef math::chi_squared_distribution<T>                   		d1_;
        typedef typename dist::toolkit::meta_ratio_pdf< d0_, d1_ >::type    d_;
        typedef dist::meta::wrapper_distribution<d_,p_x_>  w_;
    
        BOOST_CONCEPT_ASSERT(( 
        	dist::concept::Description<
	        	dist::concept::LogUnnormalizedPdf<w_>
            >
        ));
    }

    os << " <- " << std::endl;
}

#endif