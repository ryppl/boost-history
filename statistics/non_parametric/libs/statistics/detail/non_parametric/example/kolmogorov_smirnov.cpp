///////////////////////////////////////////////////////////////////////////////
// non_parametric::kolmogorov_smirnov.cpp     						     	 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/mpl/int.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>

#include <boost/assign/std/vector.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <boost/math/distributions/poisson.hpp>
#include <boost/random/poisson_distribution.hpp>

#include <boost/math/distributions/normal.hpp> 
#include <boost/random/normal_distribution.hpp> 

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/check_convergence.hpp>
#include <libs/statistics/detail/non_parametric/example/frequency_int.h>

void example_kolmogorov_smirnov(
	std::ostream& os,
    double mean = 1.0,
    std::size_t n_loops = 1,
    std::size_t n_init = 10,
    std::size_t n_factor = 10 // n_init *= n_factor at each loop
)
{

	os << "-> example_kolmogorov_smirnov" << std::endl;

	// This example illustrates kolmogorov smirnov for both a discrete
    // and a continuous distribution

	namespace ks = boost::statistics::detail::kolmogorov_smirnov;

	typedef double val_;


    typedef boost::mt19937 urng_;
	typedef std::vector<val_> vals_;
	typedef ks::check_convergence<val_> check_;

    urng_ urng;
	check_ check;

    os 	<< "(sample size,statistic) :" << std::endl;
	{
		typedef boost::math::poisson_distribution<> dist_;
    	typedef boost::poisson_distribution<> random_;
    	typedef boost::variate_generator<urng_&,random_> vg_;
        dist_ dist(mean);
		vg_ vg(urng,random_(mean));

    	os << "poisson(" << mean << ')' << std::endl;
    	check(n_loops,n_init,n_factor,dist,vg,os);
    }
	{
		typedef boost::math::normal_distribution<> dist_;
		typedef boost::normal_distribution<> random_;
    	typedef boost::variate_generator<urng_&,random_> vg_;
		const val_ sd = 1.0;
		dist_ dist(mean,sd);
		vg_ vg(urng,random_(mean,sd));
    	os 	<< "normal(" << mean 
            << ','
            << sd
            << ')'
            << std::endl;
    	check(n_loops,n_init,n_factor,dist,vg,os);
	}

	os << "<-" << std::endl;
}