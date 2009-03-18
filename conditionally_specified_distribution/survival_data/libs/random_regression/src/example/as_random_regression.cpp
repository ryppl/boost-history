///////////////////////////////////////////////////////////////////////////////
//  as_random_regression.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/normal_distribution.hpp>

#include <boost/generator/clock.hpp>
#include <boost/generator/ref.hpp>
#include <boost/random_regression/as_random_distribution.hpp>
#include <boost/random_regression/as_random_regression.hpp>


void example_as_random_regression(){
    std::cout << "->example_as_random_regression" << std::endl;

    using namespace boost;
    namespace gen = boost::generator;
    namespace ran = boost::random;
    typedef  double value_type;

    typedef boost::mt19937                                  urng_t;
    typedef uniform_real<value_type>                        unif_t;
    typedef urng_t::result_type                             input_t;
    typedef std::vector<value_type>                         vec_t;
    typedef gen::clock<value_type> clock_t;
    typedef as_random_distribution<clock_t,input_t> as_rdist_clock_t;
    typedef as_random_regression<as_rdist_clock_t> as_reg_clock_t;

    const value_type start = 1.0;
    const value_type delta = 0.1;
    const unsigned n = 10;
    const unsigned seed  = 0;

    as_rdist_clock_t as_rdist_clock((
        gen::kwd<>::clock_start_time = start,
        gen::kwd<>::clock_delta_time = delta
    ));
    as_reg_clock_t as_reg_clock(
        as_rdist_clock
    );

    vec_t x(3);
    x[0] = 0.0;
    x[1] = 0.1;
    x[2] = 0.2;
    urng_t urng((urng_t)(seed));

    vec_t draws;
    for(unsigned i =0; i< n; ++i){
        value_type y = as_reg_clock(urng,x);
        draws.push_back(y);
    }

    value_type zero = ((value_type)(n-1))*delta + start;
    zero -= draws.back();

    BOOST_ASSERT(
       ( zero < 1e-10 ) && ( zero > -1e-10 )
    );

    copy(
        begin(draws),
        end(draws),
        std::ostream_iterator<value_type>(std::cout," ")
    ); std::cout << std::endl;


    std::cout << " <-" << std::endl;
}
