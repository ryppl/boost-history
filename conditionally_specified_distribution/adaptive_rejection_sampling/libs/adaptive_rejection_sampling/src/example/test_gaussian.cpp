/////////////////////////////////////////////////////////////////////////////
// test_gaussian.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <list>
#include <string>
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
        const double tolerance  = 1e-4;
		std::string str;
        str+= "n = %1% samples from N(0,1) using adaptive rejection";
        str+= "sampling vs a similar implementation in Mathematica.";
        str+= " Tolerance is set at %2%.";
        str+= " The uniform draws were Mathematica generated.";
        boost::format f(str); f % colsCount % tolerance;
        std::cout << f.str() << std::endl;

		Uniform_sampler_mathematica usm;
		typedef double value_type;
		typedef boost::adaptive_rejection_sampling
            ::standard_gaussian_evaluator<value_type>
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
				std::cout<<"Initialized with (x1,x2)=("<<
                    initAr[j][0]<<","<<initAr[j][1]<<") ";
				dbars.initialize(initAr[j][0], initAr[j][1]);
				for(unsigned int i=0; i<colsCount; i++){
				    //std::cout << "i=" << i << std::endl;
					drawVal = dbars(usm);
					ncVal = dbars.total_unnormalized_cdf();
					if(
                        (drawVal > dbarsAr[j][i]+tolerance)
                            ||
                        (drawVal < dbarsAr[j][i]-tolerance)
                   )
                   {
                       std::string str = "(drawVal = %1%)";
                       str+= "- (dbarsAr[j][i] = %2% ) < %3%";
                       boost::format f(str); f%drawVal%dbarsAr[j][i];
                       f % tolerance;
                       throw std::runtime_error(f.str());
                   }
				};
				std::cout << ": OK." << std::endl;
			};
		}catch(std::exception& e){
		    std::string str = e.what();
		    str+= " ";
		    str+= dbars.as_string();
			std::cerr << str << std::endl;
		};

		std::cout<<"<-"<<std::endl;
	};
}
}
