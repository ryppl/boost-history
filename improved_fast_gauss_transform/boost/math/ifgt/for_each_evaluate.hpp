//////////////////////////////////////////////////////////////////////////////
// ifgt/for_each_evaluate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_IFGT_FOR_EACH_EVALUATE_HPP_ER_2009
#define BOOST_IFGT_FOR_EACH_EVALUATE_HPP_ER_2009
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/iterator/for_each_over_2_vector2matrix.hpp>
#include <boost/math/ifgt/traits.hpp>
#include <boost/math/ifgt/evaluator_base.hpp>
#include <boost/math/ifgt/call_wrapper.hpp>
namespace boost{namespace math{namespace ifgt{

template<
    typename R0, typename R1,
    typename Common, typename Derived,template<typename> class Method>
void for_each(
    const R0& targets,
    R1& ranges_out,
    const evaluator_base<Common,Derived>& e,
    const call<Method>& method
){
    typedef Method<Derived> call_type;
    typedef ifgt::call_traits<call_type> traits;
    typedef for_each_over_2_vector2matrix<traits> for_each_type;
    call_type call(e);
    for_each_type f( call );
    f(targets,ranges_out);
}
}}}
#endif
