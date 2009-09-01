//////////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::models::exponential::detail::log_likelihood.hpp //
//                                                                              //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                   //
//  Software License, Version 1.0. (See accompanying file                       //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_MODEL_MODELS_EXPONENTIAL_DETAIL_LOG_LIKELIHOOD_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_MODEL_MODELS_EXPONENTIAL_DETAIL_LOG_LIKELIHOOD_HPP_ER_2009
#include <cmath>
#include <stdexcept>
#include <boost/format.hpp>
#include <boost/statistics/survival/data/data/event.hpp>

namespace boost{
namespace statistics{

namespace survival{
namespace model{
namespace exponential{
namespace detail{                
                
    template<typename T>
    T
    log_likelihood(
        const T& log_rate,
        const data::event<T>& e
    ){
        static const char* msg = 
            "survival::model::exponential::log_unnromalized_pdf(%1%,%2%)";
        typedef T value_type;
        value_type result = exp(log_rate);
        result *= (- e.time());
        try{
            if( boost::math::isinf(result) ){
                throw std::runtime_error("isinf(result)");
            }
            if( boost::math::isnan(result) ){
                throw std::runtime_error("isnan(result)");
            }
        }catch(std::exception ex){
            std::string str = msg;
            str += ex.what();
            format f(str); f % log_rate % e;
            throw std::runtime_error(
                f.str()
            );
        }
        if(e.failure()){
            result += log_rate;
        }
        return result;
    }
            
}// detail
}// exponential
}// model
}// survival
}// statistics
}// boost

#endif 