//////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::fwd_math::cdf.hpp                                      //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_COMMON_DISTRIBUTION_FWD_MATH_CDF_HPP_ER_2009
#define BOOST_COMMON_DISTRIBUTION_FWD_MATH_CDF_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_math_distribution.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
    
    template<typename D>
    typename lazy_enable_if<
        distribution::toolkit::meta::is_math_distribution<D>,
        distribution::common::meta::value<D>
    >::type
    cdf(    
        const D& dist,
        const typename distribution::common::meta::value<D>::type& x
    )
    {
        return boost::math::cdf(dist,x);
    }


}// detail
}// statistics
}// boost 

#endif