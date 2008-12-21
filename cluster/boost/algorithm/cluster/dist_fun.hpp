#if ! defined(BOOST_ALGORITHM_CLUSTER_DIST_FUN_HPP)
#define BOOST_ALGORITHM_CLUSTER_DIST_FUN_HPP

//  (C) Copyright Anders Sundman 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <functional>
#include <cmath>

#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

// This file is currently required to get Fusion and Lambda to 
// work together. 
#include <boost/algorithm/cluster/detail/lambda_result_of.hpp>
#include <boost/algorithm/cluster/detail/abs_diff.hpp>


namespace boost
{
namespace algorithm
{
namespace cluster
{

// 2-norm.
template<typename T1, typename T2>
double euclid_dist(T1 const & t1, T2 const & t2) 
{
  return std::sqrt
  ( 
    boost::fusion::fold
    ( 
      boost::fusion::transform
      (
        boost::fusion::transform(t1, t2, std::minus<double>()), 
        boost::bind(std::multiplies<double>(), _1, _1)
      ), 
      0,
      std::plus<double>()
    ) 
  );
}

// 2-norm.
template<typename T1, typename T2>
double euclid_dist_pretty(T1 const & t1, T2 const & t2) 
{
  using namespace boost::lambda;
  return std::sqrt
  ( 
    static_cast<double>
    (
      boost::fusion::fold
      ( 
        boost::fusion::transform(t1, t2, (_1 * _1)(_1 - _2)), 
        0,
        _1 + _2
      )
    )
  );
}


// 1-norm.
template<typename T1, typename T2>
double manhattan_dist(T1 const & t1, T2 const & t2) 
{
  using namespace boost::lambda;
    
  return boost::fusion::fold
  (
    boost::fusion::transform(t1, t2, detail::abs_diff<double>()), 
    0,
    _1 + _2
  );
}

} // End of namespace cluster.
} // End of namespace algorithm.
} // End of namespace boost.

#endif
