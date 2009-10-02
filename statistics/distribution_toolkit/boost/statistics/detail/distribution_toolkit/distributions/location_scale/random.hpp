/////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::distributions::location_scale::random.hpp             //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_LOCATION_SCALE_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_LOCATION_SCALE_RANDOM_HPP_ER_2009
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/location_scale.hpp>
#include <boost/random/location_scale.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/random.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

namespace meta{


    template<typename Z>
    struct random< 
        location_scale_distribution<Z> 
    >{
        typedef location_scale_distribution<Z> dist_;
        typedef typename meta::random<Z>::type z_;
        typedef boost::random::location_scale_distribution<z_> type;
        
        static type make(const dist_& d){ 
            return type(
                d.mu(),
                d.sigma(),
                make_random(d.z())
            ); 
        }
    };

    
}// meta
}// distribution
}// toolkit

}// detail
}// statistics
}// boost

#endif