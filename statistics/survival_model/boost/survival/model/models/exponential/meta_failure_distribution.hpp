///////////////////////////////////////////////////////////////////////////////
// survival::model::models::meta_failure_distribution.hpp                    //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SURVIVAL_MODEL_MODELS_META_FAILURE_DISTRIBUTION_HPP_ER_2009
#define BOOST_SURVIVAL_MODEL_MODELS_META_FAILURE_DISTRIBUTION_HPP_ER_2009
#include <cmath>
#include <boost/model/wrap/aggregate/model_covariate_parameter.hpp>
#include <boost/standard_distribution/distributions/exponential.hpp>
#include <boost/survival/data/meta/failure_distribution.hpp>
#include <boost/survival/model/models/exponential/model.hpp>

namespace boost{
namespace survival{
namespace data{
    
    template<typename T>
    struct meta_failure_distribution< survival::model::exponential::model<T> >{
        typedef survival::model::exponential::model<T> model_;
        typedef math::exponential_distribution<T> type;
        
        template<typename X,typename P>
        static type make(
            boost::model::model_covariate_parameter_<model_,X,P> mcp
        ){
            T lambda = model_::log_rate(
                mcp.covariate(),
                mcp.parameter()
            );
            lambda = exp( lambda );
            return type(
                lambda
            );
        }        
    };



}// data
}// survival
}// boost

#endif