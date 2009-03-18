/////////////////////////////////////////////////////////////////////////////
// test_gaussian.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <list>
#include <vector>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/format.hpp>
#include <boost/adaptive_rejection_sampling/example/standard_gaussian.hpp>
#include <boost/adaptive_rejection_sampling/sampler.hpp>
#include <libs/adaptive_rejection_sampling/src/example/test_data.h>
#include <libs/adaptive_rejection_sampling/src/example/test_gaussian.h>
namespace libs{
namespace adaptive_rejection_sampling{
	void example_standard_gaussian(){
		std::cout<<"-> test_gaussian"<<std::endl;
        const double tolerance  = 1e-4; //1e-5 causes fail
        const unsigned report_every = 20;
		std::string str;
        str+= "We draw from the standard-gaussian using the algorithm.";
        str+= " Results are compared with a similar implementation in";
        str+= "Mathematica. A failure will be generated if the difference";
        str+= " is greater than %1%";
        str+= " The uniform draws were Mathematica generated.";
        boost::format f(str); f % tolerance;
        std::cout << f.str() << std::endl;

		Uniform_sampler_mathematica usm;
		typedef boost::adaptive_rejection_sampling
            ::standard_gaussian_evaluator<double>
            dist_fun_type;
        typedef 		boost::adaptive_rejection_sampling
            ::sampler<dist_fun_type,std::vector> dbars_t;
		dist_fun_type dist_fun;
		dbars_t dbars0(dist_fun,10);
		//,dist_fun,us);

        //checks that copy constructor works
		dbars_t dbars = dbars0;

		try{
			for(unsigned  int j=0; j<rowsCount; ++j){
				double drawVal,ncVal;
				BOOST_STATIC_ASSERT(colsCount<=unifsCount);
				usm.reset();
				dbars.initialize(initAr[j][0], initAr[j][1]);
				std::cout<<"Initialized with (x1,x2)=("<<
                    initAr[j][0]<<","<<initAr[j][1]<<")"<<std::endl;
				for(unsigned int i=0; i<colsCount; i++){
					//std::cout << "usm.it_distance()="
					//<< usm.it_distance() << std::endl;
					drawVal = dbars(usm);
					ncVal = dbars.total_unnormalized_cdf();
                   BOOST_ASSERT(drawVal < dbarsAr[j][i]+tolerance);
                   BOOST_ASSERT(drawVal > dbarsAr[j][i]-tolerance);
					if(!(i%report_every)){
					 std::cout
                        << "At i = "<< i+1
					     << ", normalizing constant = "
					     << ncVal
					     << " vs true value "
                        << limitingNc
                        << std::endl;
					}
				};
			};
		}catch(std::exception& e){
			std::cerr << e.what() << std::endl;
		    std::cout <<  "This throw is meant to illustrate the"
                << "!(delta>0.0) pitfall described in"
                << " approximation_impl::update_unnormalized_cdf_impl"
                << std::endl;
		};

		std::cout<<"<-"<<std::endl;
	};
}
}
