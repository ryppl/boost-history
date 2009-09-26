/////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::gamma::random.hpp                      //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_GAMMA_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_GAMMA_RANDOM_HPP_ER_2009
#include <boost/math/distributions/gamma.hpp>
#include <boost/random/gamma_distribution.hpp>
#include <boost/random/location_scale.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/random.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace meta{

    template<typename T,typename P>
    struct random< 
        math::gamma_distribution<T,P> 
    >{
        typedef math::gamma_distribution<T,P> dist_;
        typedef boost::gamma_distribution<T> z_;
        typedef boost::random::location_scale_distribution<z_> type;
        
        static type make(const dist_& d){ 
            return type(
                0,
                d.scale(),
                z_(d.shape())
            ); 
        }
    };
    
}// meta
}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif