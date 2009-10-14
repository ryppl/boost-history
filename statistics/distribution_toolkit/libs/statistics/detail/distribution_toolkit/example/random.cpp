//////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::example::random.cpp                               //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/statistics/detail/distribution_toolkit/distributions/chi_squared/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/exponential/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/gamma/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/students_t/include.hpp>

// Warning: location_scale<Z> comes after fwd_math because Z is defined in math::
#include <boost/statistics/detail/distribution_toolkit/fwd_math/cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/include.hpp>

#include <boost/statistics/detail/distribution_toolkit/test/random.hpp>

#include <libs/statistics/detail/distribution_toolkit/example/random.h>

void example_random(std::ostream& os)
{
    // For each distribution, dist, and the corresponding random engine returned
    // by make_random(dist), this example outputs a series of kolmogorov-
    // smirnov statistics. Convergence to zero is evidence that 
    // the sample agrees wit the cdf of dist

    os << "-> test_random " << std::endl;

    using namespace boost;
    using namespace boost::statistics;
    namespace stat = statistics::detail;
    namespace tk = stat::distribution::toolkit;
    namespace dc = stat::distribution::common;

    const unsigned n_loops = 6;
    const unsigned n1 = 1e1;
    const unsigned n2 = 1e1;
    
    // Types 
    typedef double val_;

    {
        typedef math::chi_squared_distribution<val_> dist_;
        const val_ df = 10;
        dist_ dist( df );

        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );

    }
    {
        typedef math::exponential_distribution<val_>     dist_;
        const val_ lambda = 1.0;
        dist_ dist( lambda );

        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );

    }
    { 
        typedef math::gamma_distribution<val_>            dist_;
        const val_ shape = 2.0;
        const val_ scale = 3.0;
        dist_ dist( shape, scale );
            
        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );
    }
    {
        typedef math::normal_distribution<val_>            dist_;
        const val_ m = 1.0;
        const val_ s = 2.0;
        dist_ dist( m, s );
            
        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );

    }
    {
        typedef math::students_t_distribution<val_>        dist_;
        const val_ df = 4.0;
        dist_ dist( df );
            
        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );
    }
    {
        typedef math::students_t_distribution<val_>         dist_z_;
        typedef tk::location_scale_distribution<dist_z_>    dist_; 
        const val_ df = 4.0;
        const val_ m = 1.0;
        const val_ s = 2.0;
        dist_z_ dist_z( df );
        dist_ dist(m, s, dist_z);
            
        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );
    }
    {
        typedef math::students_t_distribution<val_>         d0_;
        typedef tk::wrapper<d0_>                            dist_; 
        const val_ df = 4.0;
        d0_ d0( df );
        dist_ dist(d0);
            
        tk::test::random(
            dist,
            n_loops,  
            n1,       
            n2,      
            os
        );
    }

    os << "<-" << std::endl;
}
