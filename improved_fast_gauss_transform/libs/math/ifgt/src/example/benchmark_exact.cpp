//////////////////////////////////////////////////////////////////////////////
// benchmark_exact.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/math/ifgt/benchmark.hpp>
#include <boost/math/ifgt/fast/accumulator.hpp>
#include <boost/math/ifgt/exact/accumulator.hpp>
#include <boost/math/ifgt/fast/evaluator.hpp>
#include <boost/math/ifgt/exact/evaluator.hpp>
#include <boost/math/ifgt/bandwidth/normal_plug_in.hpp>
#include <libs/math/ifgt/src/example/benchmark_exact.h>
void example_benchmark_exact(){
    std::cout << " -> example_benchmark_exact" << std::endl;
    std::string str =
        "pdf(x) = prod{N(x[d]|0,1),d=0,...,D}";
    str+= "estimated by Rozenblatt-Parzen";
    str+= "and f(x) = norm(x) = sqrt(x[0]^2+...+x[D]^2) by Nadaraya-Watson";
    std::cout << str << std::endl;

    using namespace boost;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;

    typedef double                                          value_type;
    typedef std::vector<value_type>                         var_type;
    typedef mt19937                                         urng_type;
    typedef normal_distribution<value_type>                 nd_type;
    typedef variate_generator<urng_type&,nd_type>           gen_type;
    typedef std::size_t                                     size_type;
    typedef std::vector<size_t>                             sizes_type;
    typedef math::normal_distribution<value_type>           univ_distr_type;
    typedef ifgt::product_pdf<univ_distr_type>              pdf_fun_type;
    typedef l2_norm                                         w_fun_type;
    // fixed constants (do not modify)
    const value_type  sigma     = 1.0;
    const size_type wdim        = 2;

    // other constants (you may modify them)
    // Warning: if you get a bad alloc error during exec,
    // increase loop_count and try again
    const size_type sdim                = 3;
    const size_type train_count         = 2.5e3;
    const size_type test_count          = 1e2;
    const size_type loop_count          = 4;
    const value_type bandwidth_adjust   = 3;

    // dependent constant
    size_type   all_train_count  = train_count * loop_count;

    typedef ifgt::benchmark<
        value_type,
        test_count,
        sdim,
        wdim,
        pdf_fun_type,
        w_fun_type>                                         bench_type;
    typedef ifgt::bandwidth_normal_plug_in<sdim,value_type> opt_bandwidth_type;
    typedef ifgt::exact::accumulator<sdim,wdim,var_type>     exact_acc_type;
    typedef ifgt::exact::evaluator<exact_acc_type>           exact_eval_type;

    //pdf
    // Data generator
    urng_type urng(0);
    gen_type gen(urng,nd_type());

    bench_type bench(
        gen,
        pdf_fun_type(univ_distr_type(0,1)),
        w_fun_type()
    );

    std::cout << "dim(x) = " << sdim << std::endl;
    std::cout << "dim(w) = " << wdim << std::endl;

    value_type  opt_h
        = opt_bandwidth_type::find(
        sigma,all_train_count);
    opt_h *= bandwidth_adjust; //override

    std::cout << "bandwidth ="
        << opt_h  << std::endl;

    exact_acc_type  exact_acc((ifgt::kwd<>::bandwidth = opt_h  ));
    exact_eval_type exact_eval((ifgt::kwd<>::accumulator = exact_acc));

    bench.notation(std::cout);
    bench.header(std::cout); std::cout << std::endl;
    for(unsigned i = 0; i<loop_count; i++){
        bench.accumulate(gen,train_count,exact_acc);
        bench.estimate_pdf(exact_eval);
        bench.estimate_w(exact_eval);
        bench.statistics(std::cout,i); std::cout << std::endl;
    }

}
