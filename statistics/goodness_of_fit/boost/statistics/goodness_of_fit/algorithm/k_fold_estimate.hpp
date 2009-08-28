//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::algorithm::k_fold_estimate.hpp              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_GOODNESS_OF_FIT_ALGORITHM_K_FOLD_ESTIMATE_HPP_ER_2009
#define  BOOST_STATISTICS_GOODNESS_OF_FIT_ALGORITHM_K_FOLD_ESTIMATE_HPP_ER_2009
#include <string>
#include <stdexcept>
#include <boost/concept_check.hpp>
#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/statistics/estimator_concept/trainable_estimator/concept.hpp> 
#include <boost/statistics/goodness_of_fit/data/k_fold_data.hpp>

namespace boost{
namespace statistics{
namespace goodness_of_fit{ 

    template<typename C,typename T,typename E,typename It>
    void estimate(
        k_fold_data<T>& kfd,
        E estimator,
        It i
    ){
        typedef goodness_of_fit::k_fold_data<T>         k_fold_;
        typedef typename k_fold_::range_train_data_type range_; 
        BOOST_CONCEPT_ASSERT(( 
            estimator_concept::TrainableEstimator<E,range_> 
        ));
                

        C caller;
        while(kfd.j()<kfd.k()){
            range_ range(
                boost::begin( kfd.train_data() ),
                boost::end( kfd.train_data() )
            );

            train(
                estimator,
                range
            );
            BOOST_FOREACH(const T& t, kfd.test_data())
            {
                caller( 
                    estimator, 
                    t,
                    *i
                );
                ++i;
            }
            kfd.increment();
        }
    }
    
}// goodness_of_fit
}// statistics
}// boost

#endif
