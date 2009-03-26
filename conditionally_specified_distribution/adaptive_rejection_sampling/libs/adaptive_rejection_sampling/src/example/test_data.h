/////////////////////////////////////////////////////////////////////////////
// test_data.h
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_TEST_DATA_H_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_TEST_DATA_H_ER_2009

#include <vector>
#include <cstddef>
#include <boost/utility.hpp>
#include <iostream>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace libs{namespace adaptive_rejection_sampling{
	extern const double limitingNc;//sqrt(2*pi)
	const unsigned rowsCount=3;//2
	const unsigned colsCount=200;
	const unsigned unifsCount=500;
	const unsigned show_count=1; //show every show_count iteration

	extern const double dbarsAr[rowsCount][colsCount];
	extern const double dbarsAr_1[rowsCount][colsCount];
	extern const double initAr[rowsCount][2];
	extern const double unifsAr[unifsCount];

	///A test class
	class Uniform_sampler_mathematica :public boost::noncopyable{
	public:
		typedef double input_type;
		typedef double result_type;
		double min()const{return 0.0;};
		double max()const{return 1.0;};
		//(unsigned int* seed_):_seed(seed_){//forces a call to reset()
		Uniform_sampler_mathematica(){
			unifs.reserve(unifsCount);
			for(size_t i=0; i<unifsCount; ++i){
				unifs.push_back(unifsAr[i]);
			};
			it = unifs.begin();
		};
		unsigned long it_distance()const{return (it-unifs.begin());};
		double operator()(){
			double ret;
			if(it!=unifs.end()){ret = (*it); ++it;}else{
			    throw std::runtime_error(
                    "Uniform_sampler_mathematica: out of range");};
			return ret;
		};
        //not sure if this impl of reset models that of
		unsigned int reset(){it=unifs.begin(); return unifs.size();};
	private:
		std::vector<double> unifs;
		std::vector<double>::const_iterator it;
    };

}
}
#endif
