//////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::location_scale::quantile.hpp    //
//                                                                      //
//  (C) Copyright 2009 Erwann Rogard                                    //
//  Use, modification and distribution are subject to the               //
//  Boost Software License, Version 1.0. (See accompanying file         //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)    //
//////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_LOCATION_SCALE_QUANTILE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_LOCATION_SCALE_QUANTILE_HPP_ER_2009
#include <boost/concept/assert.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/location_scale.hpp>

namespace boost{
namespace statistics{
namespace detail{

    template<typename Z,typename T>
    T
    quantile(
        const distribution_toolkit::location_scale_distribution<Z>& d,
        const T& x
    ){
        T z = (x-d.mu())/d.sigma();
        return statistics::detail::quantile(
            d.z(),
            z
        );
    }

}// detail
}// statistics
}// boost

#endif