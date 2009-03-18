////////////////////////////////////////////////////////////////////////////
// pair_independent.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/shared_features/include.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>
#include <boost/conditionally_specified_distribution/parameter/normal.hpp>
#include <boost/conditionally_specified_distribution/parameter/pair_independent.hpp>
#include <boost/conditionally_specified_distribution/function/include.hpp>
#include <libs/conditionally_specified_distribution/src/example/function/pair_independent.h>

void example_function_pair_independent(){

    std::cout << "->example_function_pair_independent" << std::endl;
    using namespace boost;
    namespace shared = shared_features;
    namespace csd = conditionally_specified_distribution;
    namespace par = csd::parameter;

    // data type
    typedef double value_type;

    // identifiers
    typedef mpl::size_t<0>                  id_x;
    typedef mpl::size_t<1>                  id_y;
    typedef mpl::pair<id_x,id_y>            id_xy;
    typedef id_xy                           id_rho_xy;

    // atomic features
    typedef par::feature::normal<value_type,id_x>           par_x;   //3
    typedef par::feature::normal<value_type,id_y>           par_y;   //4

    //Attention : par_x and par_y are of different type due to id_x, id_y

    typedef par::feature::pair_independent<par_x,par_y>     par_joint_xy;

    typedef mpl::vector<par_joint_xy>                     implicit_features;
    typedef shared_features::set<implicit_features>         set_t;

    const value_type mu_x_val        = -0.1;
    const value_type sigma_x_val     = 1.0;
    const value_type mu_y_val        = 0.1;
    const value_type sigma_y_val     = 1.5;

    set_t set;

    set.initialize(
        (
            par::kwd<id_x>::mu                      = mu_x_val,
            par::kwd<id_x>::sigma                   = sigma_x_val,
            par::kwd<id_y>::mu                      = mu_y_val,
            par::kwd<id_y>::sigma                   = sigma_y_val
        )
    );

    typedef std::vector<value_type> x_values_t;
    x_values_t x_values;
    {
    	using namespace boost::assign;
        x_values+= mu_x_val - sigma_x_val * 3.0;
        x_values+= mu_x_val + sigma_x_val * 3.0;
        x_values+= mu_y_val - sigma_y_val * 3.0;
        x_values+= mu_y_val + sigma_y_val * 3.0;
    }


    for(unsigned int i = 0; i<size(x_values); i++){

        value_type ulpdf =
        csd::function::unnormalized_log_pdf(
            set.extract<par_x>(),
            ((shared::kwd_set = set)),
            x_values[i]
        );
        ulpdf += csd::function::unnormalized_log_pdf(
            set.extract<par_y>(),
            ((shared::kwd_set = set)),
            x_values[i]
        );
        value_type ulpdf2 =
        csd::function::unnormalized_log_pdf(
            set.extract<par_joint_xy>(),
            ((shared::kwd_set = set)),
            x_values[i]
        );
        BOOST_ASSERT(ulpdf == ulpdf2);
        value_type dulpdf =
        csd::function::derivative_unnormalized_log_pdf(
            set.extract<par_x>(),
            ((shared::kwd_set = set)),
            x_values[i]
        );
        dulpdf += csd::function::derivative_unnormalized_log_pdf(
            set.extract<par_y>(),
            ((shared::kwd_set = set)),
            x_values[i]
        );
        value_type dulpdf2 =
        csd::function::derivative_unnormalized_log_pdf(
            set.extract<par_joint_xy>(),
            ((shared::kwd_set = set)),
            x_values[i]
        );
        BOOST_ASSERT(dulpdf == dulpdf2);

    }
    std::cout << " OK <-" << std::endl;
}
