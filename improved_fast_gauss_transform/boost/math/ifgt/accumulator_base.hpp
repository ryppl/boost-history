//////////////////////////////////////////////////////////////////////////////
// ifgt/accumulator_base.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_ACCUMULATOR_BASE_HPP_ER_2009
#define BOOST_MATH_IFGT_ACCUMULATOR_BASE_HPP_ER_2009

namespace boost{namespace math{namespace ifgt{

    template<typename Derived>
    struct accumulator_base{};

    // For now these two are just informative
    template<typename Derived>
    struct exact_accumulator_base : accumulator_base<Derived>{};

    template<typename Derived>
    struct fast_accumulator_base  : accumulator_base<Derived>{};

}}}
#endif
