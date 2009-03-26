///////////////////////////////////////////////////////////////////////////////
// coefficients_evaluator.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/math/monomials.hpp>
#include <boost/math/ifgt/detail/zscore.hpp>
#include <boost/math/ifgt//detail/normal_kernel_properties.hpp>
#include <boost/math/ifgt/fast/detail/coefficients.hpp>
#include <boost/math/ifgt/fast/detail/coefficients_evaluator.hpp>
#include <libs/math/ifgt/src/example/coefficients_evaluator.h>

void example_coefficients_evaluator(){
    std::cout << "-> coefficients_evaluator" << std::endl;
    using namespace boost::math;
    using namespace boost::math::ifgt;
    const unsigned int dim = 2;
    typedef double                                       value_type;
    typedef std::vector<value_type>                      var_type;
    typedef std::vector<unsigned>                        ints_type;
    typedef coefficients<dim>                       cs_type;
    typedef coefficients_evaluator<var_type>        cs_evaluator_type;
    typedef normal_kernel_properties<dim,value_type> rpp_type;

    double bandwidth        =   0.1;
    int    degree           =   5;
    double cutoff_radius    =   0.1;
    var_type    center;
    var_type    source;
    var_type    target;
    value_type  weight = 0.1;
    {
        using namespace boost::assign;
        center  += (-0.1), 0.1;
        source  += (-0.2), 0.2;
        target  += (-0.15),0.15;
    }

    var_type zscore_val(dim);
    monomials<var_type> monoms;
    {
        typedef zscore<var_type>                  zscore_type;
        zscore_type zscore_op(center);
        zscore_op(source,bandwidth,zscore_val);
    }
    value_type zscore_sqnorm = std::inner_product(
        boost::begin(zscore_val),
        boost::end(zscore_val),
        boost::begin(zscore_val),
        (value_type)(0)
    );

    monoms(zscore_val,degree);

    cs_type cs;
    cs(
        zscore_sqnorm,
        monoms(),
        weight,
        degree
    );

    cs_evaluator_type cse(target);
    value_type inv_nc = rpp_type::normalizing_constant(bandwidth);
    typedef cs_evaluator_type::result_type result_type;
    result_type res = cse(
            cutoff_radius,
            center,
            bandwidth,
            inv_nc,
            cs
        );

    std::cout << "res=" << res << std::endl;

    std::cout << "<-" << std::endl;
}
