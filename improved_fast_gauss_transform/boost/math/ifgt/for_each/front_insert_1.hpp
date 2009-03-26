//////////////////////////////////////////////////////////////////////////////
// front_insert_1.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FOR_EACH_FRONT_INSERT_1_HPP_ER_2009
#define BOOST_MATH_IFGT_FOR_EACH_FRONT_INSERT_1_HPP_ER_2009
#include <boost/range.hpp>
#include <boost/iterator/insert_element_every_n_step_iterator.hpp>
namespace boost{
namespace math{
namespace ifgt{

        template<typename std::size_t final_weight_size,typename R1>
        typename
            result_of_make_range_insert_element_every_n_step<const R1>::type
        for_each_front_insert_1(const R1& range_rest_weight){
            typedef typename range_value<R1>::type value_type;
            return make_range_insert_element_every_n_step(
                range_rest_weight,
                static_cast<value_type>(1),
                final_weight_size - 1
            );
        }

}
}
}
#endif
