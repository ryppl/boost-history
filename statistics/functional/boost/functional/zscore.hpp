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
#include <boost/functional/mean_var_accumulator.hpp>

namespace boost{
namespace functional{

template<typename T,typename A = typename mean_var_accumulator<T>::type>
class zscore{
    public:
        typedef T result_type;
        zscore(){}
        zscore(const A& a):a_(a){}
        template<typename T1>
        result_type operator()(const T1& x)const{
            T m = boost::accumulators::mean(this->a());
            T s = boost::accumulators::variance(this->a());
            s = std::sqrt(s);
            return (x - m)/s;
        }
        const A& a()const{ return this->a_; }
    private:
        A a_;
};


}// functional
}// boost

#endif
