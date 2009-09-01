///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::models::exponential::log_likelihood.hpp      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_MODEL_MODELS_EXPONENTIAL_LOG_LIKELIHOOD_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_MODEL_MODELS_EXPONENTIAL_LOG_LIKELIHOOD_HPP_ER_2009
#include <boost/statistics/survival/model/meta/model_data.hpp>
#include <boost/statistics/survival/model/models/exponential/model.hpp>
#include <boost/statistics/survival/model/models/exponential/detail/log_likelihood.hpp>

namespace boost{
namespace statistics{
namespace model{
    
    // Models Model (sandbox/statistics/model).
    //
    // Intentionally not in namespace survival
    template<typename T,typename X,typename B>
    T log_likelihood(
        typename survival::model::meta::model_data<
            T,
            survival::model::exponential::model<T>,
            X
        >::type md,
        const B& beta
    ){
        typedef survival::model::exponential::model<T> model_;
    
        T lr = model_::log_rate(
            md.covariate(),
            beta
        );
        return survival::model::exponential::detail::log_likelihood( 
            lr, 
            md.response() 
        );
    }

    // If B == X we need this overload, or else the compiler cannot find
    // the above definition
    template<typename T,typename X>
    T log_likelihood(
        typename survival::model::meta::model_data<
            T,
            survival::model::exponential::model<T>,
            X
        >::type md,
        const X& beta
    ){
        return log_likelihood<T,X,X>(
            md,
            beta
        );
    }

}// model
}// statistics
}// boost

#endif 