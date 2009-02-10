//////////////////////////////////////////////////////////////////////////////
// ifgt/for_each_accumulate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FOR_EACH_ACCUMULATE_HPP_ER_2009
#define BOOST_MATH_IFGT_FOR_EACH_ACCUMULATE_HPP_ER_2009
#include <boost/iterator/for_each_over_2_vector2matrix.hpp>
#include <boost/math/ifgt/traits.hpp>
namespace boost{namespace math{namespace ifgt{

    template<typename R0, typename R1, typename Acc>
    void for_each(
        const R0& sources,
        const R1& weights,
        accumulator_base<Acc>& acc
    ){
        typedef Acc derived_type;
        typedef traits<derived_type> the_traits;
        typedef for_each_over_2_vector2matrix<the_traits> for_each_type;
        derived_type& derived = static_cast<derived_type&>(acc);
        for_each_type f(derived);
        f(sources,weights);
    };

}}}
#endif
