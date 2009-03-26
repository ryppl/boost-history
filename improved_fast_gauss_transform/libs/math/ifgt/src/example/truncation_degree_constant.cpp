///////////////////////////////////////////////////////////////////////////////
// truncation_degree_constant.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <vector>
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/truncation_degree/constant.hpp>
#include <boost/math/ifgt/keyword.hpp>
#include <libs/math/ifgt/src/example/truncation_degree_constant.h>
void example_truncation_degree_constant(){
    std::cout << "-> example_truncation_degree_constant" << std::endl;

    using namespace boost::math::ifgt;

    const unsigned degree = 5;
    const unsigned dim = 2;

    typedef truncation_degree::constant<>   truncation_degree_type;
    typedef std::vector<unsigned>       degrees_type;
    typedef double                      value_type;
    typedef std::vector<value_type>     var_type;

    value_type bandwidth = 0.1;
    value_type sqrt_dist_source_to_center = 0.1;

    truncation_degree_type a((kwd<>::degree = degree));

    var_type    weight;
    {
        using namespace boost::assign;
        weight +=  1.0,   0.1;
    }
    degrees_type degrees(dim);

    a(
        bandwidth,
        sqrt_dist_source_to_center,
        weight,
        degrees
    );

    copy(
        boost::begin(degrees),
        boost::end(degrees),
        std::ostream_iterator<unsigned>(std::cout, " ")
    );//should return degree, degree

    std::cout << "<-" << std::endl;
}
