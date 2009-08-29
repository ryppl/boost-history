//////////////////////////////////////////////////////////////////////////////
// statistics::estimator_concept::trainable_estimator::concept.hpp          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef  BOOST_STATISTICS_ESTIMATOR_CONCEPT_TRAINABLE_ESTIMATOR_CONCEPT_HPP_ER_2009
#define  BOOST_STATISTICS_ESTIMATOR_CONCEPT_TRAINABLE_ESTIMATOR_CONCEPT_HPP_ER_2009
#include <boost/concept_check.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/statistics/estimator_concept/trainable/concept.hpp>
#include <boost/statistics/estimator_concept/estimator/concept.hpp>
#include <boost/statistics/estimator_concept/trainable_estimator/meta_input.hpp>

namespace boost{
namespace statistics{
namespace estimator_concept{

// X 
template <
    typename E,
    typename R, 
    typename X = typename estimate_input<typename range_value<R>::type>::type,
    typename B = mpl::empty_base
>
struct TrainableEstimator 
    : Estimator<
        E,
        X,
        Trainable<E,R,B> 
    >
{};

}// estimator_concept
}// statistics
}// boost

#endif