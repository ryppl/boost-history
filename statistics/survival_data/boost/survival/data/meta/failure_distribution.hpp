///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::meta::failure_distribution.hpp                //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_META_FAILURE_DISTRIBUTION_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_META_FAILURE_DISTRIBUTION_HPP_ER_2009
#include <boost/statistics/model/wrap/aggregate/model_covariate_parameter.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{

    // Model + Covariate + Parameter ---> failure time distribution (such as
    // defined in boost/math/distributions)
    //
    // These are only declarations. The implementation must specialize on M.
    template<typename M> 
    struct meta_failure_distribution{
        // typedef ... type
    };

    template<typename M,typename X,typename P>
    typename meta_failure_distribution<M>::type
    make_failure_distribution(
        boost::statistics::model::model_covariate_parameter_<M,X,P> mcp
    ){
        typedef meta_failure_distribution<M> meta_;
        return meta_::make(
            mcp
        );
    }

}// data
}// survival
}// statistics
}// boost

#endif