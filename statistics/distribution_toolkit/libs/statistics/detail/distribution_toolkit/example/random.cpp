//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::example::random.cpp                                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iterator>
#include <ext/algorithm>
#include <boost/range.hpp>
#include <boost/static_assert.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/statistics/detail/distribution_toolkit/distributions/chi_squared/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/exponential/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/gamma/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/students_t/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/include.hpp>

#include <boost/statistics/detail/distribution_toolkit/random/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/data/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/unscope/os.hpp>

#include <libs/statistics/detail/distribution_toolkit/example/random.h>

void example_random(std::ostream& os){

    os << "example_random -> " << std::endl;
    using namespace boost;
    using namespace statistics;
    namespace tk  = statistics::detail::distribution_toolkit;
    
    // Types 
    typedef double                                          val_;
    typedef mt19937                                         urng_;

    typedef tk::sample_cdf<val_>                            sample_cdf_;
    typedef sample_cdf_::size_type                          size_;

    // Constants
    const unsigned n_loops  = 6;
    const unsigned n1        = 1e1;
    const unsigned n2        = 1e1;
    
    urng_ urng;

    sample_cdf_ sc;
    os << "kolmogorov statistic for : " << std::endl; 
    {
        typedef math::chi_squared_distribution<val_>            dist_;
        const val_ df           = 10;
        dist_ dist( df );

            
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        sc.clear();
        while(i<n_loops){
            sc.reserve(sc.size()+n_draws);
            generate2_n(sc,n_draws,urng,dist);
            os << sc << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
    {
        typedef math::exponential_distribution<val_>            dist_;
        const val_ lambda = 1.0;
        dist_ dist( lambda );
            
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        sc.clear();
        while(i<n_loops){
            sc.reserve(sc.size()+n_draws);
            generate2_n(sc,n_draws,urng,dist);
            os << sc << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
    { 
        typedef math::gamma_distribution<val_>            dist_;
        const val_ shape = 1.0;
        dist_ dist( shape );
            
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        sc.clear();
        while(i<n_loops){
            sc.reserve(sc.size()+n_draws);
            generate2_n(sc,n_draws,urng,dist);
            os << sc << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
    {
        typedef math::normal_distribution<val_>            dist_;
        const val_ m = 1.0;
        const val_ s = 2.0;
        dist_ dist( m, s );
            
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        sc.clear();
        while(i<n_loops){
            sc.reserve(sc.size()+n_draws);
            generate2_n(sc,n_draws,urng,dist);
            os << sc << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
    {
        typedef math::students_t_distribution<val_>        dist_;
        const val_ df = 4.0;
        dist_ dist( df );
            
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        sc.clear();
        while(i<n_loops){
            sc.reserve(sc.size()+n_draws);
            generate2_n(sc,n_draws,urng,dist);
            os << sc << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
    {
        typedef math::students_t_distribution<val_>         dist_z_;
        typedef tk::location_scale_distribution<dist_z_>    dist_; 
        const val_ df = 4.0;
        const val_ m = 1.0;
        const val_ s = 2.0;
        dist_z_ dist_z( df );
        dist_ dist(m, s, dist_z);
            
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        sc.clear();
        while(i<n_loops){
            sc.reserve(sc.size()+n_draws);
            generate2_n(sc,n_draws,urng,dist);
            os << sc << std::endl;
            ++i;
            n_draws *= n2;
        }
    }

    os << "<-" << std::endl;

}
