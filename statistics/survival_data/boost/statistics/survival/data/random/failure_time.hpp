///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::random::failure_time.hpp                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_RANDOM_FAILURE_TIME_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_RANDOM_FAILURE_TIME_HPP_ER_2009
#include <boost/dist_random/include.hpp>
#include <boost/statistics/model/wrap/aggregate/model_covariate_parameter.hpp>
#include <boost/statistics/survival/data/meta/failure_distribution.hpp>
#include <boost/statistics/survival/model/concept/random_failure.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{
namespace random{

    // Model + Covariate + Parameter + URNG ---> failure time
    // 
    // This is a default implementaion that relies on meta::failure_random. It
    // can be overloaded by specializing on M.
    template<typename T,typename M,typename X,typename P,typename U>
    T failure_time(
        boost::statistics::model::model_covariate_parameter_<M,X,P> mcp,
        U& urng
    ); 

    // Implementation //

    template<typename T,typename M,typename X,typename P,typename U>
    T failure_time(
        boost::statistics::model::model_covariate_parameter_<M,X,P> mcp,
        U& urng
    ){
        BOOST_CONCEPT_ASSERT((
            survival::model::RandomFailure<T,M,X,P,U>
        ));
    
        return make_failure_random(mcp)(urng);
    }
 
}// random
}// data
}// survival
}// statistics
}// boost

#endif