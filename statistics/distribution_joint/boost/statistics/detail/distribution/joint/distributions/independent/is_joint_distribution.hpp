//////////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::independent::is_joint_distribution.hpp   //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_INDEPENDENT_IS_JOINT_DISTRIBUTION_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_INDEPENDENT_IS_JOINT_DISTRIBUTION_HPP_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/statistics/detail/distribution/joint/meta/is_joint_distribution.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/independent/independent.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    template<typename R>
    struct is_joint_distribution< independent<R> > : mpl::bool_<true>{};
    
}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif
