#ifndef BOOST_ALGORITHM_CLUSTER_DIST_FUNC_HPP
#define BOOST_ALGORITHM_CLUSTER_DIST_FUNC_HPP

//  (C) Copyright Anders Sundman 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <functional>
#include <cmath>

#include "boost/fusion/adapted.hpp"
#include "boost/fusion/algorithm.hpp"
#include "boost/lambda/lambda.hpp"
#include "boost/bind.hpp"

// This file is currently required to get Fusion and Lambda to 
// work together. 
#include "detail/lambda_result_of.hpp"


namespace boost { namespace algorithm { namespace cluster {

template<typename T1, typename T2>
double euclid_dist( const T1 & t1, const T2 & t2) 
{
    return std::sqrt
        ( 
            boost::fusion::fold
            ( 
                boost::fusion::transform
                (
                    boost::fusion::transform
                    (
                        t1, t2, std::minus<double>()
                    ), 
                    boost::bind(std::multiplies<double>(), _1, _1)
                ), 
                0, std::plus<double>()
            ) 
        );
}

template<typename T1, typename T2>
double euclid_dist_pretty( const T1 & t1, const T2 & t2) 
{
    using namespace boost::lambda;
    return std::sqrt
        ( 
            static_cast<double>(
                boost::fusion::fold
                ( 
                    boost::fusion::transform
                    (
                        t1, t2, (_1 * _1)(_1 - _2)
                    ), 
                    0, _1 + _2
                )
            )
        );
}

namespace impl {
    template<typename T>
    struct absdiff : std::binary_function<T, T, T>
    {
        T operator ()(const T & t1, const T & t2) const
        {
            return t1 < t2 ? t2 - t1 : t1 - t2;
        }
    };
}

template<typename T1, typename T2>
double manhattan_dist( const T1 & t1, const T2 & t2) 
{
    using namespace boost::lambda;
    
    return boost::fusion::fold
           ( 
                boost::fusion::transform
                (
                    t1, t2, impl::absdiff<double>()
                ), 
                0, _1 + _2
            );
}

}}}

#endif
