///////////////////////////////////////////////////////////////////////////////
// for_each_accumulate.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/exact/accumulator.hpp>
#include <boost/math/ifgt/fast/accumulator.hpp>
#include <boost/math/ifgt/truncation_degree/constant.hpp>
#include <boost/math/ifgt/for_each/accumulate.hpp>
#include <boost/math/ifgt/for_each/weight_front_insert_1_accumulate.hpp>
#include <libs/math/ifgt/src/example/for_each_accumulate.h>

void example_for_each_accumulate(){
    std::cout << "-> example_for_each_fast_accumulator" << std::endl;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag = ifgt::tag;
    namespace mpl = boost::mpl;

    const unsigned int dim = 2;
    const unsigned int wdim = 2;
    typedef std::vector<double>                              var_type;
    typedef ifgt::truncation_degree::constant<mpl::_1>       trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>    cluster_type;
    typedef ifgt::find_nearest_cluster<mpl::_1, boost::l2_distance_squared>
                                                            find_type;
    typedef ifgt::kwd<>                                     kwd_t;
    typedef ifgt::fast::accumulator<cluster_type,find_type> fast_acc_type;
    typedef ifgt::exact::accumulator<dim,wdim,var_type>     exact_acc_type;


    double bandwidth            = 0.1;
    double max_cluster_radius   = 1.0;
    unsigned degree             = 20;
    var_type    sources;
    var_type    weights;
    var_type    r_weights;
    {
        using namespace boost::assign;
        sources   += -1.1,  0.1;
        weights   +=  1.0,  0.1;
        r_weights +=        0.1;

        sources   +=  0.2, -0.2;
        weights   +=  1.0,  0.1;
        r_weights +=        0.1;

        sources   +=  0.1,  0.1;
        weights   +=  1.0,  0.1;
        r_weights +=        0.1;

        sources   +=  0.15,-0.15;
        weights   +=  1.0,  0.1;
        r_weights +=        0.1;

        sources   +=  0.11, 0.11;
        weights   +=  1.0,  0.1;
        r_weights +=        0.1;
    }

    std::cout << "sources count" << std::endl;

    fast_acc_type fast_acc((
            kwd_t::bandwidth = bandwidth,
            kwd_t::max_cluster_radius = max_cluster_radius,
            kwd_t::degree = degree
        )
    );
    ifgt::for_each_accumulate(sources,weights,fast_acc);
    std::cout
        << "fast (weights) " << fast_acc.sources_count()
        << std::endl;

    fast_acc_type fast_acc2((
            kwd_t::bandwidth = bandwidth,
            kwd_t::max_cluster_radius = max_cluster_radius,
            kwd_t::degree = degree
        )
    );
    ifgt::for_each_weight_front_insert_1_accumulate(sources,
        r_weights,fast_acc2);
    std::cout
        << "fast (rest weights) " << fast_acc2.sources_count()
        << std::endl;

    exact_acc_type exact_acc((kwd_t::bandwidth = bandwidth));
    ifgt::for_each_accumulate(sources,weights,exact_acc);

    std::cout
        << "exact (weights) " << exact_acc.sources_count()
        << std::endl;

    exact_acc_type exact_racc((kwd_t::bandwidth = bandwidth));
    ifgt::for_each_weight_front_insert_1_accumulate(sources,
        r_weights,exact_racc);

    std::cout
        << "exact (rest weights) " << exact_racc.sources_count()
        << std::endl;


    std::cout << "<-" << std::endl;

};
