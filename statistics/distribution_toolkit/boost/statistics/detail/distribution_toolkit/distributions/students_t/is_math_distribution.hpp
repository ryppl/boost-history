//////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::students_t::is_math_distribution.hpp  //
//                                                                              //
//  (C) Copyright 2009 Erwann Rogard                                            //
//  Use, modification and distribution are subject to the                       //
//  Boost Software License, Version 1.0. (See accompanying file                 //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)            //
//////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_STUDENTS_T_IS_MATH_DISTRIBUTION_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DISTRIBUTIONS_STUDENTS_T_IS_MATH_DISTRIBUTION_HPP_ER_2009
#include <boost/mpl/bool.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/is_scalar_distribution.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace meta{

    template<typename T,typename P>
    struct is_math_distribution< math::students_t_distribution<T,P> > 
        : mpl::bool_<true>{};

}// meta
}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif
