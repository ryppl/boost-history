///////////////////////////////////////////////////////////////////////////////
// survival::data::random::failure_time.hpp                                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SURVIVAL_DATA_RANDOM_FAILURE_TIME_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_RANDOM_FAILURE_TIME_HPP_ER_2009
#include <boost/dist_random/include.hpp>
#include <boost/model/wrap/aggregate/model_covariate_parameter.hpp>
#include <boost/survival/data/meta/failure_distribution.hpp>

namespace boost{
namespace survival{
namespace data{
namespace random{

    // Model + Covariate + Parameter + URNG ---> failure time
    // 
    // This is a default implementaion that relies on meta::failure_random. It
    // can be overloaded by specializing on M.
    template<typename T,typename M,typename X,typename P,typename U>
    T failure_time(
        boost::model::model_covariate_parameter_<M,X,P> mcp,
        U& urng
    ); 

    // Implementation //

    template<typename T,typename M,typename X,typename P,typename U>
    T failure_time(
        boost::model::model_covariate_parameter_<M,X,P> mcp,
        U& urng
    ){
        return make_failure_random(mcp)(urng);
    }
 
}// random
}// data
}// survival
}// boost

#endif