///////////////////////////////////////////////////////////////////////////////
// example/exact_gauss_transform.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/math/ifgt/tag.hpp>
#include <boost/math/ifgt/exact_contribution.hpp>
#include <boost/math/ifgt/exact_accumulator.hpp>
#include <boost/math/ifgt/exact_contributions_evaluator.hpp>
#include <boost/math/ifgt/exact_evaluator.hpp>
#include <libs/math/ifgt/src/example/exact_gauss_transform.h>

void example_exact_gauss_transform(){

    std::cout << "-> example_exact_gauss_transform" << std::endl;

    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag  = ifgt::tag;

    const unsigned int dim = 2;
    const unsigned int wdim = 2;

    typedef double                                      value_type;
    typedef std::vector<double>                         var_type;
    typedef std::vector<unsigned>                       ints_type;
    typedef ifgt::exact_contribution<dim,wdim,var_type> exact_contrib_type;
    typedef std::vector<exact_contrib_type>             contributions_type;
    typedef ifgt::exact_accumulator<dim,wdim,var_type>  exact_acc_type;
    typedef ifgt::exact_evaluator<exact_acc_type>       exact_eval_type;

    const double bandwidth            = 0.5;
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
        weight  +=  1.0,   0.1;
        target  += (-0.15),0.15;
    }

    var_type range_out0(dim,(value_type)(0));
    var_type range_out1(dim,(value_type)(0));

    exact_acc_type exact_acc(
        (tag::bandwidth = bandwidth, tag::contributions_reserve_size = 2));
    exact_acc(center0,weight);
    exact_acc(center1,weight);
    exact_acc(center2,weight);
    exact_acc(source1,weight);
    exact_acc(source2,weight);

    make_exact_contributions_evaluator(exact_acc.contributions())(
        target,range_out0);

    std::cout << "gauss transform" << std::endl;
    std::cout << "(contributions evaluator)" << std::endl;
    copy(
        boost::begin(range_out0),
        boost::end(range_out0),
        std::ostream_iterator<value_type>(std::cout," ")
    ); std::cout << std::endl;

    exact_eval_type exact_eval((tag::accumulator = exact_acc));
    exact_eval.gauss_transform(target,range_out1);

    std::cout << "(exact evaluator)" << std::endl;
    copy(
        boost::begin(range_out1),
        boost::end(range_out1),
        std::ostream_iterator<value_type>(std::cout," ")
    ); std::cout << std::endl;

    BOOST_ASSERT(
        range_out0 == range_out1
    );

    //-> TODO remove
        std::cout << "contributions()[0].source()" << std::endl;
        std::cout << "(exact_acc.)" << std::endl;
        copy(
            boost::begin(exact_acc.contributions()[0].source()),
            boost::end(exact_acc.contributions()[0].source()),
            std::ostream_iterator<value_type>(std::cout, " ")
        ); std::cout << std::endl;
        std::cout << "(exact_eval.accumulator().)" << std::endl;
        copy(
            boost::begin(exact_eval.accumulator().contributions()[0].source()),
            boost::end(exact_eval.accumulator().contributions()[0].source()),
            std::ostream_iterator<value_type>(std::cout, " ")
        ); std::cout << std::endl;
    //<-

    value_type hat_pdf = exact_eval.rozenblatt_parzen_estimate(target);
    std::cout << "rp estimate" << hat_pdf << std::endl;

    std::cout << "<-" << std::endl;

};
