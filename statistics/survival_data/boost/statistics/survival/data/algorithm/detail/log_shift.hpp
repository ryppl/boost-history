///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::algorithm::log_shift.hpp                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_DETAIL_LOG_SHIFT_HPP_ER_2009
#define BOOST_STATISTICS_SURVIVAL_DATA_ALGORITHM_DETAIL_LOG_SHIFT_HPP_ER_2009
#include <boost/math/special_functions/log1p.hpp>

namespace boost{
namespace statistics{
namespace survival{
namespace data{
namespace detail{
            
    template<typename T>
    T log_shift(
        const T& x,
        const T& t
    ){
        return log(x+t);
    }
            
    template<typename T>
    T logit_shift(
        const T& p,
        const T& t
    ){
        // log(p/(1-p)) = log(p)-log(1-p)
    
        return log_shift(p,t) - boost::math::log1p(t - p);
    }
            
            
}// detail
}// data
}// survival
}// statistics
}// boost

#endif