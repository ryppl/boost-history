///////////////////////////////////////////////////////////////////////////////
// nn1::algorithm::min_element.hpp                                           //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_NN1_ALGORITHM_MIN_ELEMENT_HPP_ER_2009
#define BOOST_NN1_ALGORITHM_MIN_ELEMENT_HPP_ER_2009
#include <limits>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>
#include <iostream>

namespace boost{
namespace nn1{

// This is a rewrite of std::min_element with one additional argument, m, which
// is assigned the min value of *i over [first,last).
//
// This saves one dereferencing operation compared with *std::min_element(
// first,last) which may be worthwhile if first and last are obtained through
// costly iterator adaptors.

template <typename It,typename V>
It min_element(It first, It last,V& min_value) {
    typedef typename iterator_value<It>::type value_type;

    // TODO see about enable_if/disable_if
    static bool has_inf = std::numeric_limits<V>::has_infinity;
    static V inf_if = std::numeric_limits<V>::infinity(); 
    static V highest = boost::numeric::bounds<V>::highest();
    static V inf_ = has_inf? inf_if : highest;
    
    // TODO concept check
    // 1) It is ForwardIterator
    // 2) value_type < V comparable
    
    min_value = inf_;
    value_type value;
    It result = first;
    while (first != last){
        value = (*first);
        if (value < min_value){
            min_value = value;
            result = first;
        }

        ++first;
    }
    return result;
}


}//algorithm
}//boost
#endif