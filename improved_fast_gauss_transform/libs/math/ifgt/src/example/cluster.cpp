///////////////////////////////////////////////////////////////////////////////
// example/cluster.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/mpl/placeholders.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/cluster.hpp>
#include <libs/math/ifgt/src/example/cluster.h>

void example_cluster(){
    std::cout << "-> cluster" << std::endl;

    namespace mpl = boost::mpl;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag  = ifgt::tag;

    const unsigned dim = 2;
    const unsigned wdim = 2;
    typedef double                                            value_type;
    typedef std::vector<value_type>                           var_type;
    typedef ifgt::truncation_degree_constant<mpl::_1>         trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>     cluster_type;

    typedef std::vector<unsigned>                           ints_type;
    typedef cluster_type::coefficients_type                 coeffs_type;
    typedef cluster_type::collection_coefficients_type coll_coeffs_type;

    const value_type bandwidth    =   0.1;
    const value_type max_cluster_radius = 1.0;
    const unsigned degree = 20;
    ints_type   degrees;
    var_type    center;
    var_type    source;
    var_type    weight;
    {
        using namespace boost::assign;
        center  += (-0.1), 0.1;
        source  += (-0.2), 0.2;
        weight +=  1.0,   0.1;
    }

    cluster_type cluster(
        (tag::center=center,
        tag::bandwidth=bandwidth,
        tag::max_cluster_radius = max_cluster_radius,
        tag::degree = degree) );

    cluster_type cluster_cpy(cluster);

    cluster(source,weight);

    {
        const coeffs_type& sc
            = cluster.collection_coefficients()[0];

        std::cout << "weigths[0]: coeffs = ";
        copy(
            boost::begin(sc()),
            boost::end(sc()),
            std::ostream_iterator<value_type>(std::cout, " ")
        ); std::cout << std::endl;
    }
    {
        const coeffs_type& sc
            = cluster.collection_coefficients()[1];

        std::cout << "weigths[1]: coeffs = ";
        copy(
            boost::begin(sc()),
            boost::end(sc()),
            std::ostream_iterator<value_type>(std::cout, " ")
        ); std::cout << std::endl;
    }


    std::cout << "<-" << std::endl;
}
