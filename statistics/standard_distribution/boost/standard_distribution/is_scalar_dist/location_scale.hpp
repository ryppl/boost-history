///////////////////////////////////////////////////////////////////////////////
// standard_distribution::is_scalar_dist::location_scale.hpp                 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STANDARD_DISTRIBUTION_IS_SCALAR_DIST_LOCATION_SCALE_ER_2009
#define BOOST_STANDARD_DISTRIBUTION_IS_SCALAR_DIST_LOCATION_SCALE_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/scalar_dist/meta/is_scalar_dist.hpp>
#include <boost/standard_distribution/transformation/location_scale.hpp>

namespace boost{
namespace math{

    template<typename D>
    struct is_scalar_dist< math::location_scale_distribution<D> > 
        : mpl::bool_<true> {};

}// math
}// boost

#endif