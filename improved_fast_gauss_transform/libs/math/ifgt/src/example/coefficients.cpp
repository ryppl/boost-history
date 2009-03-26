///////////////////////////////////////////////////////////////////////////////
// coefficients.cpp
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
#include <boost/math/ifgt/fast/detail/coefficients.hpp>
#include <libs/math/ifgt/src/example/coefficients.h>

void example_coefficients(){
    std::cout << "-> coefficients" << std::endl;
    using namespace boost;
    const unsigned int dim = 2;
    typedef double                                       value_type;
    typedef std::vector<value_type>                      var_type;
    typedef std::vector<unsigned>                        ints_type;
    typedef math::ifgt::coefficients<dim>                coeffs_type;
    double bandwidth    =   0.1;
    int    degree       =   5;
    var_type    center;
    var_type    source;
    value_type  weight = 0.1;
    {
        using namespace boost::assign;
        center  += (-0.1), 0.1;
        source  += (-0.2), 0.2;
    }

    var_type zscore_val(dim);
    math::ifgt::monomials<var_type> monoms;
    {
        typedef math::zscore<var_type>                  zscore_type;
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

    coeffs_type coeffs;
    coeffs(
        zscore_sqnorm,
        monoms(),
        weight,
        degree
    );
    copy(
        boost::begin(coeffs()),
        boost::end(coeffs()),
        std::ostream_iterator<double>(std::cout, " ")
    ); std::cout << std::endl;

    std::cout << "<-" << std::endl;
}
