///////////////////////////////////////////////////////////////////////////////
//  as_random_distribution.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/normal_distribution.hpp>

#include <boost/generator/clock.hpp>
#include <boost/generator/ref.hpp>
#include <boost/random_regression/as_random_distribution.hpp>


void example_as_random_distribution(){
    std::cout << "->example_as_random_distribution" << std::endl;

    using namespace boost;
    namespace gen = boost::generator;
    namespace ran = boost::random;
    typedef  double value_type;
    typedef boost::mt19937                                  urng_t;
    typedef urng_t::result_type                             input_t;
    typedef  gen::clock<value_type> clock_t;
    typedef  as_random_distribution<clock_t,input_t>   as_r_clock_t;
    typedef  gen::ref<clock_t>  ref_clock_t;
    typedef  as_random_distribution<ref_clock_t> as_r_ref_clock_t;

    typedef boost::mt19937                                  urng_t;
    typedef uniform_real<value_type>                        unif_t;
    typedef variate_generator<urng_t&,as_r_clock_t>         gen_clock_t;
    typedef variate_generator<urng_t&,as_r_ref_clock_t>     gen_ref_clock_t;
    //TODO :
    typedef variate_generator<urng_t&,unif_t>               gen_unif_t;

    const value_type start = 1.0;
    const value_type delta = 0.1;
    const unsigned n = 10;
    const unsigned seed  = 0;

    as_r_clock_t as_r_clock((
        gen::kwd<>::clock_start_time = start,
        gen::kwd<>::clock_delta_time = delta
    ));
    clock_t clock((
        gen::kwd<>::clock_start_time = start,
        gen::kwd<>::clock_delta_time = delta
    ));
    ref_clock_t ref_clock(shared_ptr<clock_t>(new clock_t(clock)));


    typedef std::vector<value_type> vec_t;
    vec_t vec;

    urng_t urng((urng_t)(seed));
    gen_clock_t gen_clock(urng,as_r_clock);
    gen_ref_clock_t gen_ref_clock(urng,as_r_ref_clock_t(ref_clock));

    std::generate_n(back_inserter(vec),n,gen_ref_clock);
    vec.clear();
    std::generate_n(back_inserter(vec),n,gen_clock);

    value_type zero = ((value_type)(n-1))*delta + start;
    zero -= vec.back();

    BOOST_ASSERT(
       ( zero < 1e-10 ) && ( zero > -1e-10 )
    );

    vec.clear();
    std::generate_n(back_inserter(vec),n,gen_clock);

    zero = ((value_type)(n-1))*delta + start;
    zero -= vec.back();

    BOOST_ASSERT(
       ( zero < 1e-10 ) && ( zero > -1e-10 )
    );

    copy(
        begin(vec),
        end(vec),
        std::ostream_iterator<value_type>(std::cout," ")
    ); std::cout << std::endl;

    vec.clear();
    std::generate_n(back_inserter(vec),n,gen_ref_clock);

    zero = ((value_type)(2 * n-1))*delta + start;
    zero -= vec.back();

    BOOST_ASSERT(
       ( zero < 1e-10 ) && ( zero > -1e-10 )
    );

    copy(
        begin(vec),
        end(vec),
        std::ostream_iterator<value_type>(std::cout," ")
    ); std::cout << std::endl;

    std::cout << " <-" << std::endl;
}
