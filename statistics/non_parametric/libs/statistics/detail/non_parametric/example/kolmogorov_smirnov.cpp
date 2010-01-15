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
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/statistics/detail/non_parametric/empirical_distribution/frequency.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/frequency_adaptor.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/statistic.hpp>
#include <libs/statistics/detail/non_parametric/example/frequency_int.h>

void example_kolmogorov_smirnov(std::ostream& os)
{

	os << "-> example_kolmogorov_smirnov" << std::endl;

	// This example illustrates kolmogorov smirnov for a discrete distribution
    // (hence the presence of non-negligeable combos)

	namespace np = boost::statistics::detail::non_parametric;

	typedef double val_;
    typedef int int_;
	const val_ mean = 1;
	const int_ n = 10 * 10;

	typedef boost::math::poisson_distribution<> dist_;
    typedef boost::poisson_distribution<> random_;
    typedef boost::mt19937 urng_;
    typedef boost::variate_generator<urng_&,random_> vg_;
	typedef std::vector<val_> vals_;
	namespace ed = boost::statistics::detail::empirical_distribution;

    typedef ed::tag::frequency<false> tag_freq_;
    typedef ed::tag::frequency<true> tag_cum_freq_;
    
	typedef boost::accumulators::accumulator_set<
    	int_,
        boost::accumulators::stats<tag_freq_,tag_cum_freq_>
    > acc_;

	typedef boost::mpl::int_<0> k_x_;
	typedef boost::mpl::int_<1> k_cdf_;
	typedef boost::mpl::int_<2> k_cnt_;

	typedef np::kolmogorov_smirnov::statistic<val_,k_x_,k_cdf_,k_cnt_> ks_;
    typedef np::frequency_adaptor<k_x_,k_cdf_,k_cnt_,int_,val_> adaptor_;
    typedef adaptor_::type elem_;
    typedef std::vector<elem_> elems_;
    
	urng_ urng;
	vals_ random_sample;
	std::generate_n(
    	std::back_inserter(random_sample),
        n,
    	vg_(urng,random_(mean))
	);        
    
    dist_ dist(mean);
	elems_ adapted_sample;

	{
		acc_ acc = std::for_each(
    		boost::begin(random_sample),
        	boost::end(random_sample),
        	acc_()
    	);
		BOOST_AUTO(
    		tmp,
			adaptor_::call(acc,dist)
    	);
        // A physical copy is necessary because ks(b,e) sorts [b,e)
		std::copy(
        	boost::begin(tmp),
        	boost::end(tmp),
            std::back_inserter(adapted_sample)
        );
        int_ nn = 0;
        BOOST_FOREACH(const elem_& e,adapted_sample){
        	 nn += boost::fusion::at_key<k_cnt_>(e);
    	}
		BOOST_ASSERT(nn == n);
    }

    ks_ ks;
	ks(boost::begin(adapted_sample),boost::end(adapted_sample));

	os 
    	<< ks_::description_header << '('
//        << ks.description() // boost::format bug
        << ks.count() << ','
        << ks.value() << ')'
        << std::endl;	    

	os << "<-" << std::endl;
}