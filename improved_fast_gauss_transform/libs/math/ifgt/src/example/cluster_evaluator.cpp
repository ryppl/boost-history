///////////////////////////////////////////////////////////////////////////////
// example/cluster_evaluator.cpp
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
#include <boost/mpl/placeholders.hpp>
#include <boost/math/ifgt/cluster.hpp>
#include <boost/math/ifgt/truncation_degree_constant.hpp>
#include <boost/math/ifgt/cluster_evaluator.hpp>
#include <boost/math/ifgt/cutoff_radius_none.hpp>
#include <boost/math/ifgt/tag.hpp>
#include <libs/math/ifgt/src/example/cluster_evaluator.h>
void example_cluster_evaluator(){
    std::cout << "-> cluster_evaluator" << std::endl;
    namespace mpl = boost::mpl;
    namespace math =  boost::math;
    namespace ifgt =  math::ifgt;
    namespace tag = ifgt::tag;

    const unsigned int dim = 2;
    const unsigned int wdim = 2;
    typedef double                                              value_type;
    typedef std::vector<value_type>                             var_type;

    typedef ifgt::truncation_degree_constant<mpl::_1>         trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>     cluster_type;

    typedef cluster_type::coefficients_type                coeffs_type;
    typedef cluster_type::collection_coefficients_type     coll_coeffs_type;
    typedef ifgt::cutoff_radius_none<value_type>       cutoff_policy_type;
    typedef ifgt::cluster_evaluator<
        var_type,var_type,ifgt::cutoff_radius_none<mpl::_1> >    cluster_eval_type;

    const value_type bandwidth          =   0.1;
    const value_type max_cluster_radius =   1.0;
    const unsigned   degree             =   20;
    //ints_type   degrees;
    var_type    center;
    var_type    source;
    var_type    target;
    var_type    weight;
    {
        using namespace boost::assign;
        center  += (-0.1), 0.1;
        source  += (-0.2), 0.2;
        target  +=  -0.15, 0.15;
        weight +=  1.0,   0.1;
        //degrees +=  5,     5;
    }

    cluster_type cluster((
        tag::center = center,
        tag::bandwidth = bandwidth,
        tag::max_cluster_radius = max_cluster_radius,
        tag::degree = degree
    ) );
    std::cout << "before?" << std::endl;
    cluster(source,weight);
    std::cout << "after?" << std::endl;

    var_type range_out(wdim,(value_type)(0));

    cutoff_policy_type cutoff_policy;
    cluster_eval_type cluster_eval(
        (
            tag::range_out = range_out,
            tag::target = target,
            tag::cutoff_radius_policy = cutoff_policy
        )
    );

    cluster_eval(cluster);

    copy(
        boost::begin(range_out),
        boost::end(range_out),
        std::ostream_iterator<value_type>(std::cout, " ")
    );

    std::cout << "<-" << std::endl;
}
