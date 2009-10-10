//////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::iid::is_joint.hpp                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_IS_JOINT_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_IS_JOINT_HPP_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/statistics/detail/distribution/joint/meta/is_joint_distribution.hpp>
#include <boost/statistics/detail/distribution/joint/distributions/iid/iid.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    template<typename D>
    struct is_joint_distribution< iid<D> > : mpl::bool_<true>{};
    
}// joint
}// distribution
}// detail
}// statistics
}// boost

#endif
