/////////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::distributions::students_t::random.hpp                 //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_STUDENTS_T_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_STUDENTS_T_RANDOM_HPP_ER_2009
#include <boost/math/distributions/students_t.hpp>
#include <boost/random/students_t.hpp> // sandbox/statistics/random
#include <boost/statistics/detail/distribution_toolkit/meta/random.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{
namespace meta{

    template<typename T,typename P>
    struct random< 
        boost::math::students_t_distribution<T,P> 
    >{
        typedef boost::math::students_t_distribution<T,P> dist_;
        typedef boost::random::students_t_distribution<T> type;
        
        static type make(const dist_& d){ 
            return type(d.degrees_of_freedom()); 
        }
    };

}// meta
}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif