//////////////////////////////////////////////////////////////////////////////
// accumulator::zscore.cpp                                                  //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <vector>
#include <boost/ref.hpp>
#include <boost/range.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/statistics/detail/accumulator/statistics/zscore.hpp>

void example_accumulator_zscore(std::ostream& os)
{

	os << "-> example_accumulator_zscore" << std::endl;

	typedef double val_;
    namespace stat = boost::statistics::detail;
    typedef stat::accumulator::tag::zscore tag_z_;
    
    typedef boost::accumulators::accumulator_set<
    	val_,
    	boost::accumulators::stats<tag_z_>
    > acc_;

	typedef boost::normal_distribution<val_> r_;
    typedef boost::mt19937 urng_;
	typedef boost::variate_generator<urng_&,r_> vg_;
	typedef std::vector<val_> vec_;

	const val_ m = -1;
    const val_ s = 2;
	r_ r(m,s);
    urng_ urng;    
	vg_ vg(urng,r);	
    vec_ vec;
    
	std::generate_n(std::back_inserter(vec),10000,vg);
    acc_ acc;
    acc = std::for_each(
    	boost::begin(vec),
        boost::end(vec),
       	acc
    );
    
	// should be close -1.96, 0, 1.96
    val_ a = stat::accumulator::extract::zscore(acc,m-1.96 * s); 	
    val_ b = stat::accumulator::extract::zscore(acc,m); 			
    val_ c = stat::accumulator::extract::zscore(acc,m+1.96 * s); 	
	os << "a = " << a << std::endl;
	os << "b = " << b << std::endl;
	os << "c = " << c << std::endl;
    
    os << std::endl;
    
}
