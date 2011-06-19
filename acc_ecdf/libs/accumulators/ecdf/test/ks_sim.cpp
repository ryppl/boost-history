///////////////////////////////////////////////////////////////////////////////
// kolmogorov_smirnov_statistic.cpp                                          //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <iterator>
#include <string>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/geometric_series_range.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/kolmogorov_smirnov_simulator.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/mpl/int.hpp>
#include <boost/math/distributions/normal.hpp> 
#include <boost/math/distributions/poisson.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/random/normal_distribution.hpp> 
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/typeof/typeof.hpp>

#include <boost/range/algorithm/for_each.hpp>

#include <libs/accumulators/ecdf/test/ks_sim.h>

void test_ks_sim(
    std::ostream& os,
    double mean,
    long n,
    long factor,
    long size
)
{

//[test_ks_sim
    const std::string str = "sample size vs kolmogorov smirnov statisic:";

    // A series of Kolmogorov-Smirnov statistics, computed over an iid 
    // sample of increasing size (n). The reference distribution is that of
    // the random generator, so the series is expected to converge to zero.
    // Warning : numeric error may dominate for large n.
        
    namespace acc = boost::accumulators;
    namespace ecdf = acc::ecdf;
    namespace aux_ = ecdf::aux_;
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
        boost::transform(
            aux_::geometric_series_range(n, factor, size),
            std::ostream_iterator<boost::format>( os, "\n" ),
            aux_::kolmogorov_smirnov_simulator( vg, dist )
        );
        os << std::endl;
/* ouputs:
poison(1)
(1, 0.0803014)
(3, 0.0803014)
(7, 0.0821652)
(15, 0.0987872)
(31, 0.21963)
(63, 0.148457)
(127, 0.0979636)
(255, 0.0612491)
(511, 0.0136454)
(1023, 0.00715412)
(2047, 0.00534967)
(4095, 0.00548628)
(8191, 0.00858265)
(16383, 0.00287263)
(32767, 0.00364357)
(65535, 0.00236578)
(131071, 0.00147762)
(262143, 0.000797197)
(524287, 0.0005051)
(1048575, 0.000203309)*/

    }
    {
        typedef boost::math::normal_distribution<> dist_;
        typedef dist_::value_type val_;
        typedef boost::normal_distribution<> random_;
        typedef boost::variate_generator<urng_&, random_> vg_;
        const val_ sd = 1.0;
        dist_ dist( mean, sd );
        vg_ vg( urng, random_(mean) );
        
        os << "normal(" << mean << ',' << sd << ')' << std::endl;
        boost::transform(
            aux_::geometric_series_range(n, factor, size),
            std::ostream_iterator<boost::format>( os, "\n" ),
            aux_::kolmogorov_smirnov_simulator( vg, dist )
        );
        os << std::endl;
/*
normal(1,1)
(1, 0.514901)
(3, 0.23449)
(7, 0.177523)
(15, 0.106794)
(31, 0.119698)
(63, 0.0900238)
(127, 0.0938422)
(255, 0.045615)
(511, 0.0375595)
(1023, 0.0395167)
(2047, 0.0223799)
(4095, 0.0199377)
(8191, 0.00428302)
(16383, 0.00567374)
(32767, 0.00345214)
(65535, 0.00413189)
(131071, 0.00339832)
(262143, 0.00155582)
(524287, 0.00121594)
(1048575, 0.000613453)
*/
    }
//]
}
