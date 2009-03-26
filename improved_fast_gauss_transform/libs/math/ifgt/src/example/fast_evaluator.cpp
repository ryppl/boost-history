///////////////////////////////////////////////////////////////////////////////
// fast_evaluator.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/math/ifgt/include.hpp>
#include <libs/math/ifgt/src/example/fast_evaluator.h>

void example_fast_evaluator(){

    std::cout << "-> example_fast_evaluator" << std::endl;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag = ifgt::tag;
    namespace mpl = boost::mpl;

    const unsigned int dim = 2;
    const unsigned int wdim = 2;
    typedef double                                              value_type;
    typedef std::vector<value_type>                             var_type;
    typedef ifgt::truncation_degree::constant<mpl::_1>         trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>     cluster_type;
    typedef ifgt::find_nearest_cluster<mpl::_1, boost::l2_distance_squared>
                                                                find_type;
    typedef ifgt::fast::accumulator<cluster_type,find_type>     acc_type;
    typedef ifgt::kwd<>                                         kwd_t;
    value_type bandwidth            = 0.1;
    value_type max_cluster_radius   = 1.0;
    unsigned degree                 = 20;
    var_type    center0;
    var_type    center1;
    var_type    center2;
    var_type    source1;
    var_type    source2;
    var_type    weight;
    var_type    target;
    {
        using namespace boost::assign;
        center0  += -1.1,  0.1;
        center1  +=  0.2, -0.2;
        center2  +=  0.1,  0.1;
        source1  +=  0.15,-0.15;//nearest to center1
        source2  +=  0.11, 0.11;//nearest to center2
        weight   +=  1.0,   0.1;
        target  += (-0.15),0.15;
    }

    acc_type acc((
            kwd_t::bandwidth = bandwidth,
            kwd_t::max_cluster_radius = max_cluster_radius,
            kwd_t::degree = degree
        )
    );

    acc(center0, weight);
    acc(center1, weight);
    acc(center2, weight);
    acc(source1, weight);
    acc(source2, weight);
    std::cout
        << "sources_count = " << acc.sources_count()
        << std::endl;

    typedef ifgt::fast::evaluator<acc_type,
        ifgt::cutoff_radius::none<mpl::_1> > eval_type;
    eval_type eval((kwd_t::accumulator = acc));

    var_type range_out(wdim);

    ifgt::make_gauss_transform(eval)(target,range_out);

    std::cout << "gauss_transform =" << std::endl;
    std::copy(
        boost::begin(range_out),
        boost::end(range_out),
        std::ostream_iterator<value_type>(std::cout, " ")
    );

    value_type rpe = ifgt::make_rozenblatt_parzen(eval)(target);
    std::cout << "rozenblatt_parzen_estimate =" << rpe << std::endl;

    var_type range_out_nw(wdim-1);
    ifgt::make_nadaraya_watson(eval)(target,range_out_nw); //wrong size

    std::cout << "nadaya_watson_estimate =" << std::endl;
    std::copy(
        boost::begin(range_out_nw),
        boost::end(range_out_nw),
        std::ostream_iterator<value_type>(std::cout, " ")
    );

    std::cout << "<-" << std::endl;

};
