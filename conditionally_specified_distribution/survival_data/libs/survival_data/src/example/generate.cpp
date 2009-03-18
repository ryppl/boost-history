/////////////////////////////////////////////////////////////////////////////
//  generate.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/survival_data/include.hpp>
#include <libs/survival_data/src/example/generate.h>

    void example_survival_data_generate(){
        std::cout << "-> example_survival_data_generate" << std::endl;
        using namespace boost;
        typedef mpl::int_<0> fail_id;
        typedef mpl::int_<1> cens_id;

        namespace gen =  boost::generator;
        namespace ran =  boost::random;
        namespace rg =  boost::random_regression;
        namespace surv = boost::survival_data;
        typedef double                                  value_type;
        typedef std::vector<value_type>                 cont_t;
        typedef boost::mt19937                          urng_type;
        typedef gen::clock<value_type>                  gen_clock_type;
        typedef gen::ref<gen_clock_type>                ref_clock_type;
//        typedef as_random_distribution<ref_clock_type>  clock_type;
        typedef gen::cycle_through_shared_containers<cont_t>     gen_cov_t;
        typedef exponential_distribution<value_type>             exp_t;
        // Only meant to illustrate (possible) use of id
        typedef rg::exponential<exp_t,uniform_real,fail_id>     gen_fail_t;
        typedef rg::exponential<exp_t,uniform_real,cens_id>     gen_cens_t;
        typedef std::vector<value_type>                 covariate_t;
        typedef std::vector<covariate_t>                levels_t;
        typedef surv::generate<
            ref_clock_type, //important to have ref if stop and go
            gen_cov_t,
            gen_fail_t,
            gen_cens_t
        >                                               surv_gen_t;

        const unsigned      seed       = 0;
        const unsigned      sims_count = 10;
        const value_type    start_time = 0.0;
        const value_type    delta_time = 0.1;

        levels_t covs;
        {using namespace boost::assign;
            std::vector<value_type> cov0, cov1;
            cov0+=1.0,0.0;
            cov1+=0.0,1.0;
            covs+=cov0,cov1;
        };
        covariate_t beta;

        {using namespace boost::assign; beta+=-0.1,0.2;};

        urng_type urng((urng_type::result_type)(seed));
        gen_clock_type gen_clock(
            (
                gen::kwd<>::clock_start_time = start_time,
                gen::kwd<>::clock_delta_time = delta_time
            )
        );
        gen_cov_t gen_cov(
            (
                gen::kwd<>::range_containers = covs,
                gen::kwd<>::range_containers_offset = 0
            )
        );
        gen_fail_t gen_fail(
            (
                rg::kwd<fail_id>::coefficients = beta
            )
        );
        gen_cens_t gen_cens(
            (
                rg::kwd<cens_id>::coefficients = beta
            )
        );

        std::cout << " generation separately   " << std::endl;
        for(unsigned int i=0; i<sims_count; i++){
            std::cout << "entry=" << gen_clock() << std::endl;

            typedef gen_cov_t::result_type cov_result_t;
            cov_result_t cov = gen_cov();

            std::cout << "covariate=";
            copy(
                boost::begin(cov()),
                boost::end(cov()),
                std::ostream_iterator<value_type>(std::cout," ")
            ); std::cout << std::endl;

            std::cout
                << "failure time ="
                << gen_fail(urng,cov())
                << std::endl;
            std::cout
                << "censoring time ="
                << gen_cens(urng,cov())
                << std::endl;

        }
        std::cout << " generation simulatneously " << std::endl;

        typedef shared_ptr<gen_clock_type> shared_clock_t;
        shared_clock_t shared_clock(new gen_clock_type(gen_clock));

        surv_gen_t surv_gen(
                ref_clock_type(shared_clock), //cant pass gen_clock directly
                gen_cov,
                gen_fail,
                gen_cens
        );
        urng_type urng1((urng_type::result_type)(seed));
        for(unsigned int i=0; i<sims_count; i++){
            surv_gen_t::result_type u_o = surv_gen(urng1);

            std::cout << "entry=" << u_o.entry_time() << std::endl;
            std::cout << "covariate=";
            copy(
                boost::begin(u_o.covariate()),
                boost::end(u_o.covariate()),
                std::ostream_iterator<value_type>(std::cout," ")
            ); std::cout << std::endl;
            std::cout
                << "event time=" //min(failure,censoring)
                << u_o.event_time((value_type)(1e8))
                << std::endl;
        }
        std::cout << "<-" << std::endl;
    }

