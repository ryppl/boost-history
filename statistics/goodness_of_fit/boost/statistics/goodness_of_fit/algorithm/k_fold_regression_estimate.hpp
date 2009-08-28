//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::algorithm::k_fold_regression_estimate.hpp   //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_GOODNESS_OF_FIT_ALGORITHM_K_FOLD_REGRESSION_ESTIMATE_HPP_ER_2009
#define  BOOST_STATISTICS_GOODNESS_OF_FIT_ALGORITHM_K_FOLD_REGRESSION_ESTIMATE_HPP_ER_2009
#include <boost/statistics/goodness_of_fit/functional/call_regression_estimate.hpp> 
#include <boost/statistics/goodness_of_fit/algorithm/k_fold_estimate.hpp> 

namespace boost{
namespace statistics{
namespace goodness_of_fit{

    template<typename T,typename E,typename It>
    void regression_estimate(
        k_fold_data<T>& kfd, // T is a (x,y) tuple
        E estimator,
        It i
    ){

        return estimate<call_regression_estimate>(
            kfd,
            estimator,
            i
        );

    }
    
}// goodness_of_fit
}// statistics
}// boost

#endif