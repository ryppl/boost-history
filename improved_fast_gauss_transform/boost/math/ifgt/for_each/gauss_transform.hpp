//////////////////////////////////////////////////////////////////////////////
//  gauss_transform.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_IFGT_FOR_EACH_GAUSS_TRANSFORM_HPP_ER_2009
#define BOOST_IFGT_FOR_EACH_GAUSS_TRANSFORM_HPP_ER_2009
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/iterator/for_each_over_2_vector2matrix.hpp>
#include <boost/iterator/range_same.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
#include <boost/math/ifgt/functor/gauss_transform.hpp>
namespace boost{
namespace math{
namespace ifgt{

// Evaluates each target using a method and writes to a set of ranges
// Usage: for_each<M>(targets,ranges_out,e)
template<
    typename R0, typename R1,
    typename D
>
void for_each_gauss_transform(
    const R0& targets,//t[0][0],...,t[0][D],...,t[n-1][0],...,t[n-1][D]
    R1& ranges_out, //r[0][0],...,r[0][J-1],...,r[n-1][0],...,r[n-1][J-1]
    const crtp::evaluator<D>& e
){
    typedef meta::gauss_transform<> meta_t;
    return for_each_evaluate<meta_t>(
        targets,
        ranges_out,
        e
    );
}


}//ifgt
}//math
}//boost

#endif
