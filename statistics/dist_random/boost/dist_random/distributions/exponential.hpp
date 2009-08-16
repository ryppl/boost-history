//////////////////////////////////////////////////////////////////////////////
// dist_random::distributions::exponential.hpp                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DIST_RANDOM_DISTRIBUTIONS_EXPONENTIAL_HPP_ER_2009
#define BOOST_DIST_RANDOM_DISTRIBUTIONS_EXPONENTIAL_HPP_ER_2009
#include <boost/standard_distribution/distributions/exponential.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/dist_random/meta/dist_random.hpp>

namespace boost{

    template<typename T,typename P>
    struct dist_random< 
        boost::math::exponential_distribution<T,P> 
    >{
        typedef boost::math::exponential_distribution<T> dist_;
        typedef boost::exponential_distribution<T> type;
        
        static type make(const dist_& d){ return type(d.lambda()); }
    };
    
}// boost

#endif