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

#include "boost/bind.hpp"

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
                0., std::plus<double>()
            ) 
        );
}

}}}

#endif