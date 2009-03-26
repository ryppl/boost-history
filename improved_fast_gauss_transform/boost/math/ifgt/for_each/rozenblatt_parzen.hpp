//////////////////////////////////////////////////////////////////////////////
//  rozenblatt_parzen.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_IFGT_FOR_EACH_ROZENBLATT_PARZEN_HPP_ER_2009
#define BOOST_IFGT_FOR_EACH_ROZENBLATT_PARZEN_HPP_ER_2009
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/range.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/iterator/vector2matrix_iterator.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
#include <boost/math/ifgt/functor/rozenblatt_parzen.hpp>
namespace boost{
namespace math{
namespace ifgt{

template<typename R0,typename R1,typename D>
void for_each_rozenblatt_parzen(
    const R0& targets,//t[0][0],...,t[0][D],...,t[n-1][0],...,t[n-1][D]
    R1& ranges_out, //r[0],...,r[n-1]
    const crtp::evaluator<D>& e
){
    const std::size_t stride = D::source_size;
    const D& e_ = static_cast<const D&>(e);
    std::transform(
        make_vector2matrix_iterator(begin(targets),stride),
        make_end_vector2matrix_iterator(begin(targets),end(targets),
            stride),
        begin(ranges_out),
        make_rozenblatt_parzen(e_)
    );
}


}//ifgt
}//math
}//boost

#endif
