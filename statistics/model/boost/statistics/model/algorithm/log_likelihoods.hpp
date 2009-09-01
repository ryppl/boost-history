///////////////////////////////////////////////////////////////////////////////
// model::algorithm::log_likelihoods.hpp                                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_ALGORITHM_LOG_LIKELIHOODS_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_ALGORITHM_LOG_LIKELIHOODS_HPP_ER_2009
#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/statistics/model/wrap/aggregate/model_dataset.hpp>
#include <boost/statistics/model/functional/log_likelihood_evaluator.hpp>

namespace boost{ 
namespace statistics{
namespace model{   

// Evaluates the log-likelihood for each parameter in [b_p,e_p), given 
// model and data md
template<typename T,
    typename M,typename Rx,typename Ry,typename ItP,typename ItLw>
ItLw log_likelihoods(
    model_dataset_<M,Rx,Ry> md,
    ItP b_p,
    ItP e_p,
    ItLw o_lw
){
    typedef log_likelihood_evaluator<T,M,Rx,Ry> eval_;
    return std::transform(
        b_p,
        e_p,
        o_lw,
        eval_(md)
    );
}

// Maps the input log-pdf to the ratio-log-pdf
template<
    typename T,
    typename M,
    typename Rx,
    typename Ry,
    typename ItP,
    typename ItLw
>
ItLw
log_likelihoods(
    model_dataset_<M,Rx,Ry> md,
    ItP b_p,
    ItP e_p,
    ItLw b_lw,   
    ItLw o_lw   // lw <- (log_likelihod - lw)
){
    typedef log_likelihood_evaluator<T,M,Rx,Ry> eval_;
    eval_ e(md);
    return std::transform(
        b_p,
        e_p,
        b_lw,
        o_lw,
        boost::lambda::bind(e,boost::lambda::_2) - boost::lambda::_1
    );
}


}// model
}// statistics
}// boost

#endif