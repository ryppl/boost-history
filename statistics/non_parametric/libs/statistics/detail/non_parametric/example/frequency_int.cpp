///////////////////////////////////////////////////////////////////////////////
// accumulator::statistics::frequency_int.cpp     						     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>

#include <boost/assign/std/vector.hpp>
#include <boost/format.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <boost/statistics/detail/accumulator/statistics/frequency_int.hpp>

void example_frequency_int(std::ostream& os)
{

	os << "-> example_frequency_int" << std::endl;

	typedef int int_;
	typedef std::vector<int_> ints_;

	using namespace boost::assign;
    ints_ ints; 
    const int n = 3;
    for(unsigned i = 0; i<n; i++){
    	for(unsigned j = i; j<n; j++){
	    	ints.push_back(n-i);
        }
    } // 3, 3, 3, 2, 2, 1

	namespace accumulator = boost::statistics::detail::accumulator;
	
    typedef accumulator::tag::count_int<false> tag_count_;
    typedef accumulator::tag::frequency_int<false> tag_freq_;
    typedef accumulator::tag::frequency_int<true> tag_cum_freq_;
    typedef accumulator::tag::empirical_distribution_int tag_emp_;
    
	typedef boost::accumulators::accumulator_set<
    	int_,
        boost::accumulators::stats<tag_freq_,tag_cum_freq_>
    > acc_;

	acc_ acc = std::for_each(
    	boost::begin(ints),
        boost::end(ints),
        acc_()
    );

// TODO boost::format runtime bug
//	os << (boost::format("empirical {(pdf,cdf):i=1,...,%1%} : ")%n) << std::endl;
	os << "empirical {(pdf,cdf):i=1,...,n} : " << std::endl;
    int sum = 0;
    for(int i = 0; i<n; i++){
		BOOST_ASSERT( accumulator::extract::count_int<false>(acc,i+1) == i+1);
        sum += i+1;
		BOOST_ASSERT( accumulator::extract::count_int<true>(acc,i+1) == sum);

		boost::format f("(%1%,%2%)");
        double freq = accumulator::extract::frequency_int<false>(acc,i+1);
        double cum_freq = accumulator::extract::frequency_int<true>(acc,i+1);
        //f % freq % cum_freq;
		//os << f << std::endl;        
        os << '(' << freq << ',' << cum_freq << ')' << std::endl;
    }


	os << "<-" << std::endl;
}