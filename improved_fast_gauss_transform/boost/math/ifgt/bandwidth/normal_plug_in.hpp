//////////////////////////////////////////////////////////////////////////////
// normal_plug_in.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_BANDWIDTH_NORMAL_PLUG_IN_HPP_ER_2009
#define BOOST_MATH_IFGT_BANDWIDTH_NORMAL_PLUG_IN_HPP_ER_2009
#include <cmath>
namespace boost{
namespace math{
namespace ifgt{

// \warning the sart(2)*sigma adjustmnent is needed so that
// the kernel is exp(-||(x-y)/h||^2) = exp(-(1/2)||(x-y)/sigma||^2)
template<std::size_t Dimension, typename RealType = double>
struct bandwidth_normal_plug_in{
    typedef RealType value_type;

    //Minimizes statistical error for density estimation under normality
    static value_type find(
        value_type sigma,
        std::size_t count
    );
    ///Assumes sigma = 1;
    static value_type find(
        std::size_t count);

};

template<std::size_t Dimension, typename RealType>
typename bandwidth_normal_plug_in<Dimension,RealType>::value_type
bandwidth_normal_plug_in<Dimension,RealType>::find(
    value_type sigma, std::size_t count){
    static const value_type sqrt2 = sqrt(2.0);
    static const value_type d = (value_type)(Dimension);
    static const value_type e = -(value_type)(1)/((value_type)(d+4));
    value_type n = (value_type)(count);
    value_type a =  (n * (value_type)(2+d))/ ((value_type)(4));
    return sigma * std::pow( a, e ) * sqrt2;
}

template<std::size_t Dimension, typename RealType>
typename bandwidth_normal_plug_in<Dimension,RealType>::value_type
bandwidth_normal_plug_in<Dimension,RealType>::find(
std::size_t count){
    return find(
        (value_type)(1),
        count);
}

}
}
}
#endif
