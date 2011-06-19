///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <vector>
#include <boost/mpl/int.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/check.hpp>
#include <boost/accumulators/statistics/ecdf/ordered_sample.hpp>
#include <boost/accumulators/statistics/ecdf/cumulative_count.hpp>
#include <boost/accumulators/statistics/ecdf/pdf.hpp>
#include <boost/accumulators/statistics/ecdf/cdf.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <libs/accumulators/ecdf/test/ecdf.h>

void test_ecdf()
{

//[test_ecdf
    typedef int sample_;  // sample_ x = 1;
    typedef double val_;  // val_ p = pdf( dist, x );
    typedef std::vector<sample_> samples_;

    namespace acc = boost::accumulators;
    namespace num = boost::numeric;
    
    samples_ samples; 
    const int n = 3;
    for(int i = 0; i < n; i++)
    {
        for(int j = i; j < n; j++)
        {
            samples.push_back( n-i );
        }
    }// 3, 3, 3, 2, 2, 1

    namespace ecdf = acc::ecdf;
    typedef ecdf::tag::cdf<val_> tag_cdf_;
    typedef ecdf::tag::count tag_pdf_;
    typedef acc::accumulator_set<
        sample_, 
        acc::stats<tag_pdf_, tag_cdf_> 
    > set_;

    set_ set = boost::for_each(
        samples, 
        set_() 
    );

    long sum = 0; 
    val_ cum_freq;
    for(int i = 0; i < n; i++)
    {

        typedef num::converter<int,std::size_t> converter_;
        BOOST_ACCUMULATORS_ECDF_CHECK( 
            (
                converter_::convert( 
                    ecdf::extract::count( set , i+1 ) 
                ) == i+1
            )
        );

        sum += i+1;
        BOOST_ACCUMULATORS_ECDF_CHECK( 
            (
                converter_::convert( 
                    ecdf::extract::cumulative_count( set, i+1 ) 
                ) == sum
            )
        );

        cum_freq = ecdf::extract::cdf<val_>( set, i+1 );

    }

    val_ one = 1.0;
    typedef num::bounds<val_> bounds_;
    val_ eps = bounds_::smallest(); 
    BOOST_ACCUMULATORS_ECDF_CHECK( fabs(cum_freq - one) < eps );
//]
    
}
