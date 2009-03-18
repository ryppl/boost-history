////////////////////////////////////////////////////////////////////////////
// normal_given_normal.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/shared_features/feature/keyword.hpp>
#include <boost/shared_features/set.hpp>
#include <boost/shared_features/mpl_features.hpp>
#include <boost/shared_features/feature/scalar.hpp>

#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/parameter/normal.hpp>
#include <boost/conditionally_specified_distribution/parameter/dependent/normal_given_normal.hpp>

#include <libs/conditionally_specified_distribution/src/example/parameter/normal_given_normal.h>

void example_parameter_normal_given_normal(){
    std::cout << "->example_parameter_normal_given_normal" << std::endl;
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
        >
                                                        par_y_given_x;

    typedef mpl::vector<par_y_given_x>                 implicit_features;
    typedef shared_features::set<implicit_features>     set_t;

    typedef set_t::mpl_features::type                   wrapped_features;

    typedef mpl::transform<
        wrapped_features,
       shared::detail::feature_tag<mpl::_1>
    >::type                                            features;

    typedef shared::detail::features<implicit_features>::sorted_vector
                                                       sorted_vector;

    typedef mpl::size<features>::type                   actual_size;
    typedef mpl::size_t<5>                              desired_size;

    BOOST_MPL_ASSERT((
        mpl::equal_to<actual_size,desired_size>
    ));

    typedef mpl::back<features>::type             actual_back;
    typedef par_y_given_x                         desired_back;

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

    BOOST_ASSERT(set.extract<state_x>()()==state_x_val);
    //BOOST_ASSERT(set.extract<state_y>()()==scalar_y);
    BOOST_ASSERT(set.extract<par_x>().mu()==mu_x_val);
    BOOST_ASSERT(set.extract<par_x>().sigma()==sigma_x_val);
    BOOST_ASSERT(set.extract<par_y>().mu()==mu_y_val);
    BOOST_ASSERT(set.extract<par_y>().sigma()==sigma_y_val);

    std::cout
        << "mu_y|x="    <<  set.extract<par_y_given_x>().mu()   << std::endl;

    std::cout
        << "beta_y|x=" <<  set.extract<par_y_given_x>().beta()
                                                                << std::endl;
    set.extract<state_x>().set(0.0);
    BOOST_ASSERT(set.extract<state_x>()()==0.0);

    set.extract<par_y_given_x>().update((shared::kwd_set=set));

    std::cout
        << "mu_y|x="    <<  set.extract<par_y_given_x>().mu()   << std::endl;

    std::cout << "<-" << std::endl;

}
