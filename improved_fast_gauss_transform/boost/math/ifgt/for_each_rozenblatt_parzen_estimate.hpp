//////////////////////////////////////////////////////////////////////////////
// ifgt/for_each_rozenblatt_parzen_estimate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FOR_EACH_ROZENBLATT_PARZEN_ESTIMATE_HPP_ER_2009
#define BOOST_MATH_IFGT_FOR_EACH_ROZENBLATT_PARZEN_ESTIMATE_HPP_ER_2009
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/iterator/vector2matrix_iterator.hpp>
#include <boost/math/ifgt/call_rozenblatt_parzen_estimate.hpp>
#include <boost/math/ifgt/call_wrapper.hpp>
#include <boost/math/ifgt/evaluator_base.hpp>
namespace boost{namespace math{namespace ifgt{
template<typename R0, typename R1, typename Common,typename Derived>
void for_each(
    const R0& targets,
    R1& ranges_out,
    const evaluator_base<Common,Derived>& e,
    const call<rozenblatt_parzen_estimate>& method
){
    const std::size_t stride = Derived::source_size;
    transform(
        make_vector2matrix_iterator(begin(targets),stride),
        make_end_vector2matrix_iterator(begin(targets),end(targets),
            stride),
        begin(ranges_out),
        make_rozenblatt_parzen_estimate(e)
    );
}
}}}
#endif
