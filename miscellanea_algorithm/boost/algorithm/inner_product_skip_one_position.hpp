//////////////////////////////////////////////////////////////////////////////
// inner_product_skip_one_position.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ALGORITHM_INNER_PRODUCT_SKIP_ONE_POSITION_HPP_ER_2009
#define BOOST_ALGORITHM_INNER_PRODUCT_SKIP_ONE_POSITION_HPP_ER_2009
#include <functional>
#include <numeric>
#include <stdexcept>
#include <boost/range/value_type.hpp>
#include <boost/iterator/skip_one_step_iterator.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assert.hpp>

namespace boost{

    template<typename R0,typename R1,typename BinaryOp1,typename BinaryOp2>
    typename range_value<const R0>::type
    inner_product_skip_one_position(
        const R0& range0,
        const R1& range1,
        std::size_t position,
        typename range_value<const R0>::type init,
        const BinaryOp1& binary_op1,
        const BinaryOp2& binary_op2
    ){
        typedef typename range_value<const R0>::type value_type;
        typedef typename range_value<const R1>::type value1_type;

        BOOST_MPL_ASSERT( (is_same<value_type,value1_type>) );
        if(size(range0)!=size(range1)){
            throw std::runtime_error(
                "inner_product_skip_one_position size(range0)!=size(range1)"
            );
        }
        if(!((std::size_t)(size(range0))>position)){
            throw std::runtime_error(
                "inner_product_skip_one_position !(size(range0)>position)");
        }

        typedef typename range_iterator<const R0>::type        iter0_type;
        typedef typename range_iterator<const R1>::type        iter1_type;
        typedef skip_one_step_iterator<iter0_type>          skip_iter0_type;
        typedef skip_one_step_iterator<iter1_type>          skip_iter1_type;

        iter0_type skip0 = begin(range0); std::advance(skip0,position);
        iter1_type skip1 = begin(range1); std::advance(skip1,position);

        skip_iter0_type b0
            = make_skip_one_step_begin_iterator(begin(range0),skip0);
        skip_iter0_type e0
            = make_skip_one_step_end_iterator(end(range0),skip0);
        skip_iter1_type b1
            = make_skip_one_step_begin_iterator(begin(range1),skip1);

        return std::inner_product(
            b0,
            e0,
            b1,
            init,
            binary_op1,
            binary_op2
        );
    };

    template<typename R0,typename R1>
    typename range_value<const R0>::type
    inner_product_skip_one_position(
        const R0& range0,
        const R1& range1,
        std::size_t position,
        typename range_value<const R0>::type init
    ){
        typedef typename range_value<const R0>::type   value_type;
        typedef std::plus<value_type>                   plus_type;
        typedef std::multiplies<value_type>             multiplies_type;

        return inner_product_skip_one_position<
            R0,
            R1,
            plus_type,
            multiplies_type
        >(
            range0,
            range1,
            position,
            init,
            plus_type(),
            multiplies_type()
        );
    };
}
#endif
