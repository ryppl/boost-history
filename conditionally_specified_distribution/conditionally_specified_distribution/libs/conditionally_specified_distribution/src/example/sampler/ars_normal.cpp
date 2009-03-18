////////////////////////////////////////////////////////////////////////////
// ars_normal.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <utility>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/shared_features/feature/keyword.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/mpl_features.hpp>
#include <boost/shared_features/feature/scalar.hpp>
#include <boost/shared_features/functor/sample.hpp>

#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>
#include <boost/conditionally_specified_distribution/parameter/normal.hpp>

#include <boost/conditionally_specified_distribution/sampler/adaptive_rejection/sampler.hpp>
#include <libs/conditionally_specified_distribution/src/example/sampler/ars_normal.h>

void example_sampler_ars_normal(){
    std::cout << "-> example_ars_normal()" << std::endl;

    using namespace boost;
    namespace shared = shared_features;
    namespace csd = conditionally_specified_distribution;
    namespace par = csd::parameter;
    namespace sampl = csd::sampler;

    // data type
    typedef double value_type;

    // identifiers
    typedef mpl::size_t<0>                                  id_x;

    // atomic features
    typedef shared::feature::scalar<value_type,id_x>        state_x; //1
    typedef par::feature::normal<value_type,id_x>           par_x;   //2

    //sampler
    typedef sampl::feature::adaptive_rejection_sampler<
        par_x,
        state_x
    >  sampl_x;//3

    typedef mpl::vector<sampl_x>                        implicit_features;
    typedef shared_features::set<implicit_features>     set_t;
    typedef set_t::mpl_features::type                   wrapped_features;

    typedef mpl::transform<
        wrapped_features,
       shared::detail::feature_tag<mpl::_1>
    >::type                                            features;
    typedef shared::detail::features<implicit_features>::sorted_vector
                                                       sorted_vector;
    typedef mpl::size<features>::type                   actual_size;
    typedef mpl::size_t<3>                              desired_size;

    BOOST_MPL_ASSERT((
        mpl::equal_to<actual_size,desired_size>
    ));

    // input
    const value_type state_x_val     = -0.1;
    const value_type mu_x_val        = -5.1;
    const value_type sigma_x_val     = 10.0;

    //dependent
    typedef std::pair<value_type,value_type> interval_t;
    const value_type l = mu_x_val - 1.96 * sigma_x_val;
    const value_type u = mu_x_val + 1.96 * sigma_x_val;
    interval_t interval(l,u);

    set_t set;
    set.initialize(
        (
            shared::kwd<id_x>::scalar      = state_x_val,
            par::kwd<id_x>::mu             = mu_x_val,
            par::kwd<id_x>::sigma          = sigma_x_val,
            sampl::kwd<id_x>::ars_init_interval     = interval
        )
    );

    typedef mpl::vector<sampl_x>                            samplers;
    typedef shared::contains_feature_of<samplers,mpl::_>    is_sampler;

    typedef boost::mt19937                                  urng_type;
    const unsigned seed  = 0;
    urng_type urng((urng_type)(seed));

    BOOST_ASSERT((
        set.extract<state_x>()() == state_x_val
    ));

    typedef std::vector<value_type> draws_t;
    const unsigned n = 1e6;
    draws_t draws;
    draws.reserve(n);

    std::string str = "%1% samples from N(%2%,%3%^2)";
    str += " using an adaptive rejection sampler, initialized with";
    str += "x1 = %4% and x2 = %5%";
    format the_format(str);
    the_format % n;
    the_format % set.extract<par_x>().mu();
    the_format % set.extract<par_x>().sigma();
    the_format % l % u;
    std::cout << the_format.str() << std::endl;
    for(unsigned i=0; i<n; ++i){

        set.visit_if<is_sampler>(
            shared::functor::sample(),
            (sampl::kwd<>::random_number_generator = urng)
        );
        draws.push_back(
            set.extract<state_x>()()
        );
    }

    //copy(
    //    begin(draws),
    //    end(draws),
    //    std::ostream_iterator<value_type>(std::cout," ")
    //);

    value_type m = std::accumulate(begin(draws),end(draws),(value_type)(0));
    m/=(value_type)(n);

    std::cout << "sample statistics : " << std::endl;
    std::cout
        << " mu :" << m
        << std::endl;

    std::transform(
        begin(draws),
        end(draws),
        begin(draws),
        bind(
            std::minus<value_type>(),
            _1,
            m
        )
    );

    value_type s = std::inner_product(
        begin(draws),
        end(draws),
        begin(draws),
        (value_type)(0)
    );
    s = sqrt(s/(value_type)(n));

    std::cout
        << "sigma : "
        << s
        << std::endl;

    std::cout << "<-" << std::endl;

}
