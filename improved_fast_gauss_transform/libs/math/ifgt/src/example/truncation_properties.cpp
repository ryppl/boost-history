///////////////////////////////////////////////////////////////////////////////
// example/truncation_properties.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <iterator>
#include <boost/range.hpp>
#include <boost/math/ifgt/include.hpp>
#include <libs/math/ifgt/src/example/truncation_properties.h>

void example_truncation_properties(){
    std::cout << "->example_truncation_properties" << std::endl;
    using namespace boost;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    typedef double                                      value_type;
    typedef std::vector<value_type>                     vec_type;
    typedef ifgt::truncation_properties                 prop_type;
    typedef ifgt::optimal_parameter_given_max_degree<value_type>
                                                        opt_pars_type;

    value_type h  = 0.5; //Raykar2006 probably meant h=0.5 in Figure 6
    value_type rx = 0.5;
    value_type ry = 1e15;
    value_type eps = 1e-3;
//    unsigned max_p = 40;
    unsigned n = 100;
    opt_pars_type opt_pars((ifgt::tag::error_tolerance = eps));

    value_type log10 = log((value_type)(10));
    vec_type wry;
    vec_type log10_err;
    vec_type log10_err_bound;
//    vec_type opt_max_radius;
    value_type value;
    for(unsigned i = 0; i<n; i++){
        value_type ryi = 5 * (value_type)(i+1)/((value_type)(n));
        value = prop_type::log_error_bound(15, h, rx, ryi)/log10;
        log10_err.push_back(value);
        value_type rxi = (value_type)(i+1)/((value_type)(n));
        value = prop_type::worst_target_to_center_distance(40, h, rxi);
        wry.push_back(value);
        value = prop_type::max_log_error_bound(40, h, rxi, ry)/log10;
        log10_err_bound.push_back(value);

    };

    std::cout << " ->log10_errs - Figure 5a in Raykar2006a" << std::endl;
    std::copy(
        begin(log10_err),
        end(log10_err),
        std::ostream_iterator<value_type>(std::cout," ")
    );
    std::cout << " <-" << std::endl;

    std::cout << " -> worst ry" << std::endl;
    std::copy(
        begin(wry),
        end(wry),
        std::ostream_iterator<value_type>(std::cout," ")
    );
    std::cout << " <-" << std::endl;

    std::cout
        << " ->log10_err_bounds - Figure 6a in Raykar2006a"
        << std::endl;
    std::copy(
        begin(log10_err_bound),
        end(log10_err_bound),
        std::ostream_iterator<value_type>(std::cout," ")
    );
    std::cout << " <-" << std::endl;

    std::cout << "<-" << std::endl;
};
