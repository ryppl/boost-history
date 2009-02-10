///////////////////////////////////////////////////////////////////////////////
// example/for_each_evaluate.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/exact_accumulator.hpp>
#include <boost/math/ifgt/fast_accumulator.hpp>
#include <boost/math/ifgt/fast_evaluator.hpp>
#include <boost/math/ifgt/truncation_degree_constant.hpp>
#include <boost/math/ifgt/for_each_accumulate_rest_weights.hpp>
#include <boost/math/ifgt/for_each_evaluate.hpp>
#include <boost/math/ifgt/call_wrapper.hpp>
#include <boost/math/ifgt/call_gauss_transform.hpp>
#include <boost/math/ifgt/call_rozenblatt_parzen_estimate.hpp>
#include <boost/math/ifgt/call_nadaraya_watson_estimate.hpp>
#include <boost/math/ifgt/for_each_rozenblatt_parzen_estimate.hpp>
#include <boost/math/ifgt/for_each_evaluate.hpp>
#include <libs/math/ifgt/src/example/for_each_evaluate.h>

void example_for_each_evaluate(){
    std::cout << "-> example_for_each_evaluate" << std::endl;

    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag = ifgt::tag;
    namespace mpl = boost::mpl;

    const unsigned int dim = 2;
    const unsigned int wdim = 2;
    typedef double                                             value_type;
    typedef std::vector<value_type>                            var_type;
    typedef ifgt::truncation_degree_constant<mpl::_1>           trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>       cluster_type;
    typedef ifgt::find_nearest_cluster<mpl::_1, boost::l2_distance_squared>
                                                                find_type;

    typedef  ifgt::fast_accumulator<cluster_type,find_type>    fast_acc_type;
    typedef  ifgt::exact_accumulator<dim,wdim,var_type>       exact_acc_type;


    value_type bandwidth            = 0.1;
    value_type max_cluster_radius   = 1.0;
    unsigned degree             = 20;
    var_type    sources;
    var_type    weights;
    {
        using namespace boost::assign;
        sources  += -1.1,  0.1;
        weights  +=  1.0,   0.1;
        sources  +=  0.2, -0.2;
        weights  +=  1.0,   0.1;
        sources  +=  0.1,  0.1;
        weights  +=  1.0,   0.1;
        sources  +=  0.15,-0.15;
        weights  +=  1.0,   0.1;
        sources  +=  0.11, 0.11;
        weights  +=  1.0,   0.1;
    }

    fast_acc_type fast_acc((
            tag::bandwidth = bandwidth,
            tag::max_cluster_radius = max_cluster_radius,
            tag::degree = degree
        )
    );

    ifgt::for_each(sources,weights,fast_acc);

    exact_acc_type exact_acc((tag::bandwidth = bandwidth));
    ifgt::for_each(sources,weights,exact_acc);

    var_type    targets;
    {
        using namespace boost::assign;
        targets  += (-0.15),0.15;//1
        targets  += (-0.15),0.15;//2
        targets  += (-0.15),0.15;//3
        targets  += (-0.15),0.15;//4
        targets  += (-0.15),0.15;//5
    }
    var_type::size_type targets_count = boost::size(targets)/dim;


    typedef ifgt::cutoff_radius_none<mpl::_1>           cutoff_policy_type;
    typedef ifgt::fast_evaluator<fast_acc_type, cutoff_policy_type>
                                                            fast_eval_type;
    fast_eval_type eval((tag::accumulator = fast_acc));

    var_type ranges_out( targets_count * wdim);
    std::cout << "gauss_transform->" << std::endl;
    ifgt::for_each(
        targets,
        ranges_out,
        eval,
        ifgt::call<ifgt::gauss_transform>()
    );
    copy(
        boost::begin(ranges_out),
        boost::end(ranges_out),
        std::ostream_iterator<value_type>(std::cout," ")
    );
    std::cout << "<-" << std::endl;
    std::cout << "rozenblatt_parzen_estimate->" << std::endl;

    var_type rp_out( targets_count * 1);
    ifgt::for_each(
        targets,
        rp_out,
        eval,
        ifgt::call<ifgt::rozenblatt_parzen_estimate>()
    );
    copy(
        boost::begin(rp_out),
        boost::end(rp_out),
        std::ostream_iterator<value_type>(std::cout," ")
    );
    std::cout << "<-" << std::endl;

    var_type nw_out( targets_count * (wdim-1) );
    std::cout << "nadaraya_watston_estimate->" << std::endl;
    ifgt::for_each(
        targets,
        nw_out,
        eval,
        ifgt::call<ifgt::nadaraya_watson_estimate>()
    );
    copy(
        boost::begin(nw_out),
        boost::end(nw_out),
        std::ostream_iterator<value_type>(std::cout," ")
    );

    std::cout << "<-" << std::endl;

    std::cout << "<-" << std::endl;
};
