///////////////////////////////////////////////////////////////////////////////
// kolmogorov_smirnov_statistic.cpp     						     	     //
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

#include <boost/statistics/detail/non_parametric/empirical_distribution/kolmogorov_smirnov_statistic/value.hpp>
#include <boost/statistics/detail/non_parametric/empirical_distribution/kolmogorov_smirnov_statistic/series.hpp>

#include <libs/statistics/detail/non_parametric/example/frequency_int.h>

void example_kolmogorov_smirnov(
	std::ostream& os,
    double mean,
    long offset,
    long base,
    long first_p,
    long last_p
)
{

	os << "-> example_kolmogorov_smirnov" << std::endl;

	// This example illustrates kolmogorov smirnov for both a discrete
    // and a continuous distribution

    namespace ac = boost::accumulators;
    namespace ed = boost::statistics::detail::empirical_distribution;
	namespace ks = ed::kolmogorov_smirnov_statistic;
    typedef boost::mt19937 urng_;

    urng_ urng;

	{
		typedef boost::math::poisson_distribution<> dist_;
        typedef dist_::value_type val_;
    	typedef boost::poisson_distribution<> random_;
    	typedef boost::variate_generator<urng_&,random_> vg_;
        dist_ dist( mean );
		vg_ vg( urng, random_(mean) );

    	os << "poisson(" << mean << ')' << std::endl;
        
        typedef ks::series_data<val_> data_;
        
        ks::series( 
            dist, vg, 
            offset, base, first_p, last_p,
            std::ostream_iterator<data_>(os,"\n")
        );        

    }
	{
		typedef boost::math::normal_distribution<> dist_;
        typedef dist_::value_type val_;
    	typedef boost::poisson_distribution<> random_;
    	typedef boost::variate_generator<urng_&,random_> vg_;
        const val_ sd = 1.0;
        dist_ dist( mean, sd );
		vg_ vg( urng, random_(mean) );
    	os << "normal(" << mean << ',' << sd << ')' << std::endl;
        typedef ks::series_data<val_> data_;
        ks::series( 
            dist, vg, 
            offset, base, first_p, last_p,
            std::ostream_iterator<data_>(os,"\n")
        );        

    }
	os << "<-" << std::endl;
}
