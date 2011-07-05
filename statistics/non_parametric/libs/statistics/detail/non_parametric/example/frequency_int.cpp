///////////////////////////////////////////////////////////////////////////////
// frequency_int.cpp     						                             //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <vector>
#include <algorithm>

#include <boost/mpl/int.hpp>

#include <boost/numeric/conversion/bounds.hpp>

#include <boost/assign/std/vector.hpp>
#include <boost/format.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <boost/statistics/detail/non_parametric/empirical_distribution/ordered_sample.hpp>
#include <boost/statistics/detail/non_parametric/empirical_distribution/cumulative_count.hpp>
#include <boost/statistics/detail/non_parametric/empirical_distribution/cdf.hpp>
#include <boost/statistics/detail/non_parametric/empirical_distribution/pdf.hpp>

#include <libs/statistics/detail/non_parametric/example/frequency_int.h>

void example_frequency_int(std::ostream& os)
{

	os << "-> example_frequency_int" << std::endl;

	typedef int sample_;	// sample_ x = 1;
    typedef double val_; 	// val_ p = pdf(dist,x);
	typedef std::vector<sample_> samples_;

	using namespace boost::assign;
    samples_ samples; 
    const int n = 3;
    for(int i = 0; i<n; i++){
    	for(int j = i; j<n; j++){
	    	samples.push_back(n-i);
        }
    } // 3, 3, 3, 2, 2, 1

	namespace ed = boost::statistics::detail::empirical_distribution;
    typedef ed::tag::cdf<val_> tag_cdf_;
    typedef ed::tag::pdf<val_> tag_pdf_;
    typedef boost::accumulators::accumulator_set<
        sample_,boost::accumulators::stats<tag_pdf_,tag_cdf_> > acc_;

	acc_ acc = std::for_each(
    	boost::begin( samples ),
        boost::end( samples ),
        acc_()
    );

	os 
    	<< (boost::format("empirical {(pdf,cdf):i=1,...,%1%} : ")%n).str() 
        << std::endl;
    int sum = 0;
    val_ freq, cum_freq;
    for(int i = 0; i < n; i++){
		BOOST_ASSERT( ed::extract::count( acc, i+1 ) == i+1);
        sum += i+1;
		BOOST_ASSERT( ed::extract::cumulative_count( acc, i+1 ) == sum);

        cum_freq = ed::extract::cdf<val_>( acc, i+1 );
        freq = ed::extract::pdf<val_>( acc, i+1 );
        os << '(' << freq << ',' << cum_freq << ')' << std::endl;
    }

    typedef boost::numeric::bounds<val_> bounds_;
    val_ eps = bounds_::smallest();
    val_ one = static_cast<val_>( 1 );
    
    BOOST_ASSERT( fabs( one - cum_freq ) < eps );

	os << "<-" << std::endl;
}
