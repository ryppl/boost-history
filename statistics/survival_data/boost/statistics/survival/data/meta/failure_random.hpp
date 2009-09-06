///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::meta::failure_random.hpp                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_META_FAILURE_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_META_FAILURE_RANDOM_HPP_ER_2009
#include <boost/dist_random/include.hpp>
#include <boost/statistics/model/wrap/aggregate/model_covariate_parameter.hpp>
#include <boost/statistics/survival/data/meta/failure_distribution.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

    // Model + Covariate + Parameter --->  RandomDistribution (of failure time)
    //
    // These are default implementations that can be overloaded with a 
    // specialization on M

    template<typename M> 
    struct meta_failure_random{
        typedef meta_failure_distribution<M>                        map1_;
        typedef typename map1_::type                                dist_;
        typedef boost::dist_random<dist_>                           map2_;
        typedef typename map2_::type                                type;
    };

    template<typename M,typename X,typename P>
    typename meta_failure_random<M>::type 
    make_failure_random(
        boost::statistics::model::model_covariate_parameter_<M,X,P>
    );

    // Implementation //

    template<typename M,typename X,typename P>
    typename meta_failure_random<M>::type 
    make_failure_random(
        boost::statistics::model::model_covariate_parameter_<M,X,P> mcp
    ){
        typedef meta_failure_random<M>                              map_;
        typedef typename map_::map2_                                map2_;
        return map2_::make(
            make_failure_distribution(mcp) 
        );
    }

}// data
}// survival
}// statistics
}// boost

#endif