////////////////////////////////////////////////////////////////////////////
// exact_normal_given_normal.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/format.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/range.hpp>

#include <boost/shared_features/feature/keyword.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/mpl_features.hpp>
#include <boost/shared_features/feature/scalar.hpp>
#include <boost/shared_features/functor/sample.hpp>

#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/parameter/normal.hpp>
#include <boost/conditionally_specified_distribution/parameter/dependent/normal_given_normal.hpp>

#include <boost/conditionally_specified_distribution/sampler/normal.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>
#include <libs/conditionally_specified_distribution/src/example/sampler/exact_normal_given_normal.h>


void example_sampler_exact_normal_given_normal(){
    std::cout << "->example_exact_sample_normal_given_normal" << std::endl;

    using namespace boost;
    namespace shared = shared_features;
    namespace csd = conditionally_specified_distribution;
    namespace par = csd::parameter;
    namespace sampl = csd::sampler;

    // data type
    typedef double value_type;

    // identifiers
    typedef mpl::size_t<0>                  id_x;
    typedef mpl::size_t<1>                  id_y;
    typedef mpl::pair<id_x,id_y>            id_xy;
    typedef id_xy                           id_rho_xy;

    // atomic features
    typedef shared::feature::scalar<value_type,id_x>        state_x; //1
    typedef shared::feature::scalar<value_type,id_y>        state_y; //2
    typedef par::feature::normal<value_type,id_x>           par_x;   //3
    typedef par::feature::normal<value_type,id_y>           par_y;   //4
    typedef shared::feature::scalar<value_type,id_rho_xy>   rho_xy;  //5

    // dependent feature
    typedef
        par::feature::normal_given_normal<
            par_y,
            state_x,
            par_x,
            rho_xy
        > //6
                                                         par_y_given_x;
    //sampler
    typedef sampl::feature::normal<par_y_given_x,state_y> sampl_y_given_x;//7


    typedef mpl::vector<sampl_y_given_x>                 implicit_features;
    typedef shared_features::set<implicit_features>     set_t;
    typedef set_t::mpl_features::type                   wrapped_features;

    typedef mpl::transform<
        wrapped_features,
       shared::detail::feature_tag<mpl::_1>
    >::type                                            features;
    typedef shared::detail::features<implicit_features>::sorted_vector
                                                       sorted_vector;
    typedef mpl::size<features>::type                   actual_size;
    typedef mpl::size_t<7>                              desired_size;
    BOOST_MPL_ASSERT((
        mpl::equal_to<actual_size,desired_size>
    ));
    typedef mpl::back<features>::type             actual_back;
    typedef sampl_y_given_x                       desired_back;
    BOOST_MPL_ASSERT((
        is_same<desired_back,actual_back>
    ));

    // input
    const value_type state_x_val     = -0.1;
    const value_type state_y_val     = 0.1;
    const value_type mu_x_val        = -0.1;
    const value_type sigma_x_val     = 1.0;
    const value_type mu_y_val        = 0.1;
    const value_type sigma_y_val     = 1.5;
    const value_type rho_val         = 0.5;

    set_t set;

    set.initialize(
        (
            shared::kwd<id_x>::scalar               = state_x_val,
            shared::kwd<id_y>::scalar               = state_y_val,
            par::kwd<id_x>::mu                      = mu_x_val,
            par::kwd<id_x>::sigma                   = sigma_x_val,
            par::kwd<id_y>::mu                      = mu_y_val,
            par::kwd<id_y>::sigma                   = sigma_y_val,
            shared::kwd<id_rho_xy>::scalar          = rho_val
        )
    );

    typedef mpl::vector<sampl_y_given_x>                    samplers;
    typedef shared::contains_feature_of<samplers,mpl::_>    is_sampler;

    typedef boost::mt19937                                  urng_type;
    const unsigned seed  = 0;
    urng_type urng((urng_type)(seed));

    BOOST_ASSERT((
        set.extract<state_y>()() == state_y_val
    ));

    typedef std::vector<value_type> draws_t;
    const unsigned n = 1e6;
    draws_t draws;
    draws.reserve(n);

    std::string str = "%1% samples from y|x, with (x,y) jointly normal, ";
    str += " and x fixed at %2%";
    str += "mu_x = %3% ";
    str += "sigma_x = %4% ";
    str += "rho_xy = %5% ";
    str += "mu_y = %6% ";
    str += "sigma_y = %7% ";
    str += "so that mu_y|x = %8%, sigma_y|x = %9%";
    str += "and a normal sampler.";
    format the_format(str);
    the_format % n ;
    the_format % set.extract<state_x>()();
    the_format % set.extract<par_x>().mu();
    the_format % set.extract<par_x>().sigma();
    the_format % set.extract<rho_xy>()();
    the_format % set.extract<par_y>().mu();
    the_format % set.extract<par_y>().sigma();
    the_format % set.extract<par_y_given_x>().mu();
    the_format % set.extract<par_y_given_x>().sigma();
    std::cout << the_format.str() << std::endl;


    for(unsigned i=0; i<n; ++i){
        set.visit_if<is_sampler>(
            shared::functor::sample(),
            (sampl::kwd<>::random_number_generator = urng)
        );
        draws.push_back(
            set.extract<state_y>()()
        );
    }

    std::cout << "sample statistics : " << std::endl;

    value_type m = std::accumulate(begin(draws),end(draws),(value_type)(0));
    m/=(value_type)(n);

    std::cout << " mu_y|x  : " << m << std::endl;

    value_type s = std::inner_product(
        begin(draws),
        end(draws),
        begin(draws),
        (value_type)(0)
    );
    s -= n*m*m;
    s = sqrt(s/(value_type)(n));

    std::cout << " mu_y|x  : " << s << std::endl;

    std::cout << "<-" << std::endl;

}
