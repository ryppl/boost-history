//////////////////////////////////////////////////////////////////////////////
// example/benchmark_fast.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/range.hpp>
#include <boost/math/ifgt/benchmark.hpp>
#include <libs/math/ifgt/src/example/benchmark_fast.h>
void example_benchmark_fast(){
    std::cout << " -> example_benchmark_fast" << std::endl;
    std::string str =
        "pdf(x) = prod{N(x[d]|0,1),d=0,...,D} estimated by Rozenblatt-Parzen";
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
    const size_type   wdim      = 2;

    // Other constants (you may modify them)
    // Warning: if you get a bad alloc error during exec,
    // increase bandwidth_adjust or train_count
    const size_type sdim                = 3;
    const size_type train_count         = 2.5e3;
    const size_type test_count          = 1e2;
    const size_type loop_count          = 4;
    const value_type bandwidth_adjust   = 3;

    //
    //const size_type sdim                = 3;
    //const size_type train_count         = 2.5e5;
    //const size_type test_count          = 1e2;
    //const size_type loop_count          = 4;
    //const value_type bandwidth_adjust   = 4;

    // fast addtional parameters
    const bool do_cutoff_rydg           = false;
    const value_type error_tol          = 1e-2;
    const size_type max_p               = 20;
    const value_type start_rx_div_h     = 2;
    const size_type max_recursions      = 20;


    // dependent constant
    size_type   all_train_count  = train_count * loop_count;
    // additional statistics
    sizes_type  clusters_count(0);

    typedef ifgt::benchmark<
        value_type,
        test_count,
        sdim,
        wdim,
        pdf_fun_type,
        w_fun_type>                                         bench_type;
    typedef ifgt::optimal_bandwidth<sdim,value_type> opt_bandwidth_type;
    //fast types
    typedef ifgt::optimal_parameter_given_max_degree<value_type>
                                                              opt_pars_type;
    typedef ifgt::truncation_degree_constant<mpl::_1>         trunc_degree;
    typedef ifgt::cluster<sdim,wdim,trunc_degree,var_type>    cluster_type;
    typedef ifgt::find_nearest_cluster<mpl::_1, boost::l2_distance_squared>
                                                              find_type;
    typedef ifgt::fast_accumulator<cluster_type,find_type>    fast_acc_type;
    typedef mpl::if_<
        mpl::bool_<do_cutoff_rydg>,
        mpl::identity<ifgt::cutoff_radius_rydg<mpl::_1> >,
        mpl::identity<ifgt::cutoff_radius_none<mpl::_1> >
    >::type                                                    cutoff_policy;

    typedef ifgt::fast_evaluator<fast_acc_type,cutoff_policy> fast_eval_type;

    //pdf
    // Data generator
    urng_type urng(0);
    gen_type gen(urng,nd_type());

    bench_type bench(
        gen,
        pdf_fun_type(univ_distr_type(0,1)),
        w_fun_type()
    );
    std::cout << "do cutoff = " << do_cutoff_rydg << std::endl;
    std::cout << "dim(x) = " << sdim << std::endl;
    std::cout << "dim(w) = " << wdim << std::endl;

    value_type  opt_h
        = opt_bandwidth_type::for_density_estimation_under_normality(
        sigma,all_train_count);
    opt_h *= bandwidth_adjust; //override

    //fast specific parameters
    opt_pars_type opt_pars((ifgt::tag::error_tolerance = error_tol));
    opt_pars(max_p, opt_h, start_rx_div_h, max_recursions);
    value_type  max_rx = opt_pars.max_radius();

    std::cout << "bandwidth =" << opt_h  << std::endl;
    std::cout << " max_rx=" << max_rx << std::endl;

    fast_acc_type fast_acc(
        (
            ifgt::tag::bandwidth = opt_h,
            ifgt::tag::max_cluster_radius = max_rx,
            ifgt::tag::degree = max_p,
            ifgt::tag::error_tolerance = error_tol
        )
    );
    fast_eval_type fast_eval((
        ifgt::tag::accumulator = fast_acc,
        ifgt::tag::error_tolerance = error_tol));

    bench.notation(std::cout);
    std::cout << "K : # clusters" << std::endl;
    bench.header(std::cout);
    std::cout << boost::format("%|1$-10|")%"K" << std::endl;
    for(unsigned i = 0; i<loop_count; i++){
        bench.accumulate(gen,train_count,fast_acc);
        clusters_count.push_back(boost::size(fast_acc.clusters()));
        bench.estimate_pdf(fast_eval);
        bench.estimate_w(fast_eval);
        bench.statistics(std::cout,i);
        std::cout
            << boost::format("%|1$-10|")% clusters_count.back()<<std::endl;
    }

}
