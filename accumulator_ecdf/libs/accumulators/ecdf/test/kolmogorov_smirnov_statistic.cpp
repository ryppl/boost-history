///////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                         //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/check.hpp>
#include <boost/accumulators/statistics/ecdf/cdf.hpp>
#include <boost/accumulators/statistics/ecdf/kolmogorov_smirnov_statistic.hpp>
#include <boost/math/distributions/poisson.hpp>
#include <libs/accumulators/ecdf/test/kolmogorov_smirnov_statistic.h>

void test_ecdf_kolmogorov_smirnov_statistic()
{
    using namespace boost;
    using namespace accumulators;

//[test_ecdf_kolmogorov_smirnov_statistic
    typedef double float_;
    boost::math::poisson_distribution<> dist( 1.0 );
    accumulator_set<
        int, 
        stats< 
            ecdf::tag::cdf<float_>,
            ecdf::tag::kolmogorov_smirnov_statistic 
        > 
    > acc;

    typedef numeric::bounds<float_> bounds_;
    typedef numeric::converter<std::size_t, float_> converter_;

    // Layout is chosen for its clarify, but there is no need to assume a
    // particular ordering
    acc( 1 );
    acc( 2 ); acc( 2 ); 
    acc( 3 ); acc( 3 ); acc( 3 ); 

    float_ 
        benchmark = bounds_::smallest(), 
        eps = bounds_::smallest(),
        max_diff;

    for(std::size_t i = 1; i < 4; i++ )
    {
        max_diff = fabs( math::cdf( dist, i ) - ecdf::cdf<float_>( acc, i ) );
        benchmark = ( benchmark < max_diff ) ? max_diff : benchmark ;
    }
    float_ result =  ecdf::kolmogorov_smirnov_statistic<float_>( acc, dist );
    BOOST_ACCUMULATORS_ECDF_CHECK( fabs( result - benchmark ) < eps );

//]
    
}