//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::algorithm::k_fold_regression_estimate.hpp   //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_GOODNESS_OF_FIT_ALGORITHM_K_FOLD_MARGINAL_ESTIMATE_HPP_ER_2009
#define  BOOST_STATISTICS_GOODNESS_OF_FIT_ALGORITHM_K_FOLD_MARGINAL_ESTIMATE_HPP_ER_2009
#include <boost/statistics/goodness_of_fit/functional/call_marginal_estimate.hpp> 
#include <boost/statistics/goodness_of_fit/algorithm/k_fold_estimate.hpp> 

namespace boost{
namespace statistics{
namespace goodness_of_fit{

    template<typename T,typename E,typename It>
    void marginal_estimate(
        k_fold_data<T>& kfd, // T is a variable, such as Y or X, but not (X,Y)
        E estimator,
        It i
    ){

        return estimate<call_marginal_estimate>(
            kfd,
            estimator,
            i
        );

    }
    
}// goodness_of_fit
}// statistics
}// boost

#endif