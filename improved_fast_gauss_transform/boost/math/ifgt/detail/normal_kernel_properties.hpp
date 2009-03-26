//////////////////////////////////////////////////////////////////////////////
// normal_kernel_properties.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_NORMAL_KERNEL_PROPERTIES_HPP_ER_2009
#define BOOST_MATH_IFGT_NORMAL_KERNEL_PROPERTIES_HPP_ER_2009
#include <cmath>
namespace boost{namespace math{namespace ifgt{
    // Normal kernel : exp(-(1/2)||(x-y)/sigma||^2) (2*pi*sigma^2)^(-d/2)
    // Note that the bandwidth is h = sqrt(2) * sigma
    template<unsigned SourceSz,typename RealType = double>
        struct normal_kernel_properties{
        typedef RealType                value_type;
        static const std::size_t source_size = SourceSz;
        static value_type
            pi(){
                static value_type
                val = 3.14159265358979323846264338327950288419716939937510;
                return val;
            }
        static value_type normalizing_constant(
            value_type sigma
        ){
            static value_type d = (value_type)(SourceSz);
            static value_type sqrt2pi = sqrt((value_type)(2)*pi());
            return std::pow(sigma*sqrt2pi,d);
        }
        static value_type normalizing_constant(
            value_type sigma,
            std::size_t n
        ){
            return (value_type)(n) * normalizing_constant(sigma);
        }
    };

}}}
#endif
