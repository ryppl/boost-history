//////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::aggregate::normalized_functions.hpp     //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_COMMON_DISTRIBUTION_CONCEPT_AGGREGATE_NORMALIZED_FUNCTIONS_HPP_ER_2009
#define BOOST_COMMON_DISTRIBUTION_CONCEPT_AGGREGATE_NORMALIZED_FUNCTIONS_HPP_ER_2009
#include <boost/statistics/detail/distribution_common/concept/unary/cdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/pdf.hpp>
#include <boost/statistics/detail/distribution_common/concept/unary/quantile.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace concept{

    // Why would this fail?
    // If D is in boost::math, you need to bring its functions into the 
    // scope of distribution_toolkit by 
    // a) including (or if necessary creating) the file, 
    // distribution_toolkit/distributions/D/is_math_distribution.hpp
    // where meta::is_math::distribution<D> specializes to mpl::bool_<true> 
    // b) including the relevant file in distribution_toolkit/fwd_math
    //template<typename D>
    //class NormalizedFunctions : 
    //    Cdf<D>, 
    //    Pdf<D>, 
    //    Quantile<D>
    //{
    //};

}// concept
}// distribution_toolkit
}// statistics
}// detail
}// boost

#endif
