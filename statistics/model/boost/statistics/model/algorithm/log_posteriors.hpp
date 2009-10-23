///////////////////////////////////////////////////////////////////////////////
// statistics::model::algorithm::log_posteriors.hpp                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_ALGORITHM_LOG_POSTERIORS_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_ALGORITHM_LOG_POSTERIORS_HPP_ER_2009
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/statistics/model/wrap/aggregate/prior_model_dataset.hpp>
#include <boost/statistics/model/functional/log_posterior_evaluator.hpp>

namespace boost{ 
namespace statistics{
namespace model{   

// Evaluates the log-postetior for each parameter in [b_p,e_p)
// Also see log_posteriors2
template<
    typename T,typename D,typename M,typename Rx,typename Ry,typename ItP,
    typename ItLw
>
ItLw
log_posteriors(
    boost::statistics::model::prior_model_dataset_<D,M,Rx,Ry> pmd,
    ItP b_p,
    ItP e_p,
    ItLw o_lw
){
    typedef log_posterior_evaluator<T,D,M,Rx,Ry> eval_;
    return std::transform(
        b_p,
        e_p,
        o_lw,
        eval_(pmd)
    );
}

// log-pdf_Q --> log_pdf P + log_pdf L - log_pdf_Q
template<
    typename T,
    typename D,
    typename M,
    typename Rx,
    typename Ry,
    typename ItP,
    typename ItLw,
    typename ItLw2
>
ItLw
log_posteriors(
    prior_model_dataset_<D,M,Rx,Ry> pmd,
    ItP b_p,
    ItP e_p,
    ItLw b_lw,   
    ItLw2 o_lw   // lw <- (log_posterior - lw)
){
    typedef log_posterior_evaluator<T,D,M,Rx,Ry> eval_;
    eval_ e(pmd);
    return std::transform(
        b_p,    //1
        e_p,
        b_lw,   //2
        o_lw,
        boost::lambda::bind(
            e,
            boost::lambda::_1
        ) - boost::lambda::_2
    );
}

}// model
}// statistics
}// boost

#endif