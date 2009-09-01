///////////////////////////////////////////////////////////////////////////////
// model::algorithm::log_likelihood.hpp                                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_MODEL_ALGORITHM_LOG_LIKELIHOOD_HPP_ER_2009
#define BOOST_STATISTICS_MODEL_ALGORITHM_LOG_LIKELIHOOD_HPP_ER_2009
#include <boost/binary_op/algorithm/for_each.hpp>
#include <boost/statistics/model/wrap/aggregate/model_parameter.hpp>
#include <boost/statistics/model/functional/log_likelihood_accumulator.hpp>

namespace boost{ 
namespace statistics{
namespace model{        

    // Evaluates the likelihood at mp.parameter(), given model mp.model(),
    // by summing all contributions from a dataset i.e. a
    // sequence of covariates, [b_x,e_x), and reponses (starting at b_y)
    template<typename T,typename M,typename P,typename ItX,typename ItE>
    T
    log_likelihood(
        model_parameter_<M,P> mp,
        ItX b_x,
        ItX e_x,
        ItE b_y
    ){
        typedef log_likelihood_accumulator<T,M,P> acc_;
        acc_ acc(mp);
        return boost::binary_op::for_each<acc_>(
            b_x,
            e_x,
            b_y,
            acc
        ).value();
    }

    
}// model
}// statistics
}// boost

#endif