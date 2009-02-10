//////////////////////////////////////////////////////////////////////////////
// ifgt/for_each_accumulate_rest_weights.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FOR_EACH_ACCUMULATE_REST_WEIGHTS_HPP_ER_2009
#define BOOST_MATH_IFGT_FOR_EACH_ACCUMULATE_REST_WEIGHTS_HPP_ER_2009
#include <boost/iterator/for_each_over_2_vector2matrix.hpp>
#include <boost/iterator/insert_element_every_n_step_iterator.hpp>
#include <boost/math/ifgt/for_each_accumulate.hpp>
#include <boost/math/ifgt/rest_weights_wrapper.hpp>

namespace boost{namespace math{namespace ifgt{

    template<typename R0, typename R1, typename Acc>
    void for_each(
        const R0& sources,
        const rest_weights_wrapper<R1>& rw,
        accumulator_base<Acc>& acc
    ){

        typedef typename result_of_make_range_insert_element_every_n_step<
            const R1>::type                                 weights_type;
        std::size_t n = Acc::weight_size - 1;
        BOOST_ASSERT(n>0);
        weights_type weights = make_range_insert_element_every_n_step(
            rw(),
            1.0,
            n
        );
       for_each(sources,weights,acc);
    }

}}}
#endif
