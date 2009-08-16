//////////////////////////////////////////////////////////////////////////////
// functional::zscore.hpp                                                   //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_FUNCTIONAL_ZSCORE_HPP_ER_2009           
#define BOOST_FUNCTIONAL_ZSCORE_HPP_ER_2009 
#include <cmath>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

namespace boost{
namespace functional{

template<typename T>
struct zscore{
    typedef T result_type;
    zscore(){}
    template<typename T1>
    result_type operator()(const A& a, const T1& x)const{
        T m = static_cast<T>(boost::accumulators::mean(a));
        T s = static_cast<T>(boost::accumulators::variance(a));
        s = std::sqrt(s);
        return (x - m)/s;
    }
};


}// functional
}// boost

#endif
