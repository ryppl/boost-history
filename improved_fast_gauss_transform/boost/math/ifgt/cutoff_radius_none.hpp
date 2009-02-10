//////////////////////////////////////////////////////////////////////////////
// ifgt/cutoff_radius_non.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CUTOFF_RADIUS_NONE_HPP_ER_2009
#define BOOST_MATH_IFGT_CUTOFF_RADIUS_NONE_HPP_ER_2009
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>
#include <boost/utility/dont_care.hpp>

namespace boost{namespace math{namespace ifgt{
    // Models CutoffRadiusPolicy
    // This policy imposes no cutoff radius i.e. all clusters are used
    // \warning the cutoff squared may be nan
    template<typename RealType>
    struct cutoff_radius_none{
        typedef RealType    result_type;
        typedef RealType    first_argument_type;
        typedef RealType    second_argument_type;
        cutoff_radius_none(utility::dont_care){}
        cutoff_radius_none(){}
        template<typename Cluster>
        result_type operator()(const Cluster& cluster)const{
            static const result_type res
                = numeric::bounds<result_type>::highest();
            return res;
        }
    };
}}}
#endif
