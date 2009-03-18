/////////////////////////////////////////////////////////////////////////////
//  generate_fixed_design_no_censoring.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/survival_data/include.hpp>


void example_survival_data_generate_fixed_design_no_censoring(){
  std::cout << "-> example_survival_data_generate_fixed_design_no_censoring"
    << std::endl;
    using namespace boost;

    namespace gen =  boost::generator;
    namespace ran =  boost::random;
    namespace rg =   boost::random_regression;
    namespace surv = boost::survival_data;
    typedef double                                  value_type;
    typedef std::vector<value_type>                 cont_t;
    typedef cont_t::size_type                       size_type;
    typedef cont_t                                  range_covariate_t;
    typedef cont_t                                  range_beta_t;
    typedef boost::mt19937                          urng_type;
    typedef exponential_distribution<value_type>    exp_t;
    // Only meant to illustrate (possible) use of id
    typedef mpl::void_                                      fail_id_t;
    typedef rg::exponential<exp_t,uniform_real,fail_id_t>    gen_fail_t;

    typedef surv::generate_fixed_design_no_censoring<gen_fail_t> surv_rdist_t;
    typedef variate_generator<urng_type&,surv_rdist_t> surv_gen_t;

    const unsigned      seed       = 0;
    const unsigned      sims_count = 100;
    const value_type    start_time = 0.0;
    const value_type    delta_time = 0.1;

    typedef surv_gen_t::result_type     unit_t;
    typedef std::vector<unit_t>         range_data_t;
    range_data_t range_data;
    {
        std::vector<range_covariate_t> fixed_design;
        {using namespace boost::assign;
            range_covariate_t cov0, cov1, cov2, cov3;
            cov0+=-0.5,-0.5;
            cov1+=-0.5, 0.5;
            cov2+= 0.5,-0.5;
            cov3+= 0.5, 0.5; //fixed design
            fixed_design+=cov0,cov1,cov2,cov3;
        };
        range_beta_t true_beta;

        {using namespace boost::assign; true_beta+=-0.1,0.2;};

        urng_type urng((urng_type::result_type)(seed));
        surv_rdist_t surv_rdist(
            start_time,
            delta_time,
            fixed_design,
            (
                rg::kwd<fail_id_t>::coefficients = true_beta
            )
        );

        std::generate_n(
            back_inserter(range_data),
            sims_count,
            surv_gen_t(urng,surv_rdist)
        );

        BOOST_ASSERT(size(range_data)==sims_count);

        for(unsigned n=0; n< sims_count; n++){
            unsigned k = n % size(fixed_design);
            range_covariate_t zero;
            value_type zero_norm = 0.0;
            zero.clear();
            std::transform(
                begin(range_data[n].range_covariate()),
                end(range_data[n].range_covariate()),
                begin(fixed_design[k]),
                back_inserter(zero),
                std::minus<value_type>()
            );
            zero_norm = *max_element(begin(zero),end(zero));
            BOOST_ASSERT((zero_norm>-1e-10) && (zero_norm<1e-10));
            zero_norm = *min_element(begin(zero),end(zero));
            BOOST_ASSERT((zero_norm>-1e-10) && (zero_norm<1e-10));

            zero_norm =  range_data[n].entry_time();
            zero_norm -= (n * delta_time+ start_time);
            BOOST_ASSERT((zero_norm>-1e-10) && (zero_norm<1e-10));

        }//end loop

        typedef surv::unit_at_time<unit_t> unit_at_time_t;
        typedef std::vector<unit_at_time_t> range_data_at_time_type;
        value_type time_analysis = math::tools::max_value<value_type>();
        range_data_at_time_type range_data_at_time;

        transform_if_to_unit_at_time(
            begin(range_data),
            end(range_data),
            back_inserter(range_data_at_time),
            time_analysis
        );

        typedef std::vector<value_type> fail_times_t;
        fail_times_t fail_times;
        transform(
            begin(range_data_at_time),
            end(range_data_at_time),
            back_inserter(fail_times),
            bind(
                &unit_at_time_t::event_time,
                _1
            )
        );

        std::cout << "fail times: "<< std::endl;
        copy(
            begin(fail_times),
            end(fail_times),
            std::ostream_iterator<value_type>(std::cout, " ")
        );

        std::cout << " OK <-" << std::endl;
    }
}
