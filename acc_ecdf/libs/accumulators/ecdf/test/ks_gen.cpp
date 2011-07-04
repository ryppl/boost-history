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
#include <boost/accumulators/statistics/ecdf/aux_/incremental_simulator.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/kolmogorov_smirnov_generator.hpp>
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
#include <libs/accumulators/ecdf/test/ks_gen.h>

void test_ks_gen(
    std::ostream& os,
    double mean,
    long n,
    long factor,
    long size
)
{

    using namespace boost::accumulators;
    const std::string str = "sample size vs kolmogorov smirnov statisic:";
    // A series of Kolmogorov-Smirnov statistics, computed over an iid 
    // sample of increasing size (n). The reference distribution is that of
    // the random generator, so the series is expected to converge to zero.
    // Warning : numeric error may dominate for large n.

    {
//[test_ks_gen1
        typedef boost::math::poisson_distribution<> dist_;
        typedef dist_::value_type val_;
        typedef boost::poisson_distribution<> random_;
        typedef boost::mt19937 urng_;
        typedef boost::variate_generator<urng_&,random_> vg_;
        dist_ dist( mean );
        urng_ urng;
        vg_ vg( urng, random_( mean ) );

        os << "poisson(" << mean << ')' << std::endl;

        // If vg draws from dist, convergence to zero is expected
        std::generate_n(
            std::ostream_iterator<boost::format>( os, "\n" ), 
            size, 
            ecdf::aux_::make_kolmogorov_smirnov_generator( vg, dist )
        );
        
        os << std::endl;

/* 
ouputs:
poisson(1)
(1, 0.0803014)
(2, 0.132121)
(4, 0.117879)
(8, 0.0446986)
(16, 0.110759)
(32, 0.204509)
(64, 0.142009)
(128, 0.0951339)
(256, 0.0638839)
(512, 0.0131026)
(1024, 0.00740518)
(2048, 0.00516743)
(4096, 0.00557475)
(8192, 0.00862651)
(16384, 0.00288858)
(32768, 0.003621)
(65536, 0.00235452)
(131072, 0.00147481)
(262144, 0.000795791)
(524288, 0.000504595)*/

//]
    }
    {
//[test_ks_gen2
        typedef boost::math::normal_distribution<> dist_;
        typedef dist_::value_type val_;
        typedef boost::normal_distribution<> random_;
        typedef boost::mt19937 urng_;
        typedef boost::variate_generator<urng_&, random_> vg_;
        const val_ sd = 1.0;
        dist_ dist( mean, sd );
        urng_ urng;
        vg_ vg( urng, random_( mean ) );
        
        os << "normal(" << mean << ',' << sd << ')' << std::endl;
        // If vg draws from dist, convergence to zero is expected
        std::generate_n(
            std::ostream_iterator<boost::format>( os, "\n" ), 
            size, 
            ecdf::aux_::make_kolmogorov_smirnov_generator( vg, dist )
        );
        os << std::endl;
/*
normal(1,1)
(1, 0.415493)
(2, 0.415493)
(4, 0.189905)
(8, 0.192415)
(16, 0.102303)
(32, 0.0986652)
(64, 0.0728838)
(128, 0.0572596)
(256, 0.0362369)
(512, 0.034244)
(1024, 0.0172237)
(2048, 0.0143129)
(4096, 0.0174088)
(8192, 0.0108239)
(16384, 0.00705805)
(32768, 0.00438796)
(65536, 0.00198669)
(131072, 0.00244563)
(262144, 0.00122075)
(524288, 0.00111729)*/
//]
    }
}
