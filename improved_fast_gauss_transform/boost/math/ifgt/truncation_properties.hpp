//////////////////////////////////////////////////////////////////////////////
// ifgt/truncation_properties.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_TRUNCATION_PROPERTIES_HPP_ER_2009
#define BOOST_MATH_IFGT_TRUNCATION_PROPERTIES_HPP_ER_2009
#include <cmath>
#include <boost/math/special_functions/factorials.hpp>
namespace boost{namespace math{namespace ifgt{

struct truncation_properties{
    public:
    //assumes weight = 1
    template<typename RealType>
    static RealType log_error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType target_to_center_distance
     );
    template<typename RealType>
    static RealType error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType target_to_center_distance
     );
    template<typename RealType>
    static RealType worst_target_to_center_distance(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance
     );
    template<typename RealType>
    static RealType worst_target_to_center_distance(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType cutoff_radius
     );
    template<typename RealType>
    static RealType max_log_error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType cutoff_radius
     );
     template<typename RealType>
     static RealType max_error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType cutoff_radius
    );

};

template<typename RealType>
RealType truncation_properties::log_error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType target_to_center_distance
){
        typedef RealType value_type;
        value_type p = max_degree;
        value_type h = bandwidth;
        value_type rx = source_to_center_distance;
        value_type ry = target_to_center_distance;
        value_type result =
           max_degree * (
            log(2)
            + log(rx)
            + log(ry)
            - 2 * log(h	)
           ) - log ( factorial<value_type>(p) );
        result -= std::pow( (rx - ry)/h, 2 );
        return result;
}

template<typename RealType>
RealType truncation_properties::error_bound(
    unsigned max_degree,
    RealType bandwidth,
    RealType source_to_center_distance,
    RealType target_to_center_distance
){
    return exp(log_error_bound(
        max_degree,
        bandwidth,
        source_to_center_distance,
        target_to_center_distance
    ));
}

template<typename RealType>
RealType
truncation_properties::worst_target_to_center_distance(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance
){
        typedef RealType value_type;
        value_type p = max_degree;
        value_type h = bandwidth;
        value_type rx = source_to_center_distance;
        value_type result
         = ( rx + sqrt(std::pow(rx,2) + 2 * p * std::pow(h,2)) )
            /((value_type)(2));
        return result;
}

template<typename RealType>
RealType
truncation_properties::worst_target_to_center_distance(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType cutoff_radius
){
    typedef RealType value_type;
    value_type result = worst_target_to_center_distance(
        max_degree, bandwidth, source_to_center_distance);

    return (result<cutoff_radius)? result : cutoff_radius;
}

template<typename RealType>
RealType truncation_properties::max_log_error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType cutoff_radius
){
    typedef RealType value_type;
    value_type wry =
        worst_target_to_center_distance(
            max_degree, bandwidth, source_to_center_distance, cutoff_radius);
    return log_error_bound(
        max_degree, bandwidth, source_to_center_distance, wry);
}

template<typename RealType>
RealType truncation_properties::max_error_bound(
        unsigned max_degree,
        RealType bandwidth,
        RealType source_to_center_distance,
        RealType cutoff_radius
){
    return exp(
        max_log_error_bound(
        max_degree, bandwidth, source_to_center_distance, cutoff_radius));
}

}}}
#endif
