//////////////////////////////////////////////////////////////////////////////
// evaluate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_IFGT_FOR_EACH_EVALUATE_HPP_ER_2009
#define BOOST_IFGT_FOR_EACH_EVALUATE_HPP_ER_2009
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/iterator/for_each_over_2_vector2matrix.hpp>
#include <boost/math/ifgt/for_each/detail/make_traits.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
namespace boost{
namespace math{
namespace ifgt{

// Usage: for_each_evaluate<M>(targets,ranges_out,e)
// M : method for evaluating : meta::gauss_transform, meta::nadaraya_watson
template<
    typename M,
    typename R0, typename R1,
    typename D
>
void for_each_evaluate(
    const R0& targets,//t[0][0],...,t[0][D],...,t[n-1][0],...,t[n-1][D]
    R1& ranges_out, //r[0][0],...,r[0][J-1],...,r[n-1][0],...,r[n-1][J-1]
    const crtp::evaluator<D>& e
){
    typedef typename mpl::apply1<M,D>::type call_type;
    typedef make_traits<
        call_type,
        call_type::stride0,
        call_type::stride1
    >   traits;

    typedef for_each_over_2_vector2matrix<traits> for_each_type;
    const D& e_ = static_cast<const D&>(e);
    call_type call(e_);
    for_each_type f( call );
    f(targets,ranges_out);
}



}//ifgt
}//math
}//boost
#endif
