//////////////////////////////////////////////////////////////////////////////
// weight_front_insert_1_accumulate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FOR_EACH_WEIGHT_FRONT_INSERT_1_ACCUMULATE_HPP_ER_2009
#define BOOST_MATH_IFGT_FOR_EACH_WEIGHT_FRONT_INSERT_1_ACCUMULATE_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/math/ifgt/crtp/accumulator.hpp>
#include <boost/math/ifgt/for_each/accumulate.hpp>
#include <boost/math/ifgt/for_each/front_insert_1.hpp>

namespace boost{namespace math{namespace ifgt{

    template<typename R0, typename R1, typename D>
    void for_each_weight_front_insert_1_accumulate(
        const R0& sources,
        const R1& weights,
        crtp::accumulator<D>& d
    ){
       return
        for_each_accumulate(
            sources,
            for_each_front_insert_1<D::weight_size>(weights),
            d
        );
    }

}}}
#endif
