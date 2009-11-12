//////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::fwd_math::range.hpp                                 	//
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DISTRIBUTION_TOOLKIT_FWD_MATH_RANGE_HPP_ER_2009
#define BOOST_DISTRIBUTION_TOOLKIT_FWD_MATH_RANGE_HPP_ER_2009
#include <boost/utility/enable_if.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_math_distribution.hpp>
#include <boost/statistics/detail/distribution_common/meta/result_of_support.hpp>

namespace boost{
namespace statistics{
namespace detail{

template<typename D>
typename lazy_enable_if<
	distribution::toolkit::meta::is_math_distribution<D>,
    distribution::toolkit::meta::result_of_support<D>
>
range(const D& dist)
{
	return boost::math::range(dist);	
}

}// detail
}// statistics
}// boost

#endif