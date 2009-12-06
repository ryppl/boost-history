//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::example::location_scale.cpp                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <string>
#include <stdexcept>
#include <limits>

#include <boost/mpl/apply.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>

#include <boost/math/tools/precision.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/statistics/detail/distribution_common/functor/log_unnormalized_pdf.hpp>

#include <boost/statistics/detail/distribution_toolkit/meta/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/students_t/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/unscope/include.hpp>

#include <libs/statistics/detail/distribution_toolkit/example/distribution_function.h>

void example_distribution_function(std::ostream& out){

        out << "-> example_math_location_scale" << std::endl;

        using namespace boost;
        using namespace math;
        using namespace statistics::detail;
        namespace dist = distribution;

        // Types
        typedef double                                          	val_;
        typedef boost::mt19937                                  	urng_;
        typedef boost::normal_distribution<val_>                	rnd_;
        typedef math::students_t_distribution<val_>             	stud_;
        typedef dist::toolkit::location_scale_distribution<stud_>   ls_stud_;
        typedef boost::variate_generator<urng_&,rnd_>           	vg_;
        typedef boost::numeric::bounds<val_> 						bounds_;

        // Constants
        const unsigned df   = 10;
        const val_ mu       = 10;
        const val_ sigma    = 2;
        const val_ x        = 2.132;

        // Initialization
        urng_ urng;
        
        stud_ stud(df);
        ls_stud_ ls_stud(mu, sigma, stud);

		out << "testing error handling : " << std::endl;

        try{   
        	static const val_ inf = std::numeric_limits<val_>::infinity();
			ls_stud_ ls_stud(inf,sigma,stud);
        }catch(std::exception& e){
        	out << e.what() << std::endl;
		}

		try{
			ls_stud_(mu,-1.0,stud);
        }catch(std::exception& e){
        	out << e.what() << std::endl;
        }

        struct float_{
            
            static bool equal(const val_& a, const val_& b){
                static val_ e = boost::math::tools::epsilon<val_>();;
                return fabs(a-b)< e;
            }
        
        };
        {   // product_pdf
            typedef dist::toolkit::product_pdf<stud_,ls_stud_>     prod_dist_;
            prod_dist_ prod_dist(stud,ls_stud);
            prod_dist_ prod_dist2(stud,ls_stud);
            prod_dist = prod_dist2; // check assignment
            BOOST_ASSERT(
                float_::equal(
                    log_unnormalized_pdf(prod_dist,x),
                    log_unnormalized_pdf(prod_dist2,x)
                )
            );
            BOOST_ASSERT(
                float_::equal(
                    log_unnormalized_pdf(prod_dist.first(),x)
                     + log_unnormalized_pdf(prod_dist.second(),x),
                    log_unnormalized_pdf(prod_dist,x)
                )
            );

        }
        {   // inverse_pdf
            typedef dist::toolkit::inverse_pdf<stud_>             	inv_dist_;
            inv_dist_ inv_dist(stud);
            BOOST_ASSERT(
                float_::equal(
                    -log_unnormalized_pdf(inv_dist.distribution(),x),
                    log_unnormalized_pdf(inv_dist,x)
                )
            );

        }
        {   // ratio_pdf + fun_wrap
            typedef dist::toolkit::meta_ratio_pdf<stud_,ls_stud_>  	mf_;
            typedef mf_::type                                   	ratio_dist_;

            ratio_dist_ ratio_dist = mf_::call(stud,ls_stud);
            BOOST_ASSERT(
                float_::equal(
                    // + sign because inherits from inverse
                    log_unnormalized_pdf(ratio_dist.first(),x)
                        + log_unnormalized_pdf(ratio_dist.second(),x),
                    log_unnormalized_pdf(ratio_dist,x)
                )
            );
        }
    out << "<-" << std::endl;

}
