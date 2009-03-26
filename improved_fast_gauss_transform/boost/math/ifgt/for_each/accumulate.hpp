//////////////////////////////////////////////////////////////////////////////
// accumulate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FOR_EACH_ACCUMULATE_HPP_ER_2009
#define BOOST_MATH_IFGT_FOR_EACH_ACCUMULATE_HPP_ER_2009
#include <boost/mpl/size.hpp>
#include <boost/iterator/for_each_over_2_vector2matrix.hpp>
#include <boost/math/ifgt/crtp/accumulator.hpp>
#include <boost/math/ifgt/for_each/detail/make_traits.hpp>

namespace boost{namespace math{namespace ifgt{

    template<typename R0, typename R1, typename D>
    void for_each_accumulate(
        const R0& sources,
        const R1& weights,
        crtp::accumulator<D>& d
    ){

        typedef make_traits<D,D::source_size,D::weight_size> traits;
        typedef for_each_over_2_vector2matrix<traits> for_each_type;
        D& d_ = static_cast<D&>(d);
        for_each_type f(d_);
        f(sources,weights);
    };

}}}
#endif
